
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
#include <include/iusExperimentPrivate.h>
#include <include/iusInputFilePrivate.h>
#include <include/iusPatternListDictPrivate.h>
#include <include/iusPulseDictPrivate.h>
#include <include/iusReceiveChannelMapDictPrivate.h>
#include <include/iusFrameList.h>
#include <include/iusSourceDict.h>
#include <include/iusReceiveSettings.h>
#include <include/iusTransducer.h>
#include <include/iusFrameListPrivate.h>
#include <include/iusSourceDictPrivate.h>
#include <include/iusReceiveSettingsDictPrivate.h>
#include <include/iusTransducerPrivate.h>
#include <include/iusHistoryNode.h>
#include <include/iusHistoryNodePrivate.h>
#include <include/iusTransmitApodizationDictPrivate.h>
#include <include/iusInputFileStructure.h>


struct IusInputFileInstance
{
    iufl_t frameList;
	iupald_t patternListDict;            /**< a dictionary of pattern lists              */
    iupd_t pulseDict;                    /**< a dictionary of pulses                      */
    iusd_t pulseSourceDict;              /**< a dictionary of sources                     */
	iurcmd_t receiveChannelMapDict;      /**< a dictionary of receiveChannelMaps          */
	iutad_t transmitApodizationDict;     /**< a dictionary of transmitApodizations        */
	iursd_t receiveSettingsDict;         /**< a dictionary of receiveSettings             */
	iut_t transducer;                    /**< The transducer description                  */
	iue_t experiment;                    /**< The description of the performed experiment */
    int numFrames;                       /**< The number of frames in the data */
    int IusVersion;                      /**< The version of input file format */


    //  state variables
    hid_t               handle;           /**< HDF5 file handle     */
    const char          *pFilename;       /**< the filename         */
    hid_t fileChunkConfig;                /**< file chunck handle   */
    hid_t rfDataset;                      /**< dataset handle       */
    int currentFrame;                     /**< current frame number */
    int currentPulse;                     /**< current pulse number */
}  ;



struct IusInputFile
{
    iuhn_t history;
};

