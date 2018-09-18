
//
// Created by nlv09165 on 11/07/2018.
//
#include <stdlib.h>
#include <string.h>
#include <hdf5.h>
#include <hdf5_hl.h>


#include <ius.h>
#include <iusError.h>
#include <iusTypes.h>
#include <iusUtil.h>
#include <include/iusExperimentImp.h>
#include <include/iusInputFile.h>
#include <include/iusPatternListImp.h>
#include <include/iusPulseDictImp.h>
#include <include/iusReceiveChannelMapDictImp.h>
#include <include/iusFrameList.h>
#include <include/iusSourceDict.h>
#include <include/iusReceiveSettings.h>
#include <include/iusTransducer.h>
#include <include/iusFrameListImp.h>
#include <include/iusSourceDictImp.h>
#include <include/iusReceiveSettingsDictImp.h>
#include <include/iusTransducerImp.h>
#include <include/iusHistoryNode.h>
#include <include/iusHistoryNodeImp.h>

static char *const FRAME_LIST_PATH = "/Frames";
static char *const PATTERN_LIST_PATH="/Patterns";
static char *const PULSE_DICT_PATH="/Pulses";
static char *const PULSE_SOURCE_DICT_PATH="/PulseSources";
static char *const RECEIVE_CHANNEL_MAP_PATH="/ReceiveChannelMaps";
static char *const TRANSMIT_APODIZATION_DICT_PATH="/TransmitApodizations";
static char *const RECEIVE_SETTINGS_DICT_PATH="/ReceiveSettings";
static char *const TRANSDUCER_PATH="/Transducer";
static char *const EXPERIMENT_PATH="/Experiment";
static char *const NUMFRAMES_PATH="/NumFrames";
static char *const IUSVERSION_PATH="/IusVersion";


struct IusInputFile
{
    iuhn_t history;
    const char *pFilename;
    iufl_t frameList;
	iupal_t patternList;
    iupd_t pulseDict;                    /**< a dictionary of pulses */
    iusd_t pulseSourceDict;
	iurcmd_t receiveChannelMapDict;      /**< a dictionary of receiveChannelMaps */
	iutad_t transmitApodizationDict;     /**< a dictionary of transmitApodizations */
	iursd_t receiveSettingsDict;
	iut_t transducer;
	iue_t experiment;
    int numFrames;        /**< The number of frames in the data */
    int IusVersion;       /**< version of input file format */


    //  state variables
    hid_t fileChunkConfig;                /**< file chunck handle */
    hid_t handle;                         /**< file handle */
    hid_t rfDataset;                      /**< dataset handle */
    int currentFrame;                     /**< current frame number */
    int currentPulse;                     /**< current pulse number */

} ;

static iuif_t iusInputFileAlloc
(
	const char *pFilename
)
{
	iuif_t pFileInst = (IusInputFile *)calloc(1, sizeof(IusInputFile));
	if (pFileInst == NULL)
	{
		return IUIF_INVALID;
	}

	pFileInst->numFrames = IUS_DEFAULT_NUM_FRAMES;
	pFileInst->IusVersion = iusGetVersionMajor();
	pFileInst->pFilename = pFilename;
	pFileInst->rfDataset = H5I_INVALID_HID;
	pFileInst->fileChunkConfig = H5I_INVALID_HID;
	pFileInst->frameList = IUFL_INVALID;
	pFileInst->patternList = IUPAL_INVALID;
	pFileInst->pulseDict = IUPD_INVALID;
    pFileInst->pulseSourceDict = IUSD_INVALID;
	pFileInst->receiveChannelMapDict = IURCMD_INVALID;
    pFileInst->transmitApodizationDict = IUTAD_INVALID;
    pFileInst->receiveSettingsDict = IURSD_INVALID;
    pFileInst->transducer = IUT_INVALID;
	pFileInst->experiment = IUE_INVALID;
	if (pFileInst->handle < 0)
	{
		return IUIF_INVALID;
	}
	return pFileInst;
}

