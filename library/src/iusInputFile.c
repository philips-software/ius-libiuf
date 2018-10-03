
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
#include <include/iusInputFileImp.h>
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



struct IusInputFileInstance
{
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
    hid_t               handle;                         /**< file handle */
    const char          *pFilename;
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
	instanceData->frameList = IUFL_INVALID;
	instanceData->patternList = IUPAL_INVALID;
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

    instance->frameList = iusFrameListLoad(instance->handle, FRAME_LIST_PATH);
    if (instance->frameList == IUFL_INVALID)
    {
        fprintf(stderr, "Warning from iusInputFileLoad: could not load framelist");
        return IUIFI_INVALID;
    }

    // Todo: create group @here instead of in Load, see experiment
    instance->patternList = iusPatternListLoad(instance->handle, PATTERN_LIST_PATH);
    if (instance->patternList == IUPAL_INVALID)
    {
        fprintf(stderr, "Warning from iusInputFileLoad: could not load patterns: %s\n", instance->pFilename );
        return IUIFI_INVALID;
    }

    // Load instance data
    // Todo: create group @here instead of in Load, see experiment
    instance->pulseDict = iusPulseDictLoad(instance->handle, PULSE_DICT_PATH);
    if (instance->pulseDict == IUPD_INVALID)
    {
        fprintf( stderr, "Warning from iusInputFileLoad: could not load pulses: %s\n", instance->pFilename );
        return IUIFI_INVALID;
    }

    // Load instance data
    // Todo: create group @here instead of in Load, see experiment
    instance->pulseSourceDict = iusSourceDictLoad(instance->handle, PULSE_SOURCE_DICT_PATH);
    if (instance->pulseSourceDict == IUSD_INVALID)
    {
        fprintf( stderr, "Warning from iusInputFileLoad: could not load pulse sources: %s\n", instance->pFilename );
        return IUIFI_INVALID;
    }

    // Load instance data
    // Todo: create group @here instead of in Load, see experiment
    instance->receiveChannelMapDict = iusReceiveChannelMapDictLoad(instance->handle, RECEIVE_CHANNEL_MAP_PATH);
    if (instance->receiveChannelMapDict == IURCMD_INVALID)
    {
        fprintf(stderr, "Warning from iusInputFileLoad: could not load receiveChannelMap: %s\n", instance->pFilename);
        return IUIFI_INVALID;
    }

    // Load instance data
    // Todo: create group @here instead of in Load, see experiment
    instance->transmitApodizationDict = iusTransmitApodizationDictLoad(instance->handle, TRANSMIT_APODIZATION_DICT_PATH);
    if (instance->transmitApodizationDict == IUTAD_INVALID)
    {
        fprintf(stderr, "Warning from iusInputFileLoad: could not load transmitApodizationDict: %s\n", instance->pFilename);
        return IUIFI_INVALID;
    }

    // Load instance data
    // Todo: create group @here instead of in Load, see experiment
    instance->receiveSettingsDict = iusReceiveSettingsDictLoad(instance->handle, RECEIVE_SETTINGS_DICT_PATH);
    if (instance->receiveSettingsDict == IURSD_INVALID)
    {
        fprintf(stderr, "Warning from iusInputFileLoad: could not load receiveSettingsDict: %s\n", instance->pFilename);
        return IUIFI_INVALID;
    }

    hid_t group_id = H5Gopen(instance->handle, EXPERIMENT_PATH, H5P_DEFAULT);
    instance->experiment = iusExperimentLoad(group_id);
    H5Gclose(group_id);
    if (instance->experiment == IUE_INVALID)
    {
        fprintf(stderr, "Warning from iusInputFileLoad: could not load experiment: %s\n", instance->pFilename);
        return IUIFI_INVALID;
    }

    // Load instance data
    // Todo: create group @here instead of in Load, see experiment
    instance->transducer = iusTransducerLoad(instance->handle, TRANSDUCER_PATH);
    if (instance->transducer == IUT_INVALID)
    {
        fprintf(stderr, "Warning from iusInputFileLoad: could not load transducer: %s\n", instance->pFilename);
        return IUIFI_INVALID;
    }

    int status = iusHdf5ReadInt( instance->handle, IUSVERSION_PATH, &(instance->IusVersion));
    if( status != IUS_E_OK )
    {
        fprintf(stderr, "Warning from iusInputFileLoad: could not load IusVersion: %s\n", instance->pFilename);
        return IUIFI_INVALID;
    }

