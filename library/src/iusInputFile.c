
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
#include <include/iusDataStreamDictPrivate.h>

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
    iudsd_t             dataStreamDict;   /**< Contains dataset administration */
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
	instanceData->patternListDict = IUPALD_INVALID;
	instanceData->pulseDict = IUPD_INVALID;
    instanceData->pulseSourceDict = IUSD_INVALID;
	instanceData->receiveChannelMapDict = IURCMD_INVALID;
    instanceData->transmitApodizationDict = IUTAD_INVALID;
    instanceData->receiveSettingsDict = IURSD_INVALID;
    instanceData->transducer = IUT_INVALID;
	instanceData->experiment = IUE_INVALID;
	instanceData->dataStreamDict = iusDataStreamDictCreate();
    return instanceData;
}

// ADT
iuif_t iusInputFileCreate
(
    const char *pFilename,
    int numFrames
)
{
    if (numFrames <= 0)
        return IUIF_INVALID;

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
	instanceData->numFrames = numFrames;
	if (instanceData->handle == H5I_INVALID_HID)
	{
		free((void *)instanceData);
		return IUIF_INVALID;
	}
	iuhn_t node = iusHistoryNodeCreate(IUS_INPUT_TYPE,0);
	iusHistoryNodeSetInstanceData(node,(void *)instanceData);
	return (iuif_t)node;
}

int iusInputFileGetNumChannels
(
    iuif_t iusInputFile,
    char *label
)
{
    if (iusInputFile == NULL) return -1;
    iurcmd_t receiveChannelMapDict = iusInputFileGetReceiveChannelMapDict(iusInputFile);
    if (receiveChannelMapDict == IURCMD_INVALID) return -1;
    iurcm_t receiveChannelMap = iusReceiveChannelMapDictGet(receiveChannelMapDict,label);
    if (receiveChannelMap == IURCM_INVALID) return -1;
    return iusReceiveChannelMapGetNumChannels(receiveChannelMap);
}

int iusInputFileGetNumFrames
(
    iuif_t inputFile
)
{
    if (inputFile == NULL) return -1;
    iuifi_t instance = iusHistoryNodeGetInstanceData((iuhn_t)inputFile);
    return instance->numFrames;
}


int iusInputFileSetNumFrames
(
    iuif_t inputFile,
    int numFrames
)
{
    if (inputFile == NULL) return -1;
    iuifi_t instance = iusHistoryNodeGetInstanceData((iuhn_t)inputFile);
    instance->numFrames = numFrames;
    return IUS_E_OK;
}


int iusInputFileGetSamplesPerLine
(
    iuif_t iusInputFile,
    char *label
)
{
    if (iusInputFile == NULL) return -1;
    iursd_t receiveSettingsDict = iusInputFileGetReceiveSettingsDict(iusInputFile);
    if (receiveSettingsDict == IURSD_INVALID) return -1;
    iurs_t receiveSettings = iusReceiveSettingsDictGet(receiveSettingsDict,label);
    if (receiveSettings == IURS_INVALID) return -1;
    return iusReceiveSettingsGetNumSamplesPerLine(receiveSettings);
}

int iusInputFileGetNumResponses
(
    iuif_t iusInputFile,
    char *label
)
{
    if (iusInputFile == NULL) return -1;
    iupald_t listDict = iusInputFileGetPatternListDict(iusInputFile);
    if (listDict == IUPALD_INVALID) return -1;
    iupal_t patternList = iusPatternListDictGet(listDict,label);
    if (patternList == IUPAL_INVALID) return -1;
    return iusPatternListGetSize(patternList);
}

iud_t iusInputFileFrameCreate
(
    iuif_t iusInputFile,
    char *label
)
{
    if(iusInputFile == NULL) return IUD_INVALID;
    // calculate frame size:
    // numchannels * numresponses * numsamples = [x*y*z]
    int numChannels = iusInputFileGetNumChannels(iusInputFile,label);
    int numResponses = iusInputFileGetNumResponses(iusInputFile,label);
    int numSamples = iusInputFileGetSamplesPerLine(iusInputFile,label);
    int frameSize = numChannels * numSamples * numResponses;
    return iusDataCreate(frameSize);
}

iud_t iusInputFileResponseCreate
(
    iuif_t iusInputFile,
    char *label
)
{
    if(iusInputFile == NULL) return IUD_INVALID;

    int numChannels = iusInputFileGetNumChannels(iusInputFile,label);
    int numSamples = iusInputFileGetSamplesPerLine(iusInputFile,label);
    int responseSize = numChannels * numSamples ;
    return iusDataCreate(responseSize);
}