// ADT
iuif_t iusInputFileCreate
(
    const char *pFilename
)
{
	if (pFilename == NULL)
	{
		fprintf(stderr, "iusInputFileAlloc: Input arguments can not be NULL \n");
		return IUIF_INVALID;
	}

	iuif_t pFileInst = iusInputFileAlloc(pFilename);
	if (pFileInst == IUIF_INVALID)
	{
		fprintf(stderr, "iusInputFileCreate: calloc of instance failed\n");
		return IUIF_INVALID;
	}

    pFileInst->history = iusHistoryNodeCreate(IUS_INPUT_TYPE,0);
	pFileInst->handle = H5Fcreate(pFilename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
	if (pFileInst->handle == H5I_INVALID_HID)
	{
		iusInputFileDelete(pFileInst);
		return IUIF_INVALID;
	}
	return pFileInst;
}

int iusInputFileDelete
(
    iuif_t iusInputFile
)
{
    int status = IUS_ERR_VALUE;
    if(iusInputFile != NULL)
    {
        free(iusInputFile);
        status = IUS_E_OK;
    }
    return status;
}


// operations

iuif_t iusInputFileLoad
(
    const char *pFilename
)
{
	if (pFilename == NULL)
	{
		fprintf(stderr, "iusInputFileAlloc: Input arguments can not be NULL \n");
		return IUIF_INVALID;
	}

	IusInputFile *pFileInst = iusInputFileAlloc(pFilename);

    // check calloc
	if (pFileInst == IUIF_INVALID)
	{
		fprintf(stderr, "iusInputFileLoad: calloc of instance failed\n");
		return IUIF_INVALID;
	}

    // open  Hdf5 file using default properties.
    pFileInst->handle = H5Fopen( pFilename, H5F_ACC_RDONLY, H5P_DEFAULT );
    if (pFileInst->handle <=0)
    {
        fprintf( stderr, "iusInputFileLoad: could not create file: %s\n", pFilename );
        return IUIF_INVALID;
    }

    // Load instance data
    // Todo: create group @here instead of in Load, see experiment
    pFileInst->history = iusHistoryNodeLoad(pFileInst->handle);
    pFileInst->frameList = iusFrameListLoad(pFileInst->handle, FRAME_LIST_PATH);
    if (pFileInst->frameList == IUFL_INVALID)
    {
        fprintf( stderr, "Warning from iusInputFileLoad: could not load framelist: %s\n", pFilename );
        return IUIF_INVALID;
    }

    // Todo: create group @here instead of in Load, see experiment
    pFileInst->patternList = iusPatternListLoad(pFileInst->handle, PATTERN_LIST_PATH);
    if (pFileInst->patternList == IUPAL_INVALID)
    {
        fprintf( stderr, "Warning from iusInputFileLoad: could not load patterns: %s\n", pFilename );
        return IUIF_INVALID;
    }

    // Load instance data
    // Todo: create group @here instead of in Load, see experiment
    pFileInst->pulseDict = iusPulseDictLoad(pFileInst->handle, PULSE_DICT_PATH);
    if (pFileInst->pulseDict == IUPD_INVALID)
    {
        fprintf( stderr, "Warning from iusInputFileLoad: could not load pulses: %s\n", pFilename );
        return IUIF_INVALID;
    }

    // Load instance data
    // Todo: create group @here instead of in Load, see experiment
    pFileInst->pulseSourceDict = iusSourceDictLoad(pFileInst->handle, PULSE_SOURCE_DICT_PATH);
    if (pFileInst->pulseSourceDict == IUSD_INVALID)
    {
        fprintf( stderr, "Warning from iusInputFileLoad: could not load pulse sources: %s\n", pFilename );
        return IUIF_INVALID;
    }

    // Load instance data
    // Todo: create group @here instead of in Load, see experiment
    pFileInst->receiveChannelMapDict = iusReceiveChannelMapDictLoad(pFileInst->handle, RECEIVE_CHANNEL_MAP_PATH);
	if (pFileInst->receiveChannelMapDict == IURCMD_INVALID)
	{
		fprintf(stderr, "Warning from iusInputFileLoad: could not load receiveChannelMap: %s\n", pFilename);
		return IUIF_INVALID;
	}

    // Load instance data
    // Todo: create group @here instead of in Load, see experiment
    pFileInst->transmitApodizationDict = iusTransmitApodizationDictLoad(pFileInst->handle, TRANSMIT_APODIZATION_DICT_PATH);
    if (pFileInst->transmitApodizationDict == IUTAD_INVALID)
    {
        fprintf(stderr, "Warning from iusInputFileLoad: could not load transmitApodizationDict: %s\n", pFilename);
        return IUIF_INVALID;
    }

    // Load instance data
    // Todo: create group @here instead of in Load, see experiment
    pFileInst->receiveSettingsDict = iusReceiveSettingsDictLoad(pFileInst->handle, RECEIVE_SETTINGS_DICT_PATH);
    if (pFileInst->receiveSettingsDict == IURSD_INVALID)
    {
        fprintf(stderr, "Warning from iusInputFileLoad: could not load receiveSettingsDict: %s\n", pFilename);
        return IUIF_INVALID;
    }

    hid_t group_id = H5Gopen(pFileInst->handle, EXPERIMENT_PATH, H5P_DEFAULT);
    pFileInst->experiment = iusExperimentLoad(group_id);
    if (pFileInst->experiment == IUE_INVALID)
    {
        fprintf(stderr, "Warning from iusInputFileLoad: could not load experiment: %s\n", pFilename);
        return IUIF_INVALID;
    }

    // Load instance data
    // Todo: create group @here instead of in Load, see experiment
    pFileInst->transducer = iusTransducerLoad(pFileInst->handle, TRANSDUCER_PATH);
    if (pFileInst->transducer == IUT_INVALID)
    {
        fprintf(stderr, "Warning from iusInputFileLoad: could not load transducer: %s\n", pFilename);
        return IUIF_INVALID;
    }

    int status = iusHdf5ReadInt( pFileInst->handle, IUSVERSION_PATH, &(pFileInst->IusVersion));
    if( status != IUS_E_OK )
    {
        fprintf(stderr, "Warning from iusInputFileLoad: could not load IusVersion: %s\n", pFilename);
        return IUIF_INVALID;
    }

    status = iusHdf5ReadInt( pFileInst->handle, NUMFRAMES_PATH, &(pFileInst->numFrames));
    if( status != IUS_E_OK )
    {
        fprintf(stderr, "Warning from iusInputFileLoad: could not load numFrames: %s\n", pFilename);
        return IUIF_INVALID;
    }

    H5Gclose(group_id);
    return pFileInst;
}

int iusInputFileSave
(
    iuif_t fileHandle
)
{
    if( fileHandle == NULL ) return IUS_ERR_VALUE;

    herr_t status=0;
//    hsize_t dims[1] = {1};

    // Todo: Handle creation in iusInputFileSave iso iusPulseDictSave, iusPatternListSave, iusReceiveChannelMapDictSave,iusTransmitApodizationDictSave
    // new signature: iusPulseDictSave(fileHandle->pulseDict,fileHandle->handle);

    status |= iusHistoryNodeSave(fileHandle->history, fileHandle->handle);
    status |= iusFrameListSave(fileHandle->frameList, FRAME_LIST_PATH, fileHandle->handle);
    status |= iusPatternListSave(fileHandle->patternList, PATTERN_LIST_PATH, fileHandle->handle);
    status |= iusPulseDictSave(fileHandle->pulseDict, PULSE_DICT_PATH, fileHandle->handle);
    status |= iusSourceDictSave(fileHandle->pulseSourceDict, PULSE_SOURCE_DICT_PATH, fileHandle->handle);
	status |= iusReceiveChannelMapDictSave(fileHandle->receiveChannelMapDict, RECEIVE_CHANNEL_MAP_PATH, fileHandle->handle);
	status |= iusTransmitApodizationDictSave(fileHandle->transmitApodizationDict, TRANSMIT_APODIZATION_DICT_PATH, fileHandle->handle);
    status |= iusReceiveSettingsDictSave(fileHandle->receiveSettingsDict, RECEIVE_SETTINGS_DICT_PATH, fileHandle->handle);

	hid_t group_id = H5Gcreate(fileHandle->handle, EXPERIMENT_PATH, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
	status |= iusExperimentSave(fileHandle->experiment, group_id);
    status |= H5Gclose(group_id);
    status |= iusTransducerSave(fileHandle->transducer, TRANSDUCER_PATH, fileHandle->handle);
    status |= iusHdf5WriteInt( fileHandle->handle, IUSVERSION_PATH, &(fileHandle->IusVersion), 1);
    status |= iusHdf5WriteInt( fileHandle->handle, NUMFRAMES_PATH, &(fileHandle->numFrames), 1);

	return status;

}

int iusInputFileClose
(
    iuif_t fileHandle
)
{
    int status=0;
    if( fileHandle == IUIF_INVALID ) return IUS_FALSE;
    IUS_ASSERT_MEMORY(fileHandle);

    if( fileHandle->fileChunkConfig != H5I_INVALID_HID )
    {
        status |= H5Pclose(fileHandle->fileChunkConfig);
    }

    if( fileHandle->rfDataset != H5I_INVALID_HID )
    {
        status |= H5Dclose(fileHandle->rfDataset);
    }

    // Terminate access to the file.
    if( fileHandle->handle )
    {
        status |= H5Fclose(fileHandle->handle);
    }

    return status;
}

int iusInputFileCompare
(
    iuif_t reference,
    iuif_t actual
)
{
    if ( reference == actual ) return IUS_TRUE;
    if ( reference == NULL || actual == NULL ) return IUS_FALSE;
    if ( iusHistoryNodeCompareWithId(reference->history, actual->history)  == IUS_FALSE ) return IUS_FALSE;

    if ( reference->IusVersion != actual->IusVersion ) return IUS_FALSE;
    if ( reference->numFrames != actual->numFrames ) return IUS_FALSE;
    if ( strcmp(reference->pFilename, actual->pFilename) != 0 ) return IUS_FALSE;
    if ( iusFrameListCompare(reference->frameList, actual->frameList)  == IUS_FALSE ) return IUS_FALSE;
    if ( iusPatternListCompare(reference->patternList, actual->patternList)  == IUS_FALSE ) return IUS_FALSE;
    if ( iusPulseDictCompare(reference->pulseDict, actual->pulseDict)  == IUS_FALSE ) return IUS_FALSE;
    if ( iusSourceDictCompare(reference->pulseSourceDict, actual->pulseSourceDict)  == IUS_FALSE ) return IUS_FALSE;
	if ( iusReceiveChannelMapDictCompare(reference->receiveChannelMapDict, actual->receiveChannelMapDict) == IUS_FALSE) return IUS_FALSE;
    if ( iusTransmitApodizationDictCompare(reference->transmitApodizationDict, actual->transmitApodizationDict)  == IUS_FALSE ) return IUS_FALSE;
    if ( iusReceiveSettingsDictCompare(reference->receiveSettingsDict, actual->receiveSettingsDict)  == IUS_FALSE ) return IUS_FALSE;
    if ( iusTransducerCompare(reference->transducer, actual->transducer) == IUS_FALSE) return IUS_FALSE;
	if ( iusExperimentCompare(reference->experiment, actual->experiment) == IUS_FALSE) return IUS_FALSE;
    return IUS_TRUE;
}

// Getters
iufl_t iusInputFileGetFrameList
(
    iuif_t iusInputFile
)
{
    if ( iusInputFile != NULL )
    {
        return iusInputFile->frameList;
    }
    return IUFL_INVALID;
}

iupal_t iusInputFileGetPatternList
(
    iuif_t iusInputFile
)
{
    if ( iusInputFile != NULL )
    {
        return iusInputFile->patternList;
    }
    return NULL;
}


iupd_t iusInputFileGetPulseDict
(
    iuif_t iusInputFile
)
{
    if ( iusInputFile != NULL )
    {
        return iusInputFile->pulseDict;
    }
    return NULL;
}

iusd_t iusInputFileGetSourceDict
(
    iuif_t fileHandle
)
{
    if (fileHandle != NULL)
    {
        return fileHandle->pulseSourceDict;
    }
    return NULL;
}


iurcmd_t iusInputFileGetReceiveChannelMapDict
(
    iuif_t iusInputFile
)
{
    if (iusInputFile != NULL)
    {
        return iusInputFile->receiveChannelMapDict;
    }
    return NULL;
}

iutad_t iusInputFileGetTransmitApodizationDict
(
    iuif_t fileHandle
)
{
    if (fileHandle != NULL)
    {
        return fileHandle->transmitApodizationDict;
    }
    return NULL;
}

iursd_t iusInputFileGetReceiveSettingsDict
(
    iuif_t fileHandle
)
{
    if (fileHandle != NULL)
    {
        return fileHandle->receiveSettingsDict;
    }
    return NULL;
}

iue_t iusInputFileGetExperiment
(
	iuif_t iusInputFile
)
{
	if ( iusInputFile != NULL )
	{
		return iusInputFile->experiment;
	}
	return NULL;
}

iut_t iusInputFileGetTransducer
(
    iuif_t iusInputFile
)
{
    if ( iusInputFile != NULL )
    {
        return iusInputFile->transducer;
    }
    return NULL;
}



// Setters
int iusInputFileSetFrameList
(
    iuif_t inputFile,
    iufl_t frameList
)
{
    int status = IUS_ERR_VALUE;

    if (inputFile != NULL)
    {
        inputFile->frameList = frameList;
        status = IUS_E_OK;
    }
    return status;
}

int iusInputFileSetPatternList
(
    iuif_t inputFile,
    iupal_t paternList
)
{
    int status = IUS_ERR_VALUE;

    if(inputFile != NULL)
    {
        inputFile->patternList = paternList;
        status = IUS_E_OK;
    }
    return status;
}

int iusInputFileSetPulseDict
(
    iuif_t inputFile,
    iupd_t pulseDict
)
{
    int status = IUS_ERR_VALUE;

    if ( inputFile != NULL )
    {
        inputFile->pulseDict = pulseDict;
        status = IUS_E_OK;
    }
    return status;
}



int iusInputFileSetSourceDict
(
    iuif_t inputFile,
    iusd_t sourceDict
)
{
    int status = IUS_ERR_VALUE;

    if (inputFile != NULL)
    {
        inputFile->pulseSourceDict = sourceDict;
        status = IUS_E_OK;
    }
    return status;
}

int iusInputFileSetReceiveChannelMapDict
(
	iuif_t inputFile,
	iurcmd_t receiveChannelMapDict
)
{
	int status = IUS_ERR_VALUE;

	if(inputFile != NULL)
	{
		inputFile->receiveChannelMapDict = receiveChannelMapDict;
		status = IUS_E_OK;
	}
	return status;
}

int iusInputFileSetTransmitApodizationDict
(
	iuif_t inputFile,
	iutad_t transmitApodizationDict
)
{
	int status = IUS_ERR_VALUE;

	if(inputFile != NULL && transmitApodizationDict != NULL)
	{
		inputFile->transmitApodizationDict = transmitApodizationDict;
		status = IUS_E_OK;
	}
	return status;
}

int iusInputFileSetReceiveSettingsDict
(
    iuif_t inputFile,
    iursd_t transmitApreceiveSettingsDict
)
{
    int status = IUS_ERR_VALUE;

    if(inputFile != NULL && transmitApreceiveSettingsDict != NULL)
    {
        inputFile->receiveSettingsDict = transmitApreceiveSettingsDict;
        status = IUS_E_OK;
    }
    return status;
}



int iusInputFileSetExperiment
(
	iuif_t inputFile,
	iue_t experiment
)
{
	int status = IUS_ERR_VALUE;

	if (inputFile != NULL && experiment != NULL)
	{
		inputFile->experiment = experiment;
		status = IUS_E_OK;
	}
	return status;
}


int iusInputFileSetTransducer
(
    iuif_t inputFile,
    iut_t transducer
)
{
    int status = IUS_ERR_VALUE;

    if (inputFile != NULL && transducer != NULL)
    {
        inputFile->transducer = transducer;
        status = IUS_E_OK;
    }
    return status;
}