    status = iusHdf5ReadInt( instance->handle, NUMFRAMES_PATH, &(instance->numFrames));
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
    status |= iusFrameListSave(instanceData->frameList, FRAME_LIST_PATH, handle);
    status |= iusPatternListSave(instanceData->patternList, PATTERN_LIST_PATH, handle);
    status |= iusPulseDictSave(instanceData->pulseDict, PULSE_DICT_PATH, handle);
    status |= iusSourceDictSave(instanceData->pulseSourceDict, PULSE_SOURCE_DICT_PATH, handle);
    status |= iusReceiveChannelMapDictSave(instanceData->receiveChannelMapDict, RECEIVE_CHANNEL_MAP_PATH, handle);
    status |= iusTransmitApodizationDictSave(instanceData->transmitApodizationDict, TRANSMIT_APODIZATION_DICT_PATH, handle);
    status |= iusReceiveSettingsDictSave(instanceData->receiveSettingsDict, RECEIVE_SETTINGS_DICT_PATH, handle);

    hid_t group_id = H5Gcreate(instanceData->handle, EXPERIMENT_PATH, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    status |= iusExperimentSave(instanceData->experiment, group_id);
    status |= H5Gclose(group_id);
    status |= iusTransducerSave(instanceData->transducer, TRANSDUCER_PATH, instanceData->handle);
    status |= iusHdf5WriteInt( instanceData->handle, IUSVERSION_PATH, &(instanceData->IusVersion), 1);
    status |= iusHdf5WriteInt( instanceData->handle, NUMFRAMES_PATH, &(instanceData->numFrames), 1);
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

iupal_t iusInputFileGetPatternList
(
    iuif_t iusInputFile
)
{
    if ( iusInputFile != NULL )
    {
        iuifi_t instance = iusHistoryNodeGetInstanceData((iuhn_t)iusInputFile);
        return instance->patternList;
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

int iusInputFileSetPatternList
(
    iuif_t inputFile,
    iupal_t paternList
)
{
    int status = IUS_ERR_VALUE;

    if(inputFile != NULL)
    {
        iuifi_t instance = iusHistoryNodeGetInstanceData((iuhn_t)inputFile);
        instance->patternList = paternList;
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


int iusInputFileSaveChannel
(
    iuif_t inputFile,
    char *label,
    const float *samples
)
{
//    hid_t memspace;
//    hid_t dataspace;
//    hsize_t offset[4];
//    hsize_t count[4];
//    hsize_t memdim[2];
//    herr_t  status;
    IUS_UNUSED(samples);
    IUS_UNUSED(inputFile);
    IUS_UNUSED(label);
    // get number of channels
//    iuifi_t instance = iusHistoryNodeGetInstanceData((iuhn_t)inputFile);
//    iurcm_t channelMap = iusReceiveChannelMapDictGet(instance->receiveChannelMapDict,label);
//    int numChannels = iusReceiveChannelMapGetNumChannels(channelMap);
//
//    iurs_t receiveSettings = iusReceiveSettingsDictGet(instance->receiveSettingsDict,label)
//    memdim[0] = numChannels;
//    memdim[1] = pInst->pDrivingScheme->numSamplesPerLine;

#if 0
    memdim[0] = pInst->pTransducer->numElements;
    memdim[1] = pInst->pDrivingScheme->numSamplesPerLine;
    memspace = H5Screate_simple(2, memdim, NULL);

    if (pInst->numFrames == 1)
    {
        offset[0] = 0;
        offset[1] = 0;
        offset[2] = pInst->currentPulse;

        count[0] = pInst->pTransducer->numElements;
        count[1] = pInst->pDrivingScheme->numSamplesPerLine;
        count[2] = 1;
    }
    else
    {
        offset[0] = 0;
        offset[1] = 0;
        offset[2] = pInst->currentPulse;
        offset[3] = pInst->currentFrame;

        count[0] = memdim[0];
        count[1] = memdim[1];
        count[2] = 1;
        count[3] = 1;
    }
    dataspace = H5Dget_space(pInst->rfDataset);
    status = H5Sselect_hyperslab(dataspace, H5S_SELECT_SET, offset, NULL, count, NULL);

    /*
     *   Write the data to the dataset.
     */
    status |= H5Dwrite(pInst->rfDataset, H5T_NATIVE_FLOAT, memspace, dataspace, H5P_DEFAULT, pFrame);
    /*
     *   Close and release memspace but not (file)dataspace
     */
    status |= H5Sclose(memspace);
    status |= H5Sclose(dataspace);

    /*------------------------------------------------------------------------*/
    /* increment file read pointers                                           */
    /*------------------------------------------------------------------------*/
    pInst->currentPulse++;
    if (pInst->currentPulse == pInst->pDrivingScheme->numTransmitPulses)
    {
        pInst->currentPulse = 0;
        pInst->currentFrame++;
    }
#endif
    return IUS_E_OK;
}

#if 0
int iusInputFileReadNextPulse
(
    iuif_t file,
    float *const *const pRFout
)
{
    hsize_t i, j;
    hsize_t offset[4];
    hsize_t count[4];
    hid_t   memspace;
    hid_t   dataspace;
    hsize_t memdim[2];
    herr_t  status;

    if (file == NULL || pRFout == NULL)
    {
        fprintf(stderr, "iusInputRedNextPulse: input arguments can not be NULL\n");
        return -1;
    }
    iuifi_t instance = iusHistoryNodeGetInstanceData((iuhn_t)file);
    // TODO: parametrize label "bmode"
    iurs_t receiveSettings = iusReceiveSettingsDictGet(instance->receiveSettingsDict,"bmode");
    int numSamplesPerLine = iusReceiveSettingsGetNumSamplesPerLine(receiveSettings);
    int numElements = iusTransducerGetNumElements(instance->transducer);
    float *pPage = (float *)calloc(numSamplesPerLine * numElements, sizeof(float)) ;
    if (pPage == NULL)
    {
        fprintf(stderr, "iusInputRedNextPulse: Memory allocation error\n");
        return -1;
    }

    offset[0] = 0;
    offset[1] = 0;
    offset[2] = instance->currentPulse;
    offset[3] = instance->currentFrame;

    count[1] = numSamplesPerLine;
    count[0] = numElements;
    count[2] = 1;
    count[3] = 1;

    memdim[0] = numElements;
    memdim[1] = numSamplesPerLine;
    memspace = H5Screate_simple(2, memdim, NULL);
    dataspace = H5Dget_space(instance->rfDataset);
    status = H5Sselect_hyperslab(dataspace, H5S_SELECT_SET, offset, NULL, count, NULL);

    status |= H5Dread(instance->rfDataset, H5T_NATIVE_FLOAT, memspace, dataspace, H5P_DEFAULT, pPage);
    /* read an 1D buffer, copy that to 2D array -- might not be needed anymore */
    for (i=0; i<memdim[0] ; i++) // numElements (# of rows rows)
    {
        for (j=0; j<memdim[1]; j++) // numSamples (on each row)
        {
            pRFout[i][j] = pPage[j+i*memdim[1]];
        }
    }
    free(pPage);

    instance->currentPulse++;
    if (instance->currentPulse == instance->pIusInput->pDrivingScheme->numTransmitPulses)
    {
        instance->currentPulse = 0;
        instance->currentFrame++;
    }
    status |= H5Sclose(memspace);
    status |= H5Sclose(dataspace);

    return (int)status;

}


int iusInputFileReadNextFrame
(
    iuif_t file,
    float *const *const *const pRFout
)
{
    int i;
    iuifi_t instance = iusHistoryNodeGetInstanceData((iuhn_t)file);
    int numTransmitPulses = iusPulseDictGetSize(instance->pulseDict);
    for (i=0; i<numTransmitPulses; i++)
    {
        iusInputFileReadNextPulse(file, pRFout[i]);
    }
    return 0;
}


//
// TODO: Add label support?
// Frame should be read from specifix datapath right
int iusInputFileReadFrame
(
    iuif_t file,
    float *const *const *const pRFout,
    int frameNum
)
{
    iuifi_t instance = iusHistoryNodeGetInstanceData((iuhn_t)file);
    instance->currentFrame=frameNum;
    return iusInputFileReadNextFrame(instance, pRFout);
}


int iusInputFileWriteNextPulse
(
IusInputFileInstance * pInst,
const float *pFrame
)
{
    hid_t memspace;
    hid_t dataspace;
    hsize_t offset[4];
    hsize_t count[4];
    hsize_t memdim[2];
    herr_t  status;

    memdim[0] = pInst->pIusInput->pTransducer->numElements;
    memdim[1] = pInst->pIusInput->pDrivingScheme->numSamplesPerLine;
    memspace = H5Screate_simple(2, memdim, NULL);

    if (pInst->pIusInput->numFrames == 1)
    {
        offset[0] = 0;
        offset[1] = 0;
        offset[2] = pInst->currentPulse;

        count[0] = pInst->pIusInput->pTransducer->numElements;
        count[1] = pInst->pIusInput->pDrivingScheme->numSamplesPerLine;
        count[2] = 1;
    }
    else
    {
        offset[0] = 0;
        offset[1] = 0;
        offset[2] = pInst->currentPulse;
        offset[3] = pInst->currentFrame;

        count[0] = memdim[0];
        count[1] = memdim[1];
        count[2] = 1;
        count[3] = 1;
    }
    dataspace = H5Dget_space(pInst->rfDataset);
    status = H5Sselect_hyperslab(dataspace, H5S_SELECT_SET, offset, NULL, count, NULL);

    /*
     *   Write the data to the dataset.
    */
    status |= H5Dwrite(pInst->rfDataset, H5T_NATIVE_FLOAT, memspace, dataspace, H5P_DEFAULT, pFrame);
    /*
     *   Close and release memspace but not (file)dataspace
    */
    status |= H5Sclose(memspace);
    status |= H5Sclose(dataspace);

    /*------------------------------------------------------------------------*/
    /* increment file read pointers                                           */
    /*------------------------------------------------------------------------*/
    pInst->currentPulse++;
    if (pInst->currentPulse == pInst->pIusInput->pDrivingScheme->numTransmitPulses)
    {
        pInst->currentPulse = 0;
        pInst->currentFrame++;
    }

    return status;

}

int iusInputFileReadFrameDepthRange
(
IusInputFileInstance * pInst,
const IusRange *const pDepthRange,
float *const *const *const pPage,
int frameNum
)
{
    pInst->currentFrame = frameNum;
    return iusInputFileReadNextFrameDepthRange(pInst, pDepthRange, pPage);

}

int iusInputFileReadNextFrameDepthRange
(
IusInputFileInstance * pInst,
const IusRange *const pDepthRange,
float *const *const *const pPage
)
{
    int i;
    //IusPage wave;
    for (i = 0; i<pInst->pIusInput->pDrivingScheme->numTransmitPulses; i++)
    {
        iusInputFileReadNextPulseDepthRange(pInst, pDepthRange, pPage[i]);
    }
    return 0;
}

int iusInputFileReadNextPulseDepthRange
(
IusInputFileInstance * pInst,
const IusRange *const pDepthRange,
float *const *const ppPageOut
)
{
    hsize_t i, j;
    hsize_t offset[4];
    hsize_t count[4];

    hid_t   memspace;
    hid_t   dataspace;
    hsize_t memdim[2];
    herr_t  status;

    if (pInst == NULL || ppPageOut == NULL)
    {
        fprintf(stderr, "iusInputRedNextPulse: input arguments can not be NULL\n");
        return -1;
    }
    float *pPage = (float *)calloc(pInst->pIusInput->pDrivingScheme->numSamplesPerLine *
    pInst->pIusInput->pTransducer->numElements, sizeof(float));
    if (pPage == NULL)
    {
        fprintf(stderr, "iusInputRedNextPulse: Memory allocation error\n");
        return -1;
    }

    offset[0] = 0;
    offset[1] = pDepthRange->startIndex;
    offset[2] = pInst->currentPulse;
    offset[3] = pInst->currentFrame;

    count[0] = pInst->pIusInput->pTransducer->numElements;
    count[1] = pDepthRange->numSamples;
    count[2] = 1;
    count[3] = 1;

    memdim[0] = pInst->pIusInput->pTransducer->numElements;
    memdim[1] = pDepthRange->numSamples;
    memspace = H5Screate_simple(2, memdim, NULL);
    dataspace = H5Dget_space(pInst->rfDataset);
    status = H5Sselect_hyperslab(dataspace, H5S_SELECT_SET, offset, NULL, count, NULL);

    status |= H5Dread(pInst->rfDataset, H5T_NATIVE_FLOAT, memspace, dataspace, H5P_DEFAULT, pPage);

    /* read an 1D buffer, copy that to 2D array -- might not be needed anymore */
    for (i = 0; i<memdim[0]; i++) // numElements (# of rows rows)
    {
        for (j = 0; j<memdim[1]; j++) // numSamples (on each row)
        {
            ppPageOut[i][j+offset[1]] = pPage[j + i*memdim[1]];
        }
    }
    free(pPage);

    pInst->currentPulse++;
    if (pInst->currentPulse == pInst->pIusInput->pDrivingScheme->numTransmitPulses)
    {
        pInst->currentPulse = 0;
        pInst->currentFrame++;
    }
    status |= H5Sclose(memspace);
    status |= H5Sclose(dataspace);

    return status;
}
#endif