iud_t iusInputFileChannelCreate
(
    iuif_t iusInputFile,
    char *label
)
{
    if(iusInputFile == NULL) return IUD_INVALID;

    int numSamples = iusInputFileGetSamplesPerLine(iusInputFile,label);
    return iusDataCreate(numSamples);
}


int iusInputFileDelete
(
    iuif_t iusInputFile
)
{
    int status = IUS_ERR_VALUE;
    if(iusInputFile != NULL)
    {
        iuifi_t instance = iusHistoryNodeGetInstanceData((iuhn_t)iusInputFile);
        iusDataStreamDictDelete(instance->dataStreamDict);
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
        fprintf(stderr, "Warning from iusInputFileNodeLoad: could not load framelist");
        return IUIFI_INVALID;
    }

    instance->patternListDict = iusPatternListDictLoad(instance->handle);
    if (instance->patternListDict == IUPALD_INVALID)
    {
        fprintf(stderr, "Warning from iusInputFileNodeLoad: could not load patternlists  %s\n", instance->pFilename );
        return IUIFI_INVALID;
    }

    instance->pulseDict = iusPulseDictLoad(instance->handle);
    if (instance->pulseDict == IUPD_INVALID)
    {
        fprintf( stderr, "Warning from iusInputFileNodeLoad: could not load pulses: %s\n", instance->pFilename );
        return IUIFI_INVALID;
    }

    instance->pulseSourceDict = iusSourceDictLoad(instance->handle);
    if (instance->pulseSourceDict == IUSD_INVALID)
    {
        fprintf( stderr, "Warning from iusInputFileNodeLoad: could not load pulse sources: %s\n", instance->pFilename );
        return IUIFI_INVALID;
    }

    instance->receiveChannelMapDict = iusReceiveChannelMapDictLoad(instance->handle);
    if (instance->receiveChannelMapDict == IURCMD_INVALID)
    {
        fprintf(stderr, "Warning from iusInputFileNodeLoad: could not load receiveChannelMap: %s\n", instance->pFilename);
        return IUIFI_INVALID;
    }

    instance->transmitApodizationDict = iusTransmitApodizationDictLoad(instance->handle);
    if (instance->transmitApodizationDict == IUTAD_INVALID)
    {
        fprintf(stderr, "Warning from iusInputFileNodeLoad: could not load transmitApodizationDict: %s\n", instance->pFilename);
        return IUIFI_INVALID;
    }

    instance->receiveSettingsDict = iusReceiveSettingsDictLoad(instance->handle);
    if (instance->receiveSettingsDict == IURSD_INVALID)
    {
        fprintf(stderr, "Warning from iusInputFileNodeLoad: could not load receiveSettingsDict: %s\n", instance->pFilename);
        return IUIFI_INVALID;
    }

    instance->experiment = iusExperimentLoad(instance->handle);
    if (instance->experiment == IUE_INVALID)
    {
        fprintf(stderr, "Warning from iusInputFileNodeLoad: could not load experiment: %s\n", instance->pFilename);
        return IUIFI_INVALID;
    }

    instance->transducer = iusTransducerLoad(instance->handle);
    if (instance->transducer == IUT_INVALID)
    {
        fprintf(stderr, "Warning from iusInputFileNodeLoad: could not load transducer: %s\n", instance->pFilename);
        return IUIFI_INVALID;
    }

    int status = iusHdf5ReadInt( instance->handle, IUS_INPUTFILE_PATH_IUSVERSION, &(instance->IusVersion));
    if( status != IUS_E_OK )
    {
        fprintf(stderr, "Warning from iusInputFileNodeLoad: could not load IusVersion: %s\n", instance->pFilename);
        return IUIFI_INVALID;
    }

    status = iusHdf5ReadInt( instance->handle, IUS_INPUTFILE_PATH_NUMFRAMES, &(instance->numFrames));
    if( status != IUS_E_OK )
    {
        fprintf(stderr, "Warning from iusInputFileNodeLoad: could not load numFrames: %s\n", instance->pFilename);
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

iuif_t iusInputFileNodeLoad
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
        fprintf( stderr, "iusInputFileNodeLoad: could not create file: %s\n", pFilename );
        return IUIF_INVALID;
    }

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


int iusInputFileNodeSave
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
    if( instance->handle != H5I_INVALID_HID )
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

    // Compare instance data
    iuifi_t refInstance = iusHistoryNodeGetInstanceData((iuhn_t)reference);
    iuifi_t actInstance = iusHistoryNodeGetInstanceData((iuhn_t)actual);
    IUS_BOOL equal = iusInputFileCompareInstance(refInstance,actInstance);
    if (equal == IUS_FALSE) return IUS_FALSE;


    return equal;
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



void fillChunkDims
(
    hsize_t *chunkDims,
    hsize_t *rfDataDims,
    int numDims,
    int numItems
)
{
    int i;
    for (i=0;i<numItems;i++)
    {
        if (i<numDims)
            chunkDims[i] = rfDataDims[i];
        else
            chunkDims[i] = 1;
    }
}

hid_t iusInputFileGetWriteSpace
(
    iuif_t inputFile,
    char *label,
    int numDims
)
{
    hsize_t rfDataDims[4];
    hsize_t chunkDims[4];
    hid_t space, dataChunkConfig;

    iuifi_t instance = iusHistoryNodeGetInstanceData((iuhn_t)inputFile);
    iuds_t dataStream = iusDataStreamDictGet(instance->dataStreamDict,label);
    if ( dataStream == IUDS_INVALID)
    {
        // Entry does not exist, create
        rfDataDims[0] = (hsize_t) iusInputFileGetNumChannels(inputFile,label);    // colums in memory
        rfDataDims[1] = (hsize_t) iusInputFileGetSamplesPerLine(inputFile,label); // rows in memory
        rfDataDims[2] = (hsize_t) iusInputFileGetNumResponses(inputFile,label);
        rfDataDims[3] = (hsize_t) iusInputFileGetNumFrames(inputFile);

        fillChunkDims(chunkDims,rfDataDims,numDims,4);

        dataStream = iusDataStreamCreate();

        dataChunkConfig = H5Pcreate(H5P_DATASET_CREATE);
        H5Pset_chunk(dataChunkConfig, 4, chunkDims);
        dataStream->fileChunkConfig = dataChunkConfig;

        space = H5Screate_simple(4, rfDataDims, NULL);
        dataStream->rfDataset = H5Dcreate(instance->handle, label, H5T_NATIVE_FLOAT, space, H5P_DEFAULT, dataChunkConfig, H5P_DEFAULT);
        H5Sclose(space);
        iusDataStreamDictSet(instance->dataStreamDict,label,dataStream);
    }
    return H5Dget_space(dataStream->rfDataset);
}


hid_t iusInputFileGetReadSpace
(
    iuif_t inputFile,
    char *label
)
{
    iuifi_t instance = iusHistoryNodeGetInstanceData((iuhn_t)inputFile);
    iuds_t dataStream = iusDataStreamDictGet(instance->dataStreamDict,label);
    if ( dataStream == IUDS_INVALID)
    {
        dataStream = iusDataStreamCreate();
        dataStream->rfDataset = H5Dopen(instance->handle, label, H5P_DEFAULT);
        iusDataStreamDictSet(instance->dataStreamDict,label,dataStream);
    }
    return H5Dget_space(dataStream->rfDataset);
}


int iusInputFileFrameSave
(
    iuif_t inputFile,
    char *label,
    iud_t frame,
    iuo_t frame_offset
)
{
    hid_t memspace;
    hid_t dataspace;
    hsize_t offset[4];
    hsize_t count[4];
    hsize_t memdim[3];
    herr_t  status;

    memdim[0] = (hsize_t) iusInputFileGetNumChannels(inputFile,label);
    memdim[1] = (hsize_t) iusInputFileGetSamplesPerLine(inputFile,label);
    memdim[2] = (hsize_t) iusInputFileGetNumResponses(inputFile,label);
    memspace = H5Screate_simple(3, memdim, NULL);

    offset[0] = (hsize_t) frame_offset->x;
    offset[1] = (hsize_t) frame_offset->z;
    offset[2] = (hsize_t) frame_offset->y;
    offset[3] = (hsize_t) frame_offset->t;

    count[0] = memdim[0];
    count[1] = memdim[1];
    count[2] = memdim[2];
    count[3] = 1;

    iuifi_t instance = iusHistoryNodeGetInstanceData((iuhn_t)inputFile);
    dataspace = iusInputFileGetWriteSpace(inputFile, label, 3);
    status = H5Sselect_hyperslab(dataspace, H5S_SELECT_SET, offset, NULL, count, NULL);

    float *pFrame = iusDataGetPointer(frame);

    // Save frame
    iuds_t dataStream = iusDataStreamDictGet(instance->dataStreamDict,label);
    status |= H5Dwrite(dataStream->rfDataset, H5T_NATIVE_FLOAT, memspace, dataspace, H5P_DEFAULT, pFrame);

    // Close and release memspace but not (file)dataspace
    status |= H5Sclose(memspace);
    status |= H5Sclose(dataspace);

    return status;
}



int iusInputFileResponseSave
(
    iuif_t inputFile,
    char *label,
    iud_t response,
    iuo_t response_offset
)
{
    hid_t memspace;
    hid_t dataspace;
    hsize_t offset[4];
    hsize_t count[4];
    hsize_t memdim[2];
    herr_t  status;

    memdim[0] = (hsize_t) iusInputFileGetNumChannels(inputFile,label);
    memdim[1] = (hsize_t) iusInputFileGetSamplesPerLine(inputFile,label);
    memspace = H5Screate_simple(2, memdim, NULL);

    offset[0] = (hsize_t) response_offset->x;
    offset[1] = (hsize_t) response_offset->z;
    offset[2] = (hsize_t) response_offset->y;
    offset[3] = (hsize_t) response_offset->t;

    count[0] = memdim[0];
    count[1] = memdim[1];
    count[2] = 1;
    count[3] = 1;

    iuifi_t instance = iusHistoryNodeGetInstanceData((iuhn_t)inputFile);
    dataspace = iusInputFileGetWriteSpace(inputFile, label, 2);
    status = H5Sselect_hyperslab(dataspace, H5S_SELECT_SET, offset, NULL, count, NULL);

    float *pFrame = iusDataGetPointer(response);

    // Save frame
    iuds_t dataStream = iusDataStreamDictGet(instance->dataStreamDict,label);
    status |= H5Dwrite(dataStream->rfDataset, H5T_NATIVE_FLOAT, memspace, dataspace, H5P_DEFAULT, pFrame);

    // Close and release memspace but not (file)dataspace
    status |= H5Sclose(memspace);
    status |= H5Sclose(dataspace);

    return status;
}

int iusInputFileChannelSave
(
    iuif_t inputFile,
    char *label,
    iud_t channel,
    iuo_t channel_offset
)
{
    hid_t memspace;
    hid_t dataspace;
    hsize_t offset[4];
    hsize_t count[4];
    hsize_t memdim;
    herr_t  status;

    memdim = (hsize_t) iusInputFileGetNumChannels(inputFile,label);
    memspace = H5Screate_simple(1, &memdim, NULL);

    offset[0] = (hsize_t) channel_offset->x;
    offset[1] = (hsize_t) channel_offset->z;
    offset[2] = (hsize_t) channel_offset->y;
    offset[3] = (hsize_t) channel_offset->t;

    count[0] = memdim;
    count[1] = 1;
    count[2] = 1;
    count[3] = 1;

    iuifi_t instance = iusHistoryNodeGetInstanceData((iuhn_t)inputFile);
    dataspace = iusInputFileGetWriteSpace(inputFile, label, 1);
    status = H5Sselect_hyperslab(dataspace, H5S_SELECT_SET, offset, NULL, count, NULL);

    float *pFrame = iusDataGetPointer(channel);

    // Save frame
    iuds_t dataStream = iusDataStreamDictGet(instance->dataStreamDict,label);
    status |= H5Dwrite(dataStream->rfDataset, H5T_NATIVE_FLOAT, memspace, dataspace, H5P_DEFAULT, pFrame);

    // Close and release memspace but not (file)dataspace
    status |= H5Sclose(memspace);
    status |= H5Sclose(dataspace);

    return status;
}



int iusInputFileFrameLoad
(
    iuif_t inputFile,
    char *label,
    iud_t frame,
    iuo_t frame_offset
)
{
    hid_t memspace;
    hid_t dataspace;
    hsize_t offset[4];
    hsize_t count[4];
    hsize_t memdim[3];
    herr_t  status;

    memdim[0] = (hsize_t) iusInputFileGetNumChannels(inputFile,label);
    memdim[1] = (hsize_t) iusInputFileGetSamplesPerLine(inputFile,label);
    memdim[2] = (hsize_t) iusInputFileGetNumResponses(inputFile,label);
    memspace = H5Screate_simple(3, memdim, NULL);

    offset[0] = (hsize_t) frame_offset->x;
    offset[1] = (hsize_t) frame_offset->z;
    offset[2] = (hsize_t) frame_offset->y;
    offset[3] = (hsize_t) frame_offset->t;

    count[0] = memdim[0];
    count[1] = memdim[1];
    count[2] = memdim[2];
    count[3] = 1;

    iuifi_t instance = iusHistoryNodeGetInstanceData((iuhn_t)inputFile);
    dataspace = iusInputFileGetReadSpace(inputFile, label);
    status = H5Sselect_hyperslab(dataspace, H5S_SELECT_SET, offset, NULL, count, NULL);

    float *pFrame = iusDataGetPointer(frame);

    // Load frame
    iuds_t dataStream = iusDataStreamDictGet(instance->dataStreamDict,label);
    status |= H5Dread(dataStream->rfDataset, H5T_NATIVE_FLOAT, memspace, dataspace, H5P_DEFAULT, pFrame);

    // Close and release memspace but not (file)dataspace
    status |= H5Sclose(memspace);
    status |= H5Sclose(dataspace);

    return status;

}


int iusInputFileResponseLoad
(
    iuif_t inputFile,
    char *label,
    iud_t frame,
    iuo_t frame_offset
)
{
    hid_t memspace;
    hid_t dataspace;
    hsize_t offset[4];
    hsize_t count[4];
    hsize_t memdim[2];
    herr_t  status;

    memdim[0] = (hsize_t) iusInputFileGetNumChannels(inputFile,label);
    memdim[1] = (hsize_t) iusInputFileGetSamplesPerLine(inputFile,label);
    memspace = H5Screate_simple(2, memdim, NULL);

    offset[0] = (hsize_t) frame_offset->x;
    offset[1] = (hsize_t) frame_offset->z;
    offset[2] = (hsize_t) frame_offset->y;
    offset[3] = (hsize_t) frame_offset->t;

    count[0] = memdim[0];
    count[1] = memdim[1];
    count[2] = 1;
    count[3] = 1;

    iuifi_t instance = iusHistoryNodeGetInstanceData((iuhn_t)inputFile);
    dataspace = iusInputFileGetReadSpace(inputFile, label);
    status = H5Sselect_hyperslab(dataspace, H5S_SELECT_SET, offset, NULL, count, NULL);

    float *pFrame = iusDataGetPointer(frame);

    // Load frame
    iuds_t dataStream = iusDataStreamDictGet(instance->dataStreamDict,label);
    status |= H5Dread(dataStream->rfDataset, H5T_NATIVE_FLOAT, memspace, dataspace, H5P_DEFAULT, pFrame);

    // Close and release memspace but not (file)dataspace
    status |= H5Sclose(memspace);
    status |= H5Sclose(dataspace);

    return status;

}


int iusInputFileChannelLoad
(
    iuif_t inputFile,
    char *label,
    iud_t channel,
    iuo_t channel_offset
)
{
    hid_t memspace;
    hid_t dataspace;
    hsize_t offset[4];
    hsize_t count[4];
    hsize_t memdim;
    herr_t  status;

    memdim = (hsize_t) iusInputFileGetNumChannels(inputFile,label);
    memspace = H5Screate_simple(1, &memdim, NULL);

    offset[0] = (hsize_t) channel_offset->x;
    offset[1] = (hsize_t) channel_offset->z;
    offset[2] = (hsize_t) channel_offset->y;
    offset[3] = (hsize_t) channel_offset->t;

    count[0] = memdim;
    count[1] = 1;
    count[2] = 1;
    count[3] = 1;

    iuifi_t instance = iusHistoryNodeGetInstanceData((iuhn_t)inputFile);
    dataspace = iusInputFileGetReadSpace(inputFile, label);
    status = H5Sselect_hyperslab(dataspace, H5S_SELECT_SET, offset, NULL, count, NULL);

    float *pFrame = iusDataGetPointer(channel);

    // Load channel
    iuds_t dataStream = iusDataStreamDictGet(instance->dataStreamDict,label);
    status |= H5Dread(dataStream->rfDataset, H5T_NATIVE_FLOAT, memspace, dataspace, H5P_DEFAULT, pFrame);

    // Close and release memspace but not (file)dataspace
    status |= H5Sclose(memspace);
    status |= H5Sclose(dataspace);

    return status;
}
