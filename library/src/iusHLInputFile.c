
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
#include <include/iusHLExperimentImp.h>
#include <include/iusHLInputFile.h>
#include <include/iusHLPatternListImp.h>
#include <include/iusHLPulseDictImp.h>
#include <include/iusHLReceiveChannelMapDictImp.h>
#include <include/iusHLFrameList.h>
#include <include/iusHLSourceDict.h>
#include <include/iusHLReceiveSettings.h>
#include <include/iusHLTransducer.h>
#include <include/iusHLFrameListImp.h>
#include <include/iusHLSourceDictImp.h>
#include <include/iusHLReceiveSettingsDictImp.h>

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

static iuif_t iusHLInputFileAlloc
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
iuif_t iusHLInputFileCreate
(
    const char *pFilename
)
{
	if (pFilename == NULL)
	{
		fprintf(stderr, "iusHLInputFileAlloc: Input arguments can not be NULL \n");
		return IUIF_INVALID;
	}

	iuif_t pFileInst = iusHLInputFileAlloc(pFilename);
	if (pFileInst == IUIF_INVALID)
	{
		fprintf(stderr, "iusHLInputFileCreate: calloc of instance failed\n");
		return IUIF_INVALID;
	}

	pFileInst->handle = H5Fcreate(pFilename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
	if (pFileInst->handle == H5I_INVALID_HID)
	{
		iusHLInputFileDelete(pFileInst);
		return IUIF_INVALID;
	}
	return pFileInst;
}

int iusHLInputFileDelete
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

iuif_t iusHLInputFileLoad
(
    const char *pFilename
)
{
	if (pFilename == NULL)
	{
		fprintf(stderr, "iusHLInputFileAlloc: Input arguments can not be NULL \n");
		return IUIF_INVALID;
	}

	IusInputFile *pFileInst = iusHLInputFileAlloc(pFilename);

    // check calloc
	if (pFileInst == IUIF_INVALID)
	{
		fprintf(stderr, "iusHLInputFileLoad: calloc of instance failed\n");
		return IUIF_INVALID;
	}

    // open  Hdf5 file using default properties.
    pFileInst->handle = H5Fopen( pFilename, H5F_ACC_RDONLY, H5P_DEFAULT );
    if (pFileInst->handle <=0)
    {
        fprintf( stderr, "iusHLInputFileLoad: could not create file: %s\n", pFilename );
        return IUIF_INVALID;
    }


    // Load instance data
    // Todo: create group @here instead of in Load, see experiment
    pFileInst->frameList = iusHLFrameListLoad(pFileInst->handle, FRAME_LIST_PATH);
    if (pFileInst->frameList == IUFL_INVALID)
    {
        fprintf( stderr, "Warning from iusHLInputFileLoad: could not load framelist: %s\n", pFilename );
        return IUIF_INVALID;
    }

    // Todo: create group @here instead of in Load, see experiment
    pFileInst->patternList = iusHLPatternListLoad(pFileInst->handle, PATTERN_LIST_PATH);
    if (pFileInst->patternList == IUPAL_INVALID)
    {
        fprintf( stderr, "Warning from iusHLInputFileLoad: could not load patterns: %s\n", pFilename );
        return IUIF_INVALID;
    }

    // Load instance data
    // Todo: create group @here instead of in Load, see experiment
    pFileInst->pulseDict = iusHLPulseDictLoad(pFileInst->handle, PULSE_DICT_PATH);
    if (pFileInst->pulseDict == IUPD_INVALID)
    {
        fprintf( stderr, "Warning from iusHLInputFileLoad: could not load pulses: %s\n", pFilename );
        return IUIF_INVALID;
    }

    // Load instance data
    // Todo: create group @here instead of in Load, see experiment
    pFileInst->pulseSourceDict = iusHLSourceDictLoad(pFileInst->handle, PULSE_SOURCE_DICT_PATH);
    if (pFileInst->pulseSourceDict == IUSD_INVALID)
    {
        fprintf( stderr, "Warning from iusHLInputFileLoad: could not load pulse sources: %s\n", pFilename );
        return IUIF_INVALID;
    }

    // Load instance data
    // Todo: create group @here instead of in Load, see experiment
    pFileInst->receiveChannelMapDict = iusHLReceiveChannelMapDictLoad(pFileInst->handle, RECEIVE_CHANNEL_MAP_PATH);
	if (pFileInst->receiveChannelMapDict == IURCMD_INVALID)
	{
		fprintf(stderr, "Warning from iusHLInputFileLoad: could not load receiveChannelMap: %s\n", pFilename);
		return IUIF_INVALID;
	}

    // Load instance data
    // Todo: create group @here instead of in Load, see experiment
    pFileInst->transmitApodizationDict = iusHLTransmitApodizationDictLoad(pFileInst->handle, TRANSMIT_APODIZATION_DICT_PATH);
    if (pFileInst->transmitApodizationDict == IUTAD_INVALID)
    {
        fprintf(stderr, "Warning from iusHLInputFileLoad: could not load transmitApodizationDict: %s\n", pFilename);
        return IUIF_INVALID;
    }

    // Load instance data
    // Todo: create group @here instead of in Load, see experiment
    pFileInst->receiveSettingsDict = iusHLReceiveSettingsDictLoad(pFileInst->handle, RECEIVE_SETTINGS_DICT_PATH);
    if (pFileInst->receiveSettingsDict == IURSD_INVALID)
    {
        fprintf(stderr, "Warning from iusHLInputFileLoad: could not load receiveSettingsDict: %s\n", pFilename);
        return IUIF_INVALID;
    }

    hid_t group_id = H5Gopen(pFileInst->handle, EXPERIMENT_PATH, H5P_DEFAULT);
    pFileInst->experiment = iusHLExperimentLoad(group_id);
    if (pFileInst->experiment == IUE_INVALID)
    {
        fprintf(stderr, "Warning from iusHLInputFileLoad: could not load experiment: %s\n", pFilename);
        return IUIF_INVALID;
    }

    // Todo: Implement iusHLTransducerLoad/Save!!
//    group_id = H5Gopen(pFileInst->handle, "/Transducer", H5P_DEFAULT);
//    pFileInst->experiment = iusHLTransducerLoad(group_id);
//    if (pFileInst->experiment == IUE_INVALID)
//    {
//        fprintf(stderr, "Warning from iusHLInputFileLoad: could not load transducer: %s\n", pFilename);
//        return IUIF_INVALID;
//    }
//    H5Gclose(group_id);

    int status = iusHdf5ReadInt( pFileInst->handle, IUSVERSION_PATH, &(pFileInst->IusVersion));
    if( status != IUS_E_OK )
    {
        fprintf(stderr, "Warning from iusHLInputFileLoad: could not load IusVersion: %s\n", pFilename);
        return IUIF_INVALID;
    }

    status = iusHdf5ReadInt( pFileInst->handle, NUMFRAMES_PATH, &(pFileInst->numFrames));
    if( status != IUS_E_OK )
    {
        fprintf(stderr, "Warning from iusHLInputFileLoad: could not load numFrames: %s\n", pFilename);
        return IUIF_INVALID;
    }

    H5Gclose(group_id);
    return pFileInst;
}

int iusHLInputFileSave
(
    iuif_t fileHandle
)
{
    if( fileHandle == NULL ) return IUS_ERR_VALUE;

    herr_t status=0;
    hsize_t dims[1] = {1};

    // Todo: Handle creation in iusHLInputFileSave iso iusHLPulseDictSave, iusHLPatternListSave, iusHLReceiveChannelMapDictSave,iusHLTransmitApodizationDictSave
    // new signature: iusHLPulseDictSave(fileHandle->pulseDict,fileHandle->handle);

    status |= iusHLFrameListSave(fileHandle->frameList, FRAME_LIST_PATH, fileHandle->handle);
    status |= iusHLPatternListSave(fileHandle->patternList, PATTERN_LIST_PATH, fileHandle->handle);
    status |= iusHLPulseDictSave(fileHandle->pulseDict, PULSE_DICT_PATH, fileHandle->handle);
    status |= iusHLSourceDictSave(fileHandle->pulseSourceDict, PULSE_SOURCE_DICT_PATH, fileHandle->handle);
	status |= iusHLReceiveChannelMapDictSave(fileHandle->receiveChannelMapDict, RECEIVE_CHANNEL_MAP_PATH, fileHandle->handle);
	status |= iusHLTransmitApodizationDictSave(fileHandle->transmitApodizationDict, TRANSMIT_APODIZATION_DICT_PATH, fileHandle->handle);
    status |= iusHLReceiveSettingsDictSave(fileHandle->receiveSettingsDict, RECEIVE_SETTINGS_DICT_PATH, fileHandle->handle);

	hid_t group_id = H5Gcreate(fileHandle->handle, EXPERIMENT_PATH, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
	status |= iusHLExperimentSave(fileHandle->experiment, group_id);
    status |= H5Gclose(group_id);

    // Todo: implement
//    group_id = H5Gcreate(fileHandle->handle, TRANSDUCER_PATH, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
//    status |= iusHLTransducerSave(fileHandle->transducer, group_id);
//    status |= H5Gclose(group_id);

    status |= iusHdf5WriteInt( fileHandle->handle, IUSVERSION_PATH, &(fileHandle->IusVersion), 1);
    status |= iusHdf5WriteInt( fileHandle->handle, NUMFRAMES_PATH, &(fileHandle->numFrames), 1);

	return status;

}

int iusHLInputFileClose
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

int iusHLInputFileCompare
(
    iuif_t reference,
    iuif_t actual
)
{
    if ( reference == actual ) return IUS_TRUE;
    if ( reference == NULL || actual == NULL ) return IUS_FALSE;
    if ( reference->IusVersion != actual->IusVersion ) return IUS_FALSE;
    if ( reference->numFrames != actual->numFrames ) return IUS_FALSE;
    if ( strcmp(reference->pFilename, actual->pFilename) != 0 ) return IUS_FALSE;
    if ( iusHLFrameListCompare(reference->frameList, actual->frameList)  == IUS_FALSE ) return IUS_FALSE;
    if ( iusHLPatternListCompare(reference->patternList, actual->patternList)  == IUS_FALSE ) return IUS_FALSE;
    if ( iusHLPulseDictCompare(reference->pulseDict, actual->pulseDict)  == IUS_FALSE ) return IUS_FALSE;
    if ( iusHLSourceDictCompare(reference->pulseSourceDict, actual->pulseSourceDict)  == IUS_FALSE ) return IUS_FALSE;
	if ( iusHLReceiveChannelMapDictCompare(reference->receiveChannelMapDict, actual->receiveChannelMapDict) == IUS_FALSE) return IUS_FALSE;
    if ( iusHLTransmitApodizationDictCompare(reference->transmitApodizationDict, actual->transmitApodizationDict)  == IUS_FALSE ) return IUS_FALSE;
    if ( iusHLReceiveSettingsDictCompare(reference->receiveSettingsDict, actual->receiveSettingsDict)  == IUS_FALSE ) return IUS_FALSE;
    if ( iusHLTransducerCompare(reference->transducer, actual->transducer) == IUS_FALSE) return IUS_FALSE;
	if ( iusHLExperimentCompare(reference->experiment, actual->experiment) == IUS_FALSE) return IUS_FALSE;
    return IUS_TRUE;
}

// Getters
iufl_t iusHLInputFileGetFrameList
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


iupd_t iusHLInputFileGetPulseDict
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


iupal_t iusHLInputFileGetPatternList
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

iue_t iusHLInputFileGetExperiment
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


// Setters
int iusHLInputFileSetPulseDict
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

iurcmd_t iusHLInputFileGetReceiveChannelMapDict
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

iutad_t iusHLInputFileGetTransmitApodizationDict
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

iusd_t iusHLInputFileGetSourceDict
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

iursd_t iusHLInputFileGetReceiveSettingsDict
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

// Setters
int iusHLInputFileSetReceiveChannelMapDict
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

int iusHLInputFileSetTransmitApodizationDict
(
	iuif_t inputFile,
	iutad_t transmitApodizationDict
)
{
	int status = IUS_ERR_VALUE;

	if(inputFile != NULL)
	{
		inputFile->transmitApodizationDict = transmitApodizationDict;
		status = IUS_E_OK;
	}
	return status;
}

int iusHLInputFileSetReceiveSettingsDict
(
    iuif_t inputFile,
    iursd_t transmitApreceiveSettingsDict
)
{
    int status = IUS_ERR_VALUE;

    if(inputFile != NULL)
    {
        inputFile->receiveSettingsDict = transmitApreceiveSettingsDict;
        status = IUS_E_OK;
    }
    return status;
}

int iusHLInputFileSetPatternList
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

int iusHLInputFileSetExperiment
(
	iuif_t inputFile,
	iue_t experiment
)
{
	int status = IUS_ERR_VALUE;

	if (inputFile != NULL)
	{
		inputFile->experiment = experiment;
		status = IUS_E_OK;
	}
	return status;
}

int iusHLInputFileSetFrameList
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

int iusHLInputFileSetSourceDict
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