iuifi_t iusInputFileInstanceCreate
(
    void
)
{
    iuifi_t instanceData = (iuifi_t) calloc(1, sizeof(IusInputFileInstance));
	if (instanceData == NULL)
	{
		return IUIFI_INVALID;
	}

	instanceData->numFrames = IUS_DEFAULT_NUM_FRAMES;
	instanceData->IusVersion = iusGetVersionMajor();
	instanceData->pFilename = "";
	instanceData->rfDataset = H5I_INVALID_HID;
	instanceData->fileChunkConfig = H5I_INVALID_HID;
	instanceData->frameList = IUFL_INVALID;
	instanceData->patternListDict = IUPALD_INVALID;
	instanceData->pulseDict = IUPD_INVALID;
    instanceData->pulseSourceDict = IUSD_INVALID;
	instanceData->receiveChannelMapDict = IURCMD_INVALID;
    instanceData->transmitApodizationDict = IUTAD_INVALID;
    instanceData->receiveSettingsDict = IURSD_INVALID;
    instanceData->transducer = IUT_INVALID;
	instanceData->experiment = IUE_INVALID;
    return instanceData;
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

	iuifi_t instanceData = iusInputFileInstanceCreate();
	if (instanceData == IUIFI_INVALID)
	{
		fprintf(stderr, "iusInputFileCreate: calloc of instance failed\n");
		return IUIF_INVALID;
	}

    instanceData->handle = H5Fcreate(pFilename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
	instanceData->pFilename = strdup(pFilename);
	if (instanceData->handle == H5I_INVALID_HID)
	{
		free((void *)instanceData);
		return IUIF_INVALID;
	}
	iuhn_t node = iusHistoryNodeCreate(IUS_INPUT_TYPE,0);
	iusHistoryNodeSetInstanceData(node,(void *)instanceData);
	return (iuif_t)node;
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


static iuifi_t inputFileInstanceLoad
(
    iuifi_t instance
)
{
    if (instance == NULL) return IUIFI_INVALID;

    instance->frameList = iusFrameListLoad(instance->handle);
    if (instance->frameList == IUFL_INVALID)
    {
        fprintf(stderr, "Warning from iusInputFileLoad: could not load framelist");
        return IUIFI_INVALID;
    }

    instance->patternListDict = iusPatternListDictLoad(instance->handle);
    if (instance->patternListDict == IUPALD_INVALID)
    {
        fprintf(stderr, "Warning from iusInputFileLoad: could not load patternlists  %s\n", instance->pFilename );
        return IUIFI_INVALID;
    }

    // Load instance data
    instance->pulseDict = iusPulseDictLoad(instance->handle);
    if (instance->pulseDict == IUPD_INVALID)
    {
        fprintf( stderr, "Warning from iusInputFileLoad: could not load pulses: %s\n", instance->pFilename );
        return IUIFI_INVALID;
    }

    // Load instance data
    // Todo: create group @here instead of in Load, see experiment
    instance->pulseSourceDict = iusSourceDictLoad(instance->handle);
    if (instance->pulseSourceDict == IUSD_INVALID)
    {
        fprintf( stderr, "Warning from iusInputFileLoad: could not load pulse sources: %s\n", instance->pFilename );
        return IUIFI_INVALID;
    }

    // Load instance data
    // Todo: create group @here instead of in Load, see experiment
    instance->receiveChannelMapDict = iusReceiveChannelMapDictLoad(instance->handle);
    if (instance->receiveChannelMapDict == IURCMD_INVALID)
    {
        fprintf(stderr, "Warning from iusInputFileLoad: could not load receiveChannelMap: %s\n", instance->pFilename);
        return IUIFI_INVALID;
    }

    // Load instance data
    // Todo: create group @here instead of in Load, see experiment
    instance->transmitApodizationDict = iusTransmitApodizationDictLoad(instance->handle);
    if (instance->transmitApodizationDict == IUTAD_INVALID)
    {
        fprintf(stderr, "Warning from iusInputFileLoad: could not load transmitApodizationDict: %s\n", instance->pFilename);
        return IUIFI_INVALID;
    }

    // Load instance data
    // Todo: create group @here instead of in Load, see experiment
    instance->receiveSettingsDict = iusReceiveSettingsDictLoad(instance->handle);
    if (instance->receiveSettingsDict == IURSD_INVALID)
    {
        fprintf(stderr, "Warning from iusInputFileLoad: could not load receiveSettingsDict: %s\n", instance->pFilename);
        return IUIFI_INVALID;
    }


    instance->experiment = iusExperimentLoad(instance->handle);
    if (instance->experiment == IUE_INVALID)
    {
        fprintf(stderr, "Warning from iusInputFileLoad: could not load experiment: %s\n", instance->pFilename);
        return IUIFI_INVALID;
    }

    // Load instance data
    // Todo: create group @here instead of in Load, see experiment
    instance->transducer = iusTransducerLoad(instance->handle);
    if (instance->transducer == IUT_INVALID)
    {
        fprintf(stderr, "Warning from iusInputFileLoad: could not load transducer: %s\n", instance->pFilename);
        return IUIFI_INVALID;
    }

    int status = iusHdf5ReadInt( instance->handle, IUS_INPUTFILE_PATH_IUSVERSION, &(instance->IusVersion));
    if( status != IUS_E_OK )
    {
        fprintf(stderr, "Warning from iusInputFileLoad: could not load IusVersion: %s\n", instance->pFilename);
        return IUIFI_INVALID;
    }

    status = iusHdf5ReadInt( instance->handle, IUS_INPUTFILE_PATH_NUMFRAMES, &(instance->numFrames));
    if( status != IUS_E_OK )
    {
        fprintf(stderr, "Warning from iusInputFileLoad: could not load numFrames: %s\n", instance->pFilename);
        return IUIFI_INVALID;
    }


    return instance;
}

void *iusInputFileInstanceLoad
(
    hid_t handle
)
{
    iuifi_t instance = iusInputFileInstanceCreate();
    instance->handle = handle;
    instance = inputFileInstanceLoad(instance);
    return (void *)instance;
}


iuhn_t iusInputFileLoadNode
(
    hid_t handle
)
{
    iuhn_t node = iusHistoryNodeCreate(IUS_INPUT_TYPE, 0);
    iuifi_t instance = iusInputFileInstanceCreate();
    instance->handle = handle;
    instance = inputFileInstanceLoad(instance);
    iusHistoryNodeSetInstanceData(node,instance);
    return node;
}

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

    // open  Hdf5 file using default properties.
    hid_t handle = H5Fopen( pFilename, H5F_ACC_RDONLY, H5P_DEFAULT );
    if (handle <=0)
    {
        fprintf( stderr, "iusInputFileLoad: could not create file: %s\n", pFilename );
        return IUIF_INVALID;
    }

    // Load instance data
    // Todo: create group @here instead of in Load, see experiment
    iuif_t inputFile = (iuif_t) iusHistoryNodeLoad(handle);
    iuifi_t instance = iusHistoryNodeGetInstanceData((iuhn_t)inputFile);
    instance->pFilename = strdup(pFilename);
    return inputFile;
}

int iusInputFileSaveInstance
(
    hid_t handle,
    iuifi_t instanceData
)
{
    herr_t status=0;
    status |= iusFrameListSave(instanceData->frameList, handle);
    status |= iusPatternListDictSave(instanceData->patternListDict, handle);
    status |= iusPulseDictSave(instanceData->pulseDict, handle);
    status |= iusSourceDictSave(instanceData->pulseSourceDict, handle);
    status |= iusReceiveChannelMapDictSave(instanceData->receiveChannelMapDict, handle);
    status |= iusTransmitApodizationDictSave(instanceData->transmitApodizationDict, handle);
    status |= iusReceiveSettingsDictSave(instanceData->receiveSettingsDict, handle);

    status |= iusExperimentSave(instanceData->experiment, handle);
    status |= iusTransducerSave(instanceData->transducer, instanceData->handle);
    status |= iusHdf5WriteInt( instanceData->handle, IUS_INPUTFILE_PATH_IUSVERSION, &(instanceData->IusVersion), 1);
    status |= iusHdf5WriteInt( instanceData->handle, IUS_INPUTFILE_PATH_NUMFRAMES, &(instanceData->numFrames), 1);
    return status;
}


int iusInputFileSave
(
    iuif_t fileHandle
)
{
    herr_t status=0;
    if( fileHandle == NULL ) return IUS_ERR_VALUE;

    iuifi_t instance = iusHistoryNodeGetInstanceData((iuhn_t)fileHandle);
    status |= iusHistoryNodeSave(fileHandle, instance->handle);
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

    iuifi_t instance = iusHistoryNodeGetInstanceData((iuhn_t)fileHandle);
    if( instance->fileChunkConfig != H5I_INVALID_HID )
    {
        status |= H5Pclose(instance->fileChunkConfig);
    }

    if( instance->rfDataset != H5I_INVALID_HID )
    {
        status |= H5Dclose(instance->rfDataset);
    }

    // Terminate access to the file.
    if( instance->handle )
    {
        status |= H5Fclose(instance->handle);
    }

    return status;
}

static int iusInputFileCompareInstance
(
    iuifi_t reference,
    iuifi_t actual
)
{
    if ( reference == actual ) return IUS_TRUE;
    if ( reference == NULL || actual == NULL ) return IUS_FALSE;
    if ( reference->IusVersion != actual->IusVersion ) return IUS_FALSE;
    if ( reference->numFrames != actual->numFrames ) return IUS_FALSE;
    if ( strcmp(reference->pFilename, actual->pFilename) != 0 ) return IUS_FALSE;
    if ( iusFrameListCompare(reference->frameList, actual->frameList)  == IUS_FALSE ) return IUS_FALSE;
    if ( iusPatternListDictCompare(reference->patternListDict, actual->patternListDict)  == IUS_FALSE ) return IUS_FALSE;
    if ( iusPulseDictCompare(reference->pulseDict, actual->pulseDict)  == IUS_FALSE ) return IUS_FALSE;
    if ( iusSourceDictCompare(reference->pulseSourceDict, actual->pulseSourceDict)  == IUS_FALSE ) return IUS_FALSE;
    if ( iusReceiveChannelMapDictCompare(reference->receiveChannelMapDict, actual->receiveChannelMapDict) == IUS_FALSE) return IUS_FALSE;
    if ( iusTransmitApodizationDictCompare(reference->transmitApodizationDict, actual->transmitApodizationDict)  == IUS_FALSE ) return IUS_FALSE;
    if ( iusReceiveSettingsDictCompare(reference->receiveSettingsDict, actual->receiveSettingsDict)  == IUS_FALSE ) return IUS_FALSE;
    if ( iusTransducerCompare(reference->transducer, actual->transducer) == IUS_FALSE) return IUS_FALSE;
    if ( iusExperimentCompare(reference->experiment, actual->experiment) == IUS_FALSE) return IUS_FALSE;
    return IUS_TRUE;
}

int iusInputFileCompare
(
    iuif_t reference,
    iuif_t actual
)
{
    if ( reference == actual ) return IUS_TRUE;
    if ( reference == NULL || actual == NULL ) return IUS_FALSE;
    if ( iusHistoryNodeCompareWithId(reference, actual)  == IUS_FALSE ) return IUS_FALSE;

    iuifi_t refInstance = iusHistoryNodeGetInstanceData((iuhn_t)reference);
    iuifi_t actInstance = iusHistoryNodeGetInstanceData((iuhn_t)actual);
    return iusInputFileCompareInstance(refInstance,actInstance);
}

// Getters
iufl_t iusInputFileGetFrameList
(
    iuif_t iusInputFile
)
{
    if ( iusInputFile != NULL )
    {
        iuifi_t instance = iusHistoryNodeGetInstanceData((iuhn_t)iusInputFile);
        return instance->frameList;
    }
    return IUFL_INVALID;
}

iupald_t iusInputFileGetPatternListDict
(
    iuif_t iusInputFile
)
{
    if ( iusInputFile != NULL )
    {
        iuifi_t instance = iusHistoryNodeGetInstanceData((iuhn_t)iusInputFile);
        return instance->patternListDict;
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
        iuifi_t instance = iusHistoryNodeGetInstanceData((iuhn_t)iusInputFile);
        return instance->pulseDict;
    }
    return NULL;
}

iusd_t iusInputFileGetSourceDict
(
    iuif_t iusInputFile
)
{
    if (iusInputFile != NULL)
    {
        iuifi_t instance = iusHistoryNodeGetInstanceData((iuhn_t)iusInputFile);
        return instance->pulseSourceDict;
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
        iuifi_t instance = iusHistoryNodeGetInstanceData((iuhn_t)iusInputFile);
        return instance->receiveChannelMapDict;
    }
    return NULL;
}

iutad_t iusInputFileGetTransmitApodizationDict
(
    iuif_t iusInputFile
)
{
    if (iusInputFile != NULL)
    {
        iuifi_t instance = iusHistoryNodeGetInstanceData((iuhn_t)iusInputFile);
        return instance->transmitApodizationDict;
    }
    return NULL;
}

iursd_t iusInputFileGetReceiveSettingsDict
(
    iuif_t iusInputFile
)
{
    if (iusInputFile != NULL)
    {
        iuifi_t instance = iusHistoryNodeGetInstanceData((iuhn_t)iusInputFile);
        return instance->receiveSettingsDict;
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
        iuifi_t instance = iusHistoryNodeGetInstanceData((iuhn_t)iusInputFile);
		return instance->experiment;
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
        iuifi_t instance = iusHistoryNodeGetInstanceData((iuhn_t)iusInputFile);
        return instance->transducer;
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
        iuifi_t instance = iusHistoryNodeGetInstanceData((iuhn_t)inputFile);
        instance->frameList = frameList;
        status = IUS_E_OK;
    }
    return status;
}

int iusInputFileSetPatternListDict
(
    iuif_t inputFile,
    iupald_t patternListDict
)
{
    int status = IUS_ERR_VALUE;

    if(inputFile != NULL)
    {
        iuifi_t instance = iusHistoryNodeGetInstanceData((iuhn_t)inputFile);
        instance->patternListDict = patternListDict;
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
        iuifi_t instance = iusHistoryNodeGetInstanceData((iuhn_t)inputFile);
        instance->pulseDict = pulseDict;
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
        iuifi_t instance = iusHistoryNodeGetInstanceData((iuhn_t)inputFile);
        instance->pulseSourceDict = sourceDict;
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
        iuifi_t instance = iusHistoryNodeGetInstanceData((iuhn_t)inputFile);
        instance->receiveChannelMapDict = receiveChannelMapDict;
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
        iuifi_t instance = iusHistoryNodeGetInstanceData((iuhn_t)inputFile);
        instance->transmitApodizationDict = transmitApodizationDict;
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
        iuifi_t instance = iusHistoryNodeGetInstanceData((iuhn_t)inputFile);
        instance->receiveSettingsDict = transmitApreceiveSettingsDict;
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
        iuifi_t instance = iusHistoryNodeGetInstanceData((iuhn_t)inputFile);
        instance->experiment = experiment;
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
        iuifi_t instance = iusHistoryNodeGetInstanceData((iuhn_t)inputFile);
        instance->transducer = transducer;
        status = IUS_E_OK;
    }
    return status;
}
