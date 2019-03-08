
//
// Created by nlv09165 on 11/07/2018.
//
#include <stdlib.h>
#include <string.h>

#include <ius.h>
#include <iusAcquisitionPrivate.h>
#include <iusDataStreamDictPrivate.h>
#include <iusFrameListPrivate.h>
#include <iusHistoryNodePrivate.h>
#include <iusInputFilePrivate.h>
#include <iusInputFileStructure.h>
#include <iusPatternListDictPrivate.h>
#include <iusPulseDictPrivate.h>
#include <iusReceiveChannelMapDictPrivate.h>
#include <iusReceiveSettingsDictPrivate.h>
#include <iusSourceDictPrivate.h>
#include <iusTransducerPrivate.h>
#include <iusTransmitApodizationDictPrivate.h>

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
	iua_t acquisition;                    /**< The description of the performed acquisition */
    int IusVersion;                      /**< The version of input file format */


    //  state variables
    hid_t               handle;           /**< HDF5 file handle     */
    const char          *pFilename;       /**< the filename         */
    iudsd_t             dataStreamDict;   /**< Contains dataset administration */
    IUS_BOOL            deepDelete;
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
    IUS_ERR_ALLOC_NULL_N_RETURN(instanceData, IusInputFileInstance, IUIFI_INVALID);

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
	instanceData->acquisition = IUA_INVALID;
	instanceData->dataStreamDict = iusDataStreamDictCreate();
	instanceData->deepDelete = IUS_FALSE;
    return instanceData;
}

int iusInputFileInstanceDelete
(
    iuifi_t instance
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(instance, IUS_ERR_VALUE);
    iusDataStreamDictDelete(instance->dataStreamDict);
    free((void *)instance->pFilename);
    if(instance->deepDelete == IUS_TRUE)
    {
        iusFrameListDelete(instance->frameList);
        iusAcquisitionDelete(instance->acquisition);
        iusTransducerDelete(instance->transducer);
        iusReceiveSettingsDictDelete(instance->receiveSettingsDict);
        iusTransmitApodizationDictDelete(instance->transmitApodizationDict);
        iusReceiveChannelMapDictDelete(instance->receiveChannelMapDict);
        iusSourceDictDelete(instance->pulseSourceDict);
        iusPulseDictDelete(instance->pulseDict);
        iusPatternListDictDelete(instance->patternListDict);
    }
    free(instance);
    return IUS_E_OK;
}

// ADT
int iusInputFileDelete
(
    iuif_t inputFile
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(inputFile, IUS_ERR_VALUE);
    iuifi_t instance = iusHistoryNodeGetInstanceData(inputFile);
    iusInputFileInstanceDelete(instance);
    iusHistoryNodeDelete((iuhn_t)inputFile);
    return IUS_E_OK;
}

iuif_t iusInputFileCreate
(
    const char *filename
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(filename, IUIF_INVALID);
	iuifi_t instanceData = iusInputFileInstanceCreate();
	if (instanceData == IUIFI_INVALID)
	{
        IUS_ERROR_PUSH(IUS_ERR_MAJ_MEMORY, IUS_ERR_MIN_ALLOC,  "calloc of IusInputFileInstance failed");
		return IUIF_INVALID;
	}

    instanceData->pFilename = strdup(filename);
    instanceData->handle = H5Fcreate(filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
	if (instanceData->handle == H5I_INVALID_HID)
	{
        IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_ERROR, IUS_ERR_MIN_HDF5,  "unable to create file %s", filename);
        iusInputFileInstanceDelete(instanceData);
		return IUIF_INVALID;
	}
	iuhn_t node = iusHistoryNodeCreate(IUS_INPUT_TYPE);
	iusHistoryNodeSetInstanceData(node,(void *)instanceData);
	return (iuif_t)node;
}


int iusInputFileGetNumFrames
(
    iuif_t inputFile
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(inputFile, -1);
    iuifi_t instance = iusHistoryNodeGetInstanceData((iuhn_t)inputFile);
    if (instance->frameList == IUFL_INVALID) return 0;
    return iusFrameListGetSize(instance->frameList);
}

int iusInputFileGetNumChannels
(
    iuif_t inputFile,
    char *label
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(inputFile, -1);
    IUS_ERR_CHECK_NULL_N_RETURN(label, -1);
    iurcmd_t receiveChannelMapDict = iusInputFileGetReceiveChannelMapDict(inputFile);
    if (receiveChannelMapDict == IURCMD_INVALID) return -1;
    iurcm_t receiveChannelMap = iusReceiveChannelMapDictGet(receiveChannelMapDict,label);
    if (receiveChannelMap == IURCM_INVALID) return -1;
    return iusReceiveChannelMapGetNumChannels(receiveChannelMap);
}


int iusInputFileGetSamplesPerLine
(
    iuif_t inputFile,
    char *label
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(inputFile, -1);
    IUS_ERR_CHECK_NULL_N_RETURN(label, -1);

    iursd_t receiveSettingsDict = iusInputFileGetReceiveSettingsDict(inputFile);
    if (receiveSettingsDict == IURSD_INVALID)
    {
        IUS_ERROR_PUSH(IUS_ERR_MAJ_ERROR, IUS_ERR_MIN_ARG_VALUE,  "unable to iusInputFileGetReceiveSettingsDict");
        return -1;
    }
    iurs_t receiveSettings = iusReceiveSettingsDictGet(receiveSettingsDict,label);
    if (receiveSettings == IURS_INVALID)
    {
        IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_ERROR, IUS_ERR_MIN_ARG_VALUE,  "unable to get receiveSettings for label %s", label);
        return -1;
    }
    return iusReceiveSettingsGetNumSamplesPerLine(receiveSettings);
}

int iusInputFileGetNumResponses
(
    iuif_t inputFile,
    char *label
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(inputFile, -1);
    IUS_ERR_CHECK_NULL_N_RETURN(label, -1);

    iupald_t listDict = iusInputFileGetPatternListDict(inputFile);
    if (listDict == IUPALD_INVALID)
    {
        IUS_ERROR_PUSH(IUS_ERR_MAJ_ERROR, IUS_ERR_MIN_ARG_VALUE,  "unable to iusInputFileGetPatternListDict");
        return -1;
    }
    iupal_t patternList = iusPatternListDictGet(listDict,label);
    if (patternList == IUPAL_INVALID)
    {
        IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_ERROR, IUS_ERR_MIN_ARG_VALUE,  "unable to get patternLlist for label %s", label);
        return -1;
    }

    return iusPatternListGetSize(patternList);
}

iud_t iusInputFileFrameCreate
(
    iuif_t inputFile,
    char *label
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(inputFile, IUD_INVALID);
    IUS_ERR_CHECK_NULL_N_RETURN(label, IUD_INVALID);
    // calculate frame size:
    // numchannels * numresponses * numsamples = [x*y*z]
    int numChannels = iusInputFileGetNumChannels(inputFile,label);
    int numResponses = iusInputFileGetNumResponses(inputFile,label);
    int numSamples = iusInputFileGetSamplesPerLine(inputFile,label);
    int frameSize = numChannels * numSamples * numResponses;
    return iusDataCreate(frameSize);
}

iud_t iusInputFileResponseCreate
(
    iuif_t inputFile,
    char *label
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(inputFile, IUD_INVALID);
    IUS_ERR_CHECK_NULL_N_RETURN(label, IUD_INVALID);
    int numChannels = iusInputFileGetNumChannels(inputFile,label);
    int numSamples = iusInputFileGetSamplesPerLine(inputFile,label);
    int responseSize = numChannels * numSamples ;
    return iusDataCreate(responseSize);
}

iud_t iusInputFileChannelCreate
(
    iuif_t inputFile,
    char *label
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(inputFile, IUD_INVALID);
    IUS_ERR_CHECK_NULL_N_RETURN(label, IUD_INVALID);
    int numSamples = iusInputFileGetSamplesPerLine(inputFile,label);
    return iusDataCreate(numSamples);
}


static iuifi_t inputFileInstanceLoad
(
    iuifi_t instance
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(instance, IUIFI_INVALID);
    instance->frameList = iusFrameListLoad(instance->handle);
    if (instance->frameList == IUFL_INVALID)
    {
        IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_HDF5, IUS_ERR_MIN_HDF5, "could not load framelist from %s", instance->pFilename);
        return IUIFI_INVALID;
    }

    instance->patternListDict = iusPatternListDictLoad(instance->handle);
    if (instance->patternListDict == IUPALD_INVALID)
    {
        IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_HDF5, IUS_ERR_MIN_HDF5, "could not load patternlist from %s", instance->pFilename);
        return IUIFI_INVALID;
    }

    instance->pulseDict = iusPulseDictLoad(instance->handle);
    if (instance->pulseDict == IUPD_INVALID)
    {
        IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_HDF5, IUS_ERR_MIN_HDF5, "could not load pulses from %s", instance->pFilename);
        return IUIFI_INVALID;
    }

    instance->pulseSourceDict = iusSourceDictLoad(instance->handle);
    if (instance->pulseSourceDict == IUSD_INVALID)
    {
        IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_HDF5, IUS_ERR_MIN_HDF5, "could not load pulse sources from %s", instance->pFilename);
        return IUIFI_INVALID;
    }

    instance->receiveChannelMapDict = iusReceiveChannelMapDictLoad(instance->handle);
    if (instance->receiveChannelMapDict == IURCMD_INVALID)
    {
        IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_HDF5, IUS_ERR_MIN_HDF5, "could not load receiveChannelMap from %s", instance->pFilename);
        return IUIFI_INVALID;
    }

    instance->transmitApodizationDict = iusTransmitApodizationDictLoad(instance->handle);
    if (instance->transmitApodizationDict == IUTAD_INVALID)
    {
        IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_HDF5, IUS_ERR_MIN_HDF5, "could not load transmitApodizationDict from %s", instance->pFilename);
        return IUIFI_INVALID;
    }

    instance->receiveSettingsDict = iusReceiveSettingsDictLoad(instance->handle);
    if (instance->receiveSettingsDict == IURSD_INVALID)
    {
        IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_HDF5, IUS_ERR_MIN_HDF5, "could not load receiveSettingsDict from %s", instance->pFilename);
        return IUIFI_INVALID;
    }

    instance->acquisition = iusAcquisitionLoad(instance->handle);
    if (instance->acquisition == IUA_INVALID)
    {
        IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_HDF5, IUS_ERR_MIN_HDF5, "could not load acquisition from %s", instance->pFilename);
        return IUIFI_INVALID;
    }

    instance->transducer = iusTransducerLoad(instance->handle);
    if (instance->transducer == IUT_INVALID)
    {
        IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_HDF5, IUS_ERR_MIN_HDF5, "could not load transducer from %s", instance->pFilename);
        return IUIFI_INVALID;
    }

    int status = iusHdf5ReadInt( instance->handle, IUS_INPUTFILE_PATH_IUSVERSION, &(instance->IusVersion));
    if( status != IUS_E_OK )
    {
        IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_HDF5, IUS_ERR_MIN_HDF5, "could not load IusVersion from %s", instance->pFilename);
        return IUIFI_INVALID;
    }

    instance->deepDelete = IUS_TRUE;
    return instance;
}

void *iusInputFileInstanceLoad
(
    hid_t handle
)
{
    iuifi_t instance = iusInputFileInstanceCreate();
    iuifi_t new_instance;
    instance->handle = handle;
    new_instance = inputFileInstanceLoad(instance);
    if( new_instance == IUIFI_INVALID )
    {
        iusInputFileInstanceDelete(instance);
        instance = new_instance;
    }
    instance->deepDelete = IUS_TRUE;
    return (void *)instance;
}


iuhn_t iusInputFileLoadNode
(
    hid_t handle
)
{
    iuhn_t node = iusHistoryNodeCreate(IUS_INPUT_TYPE);
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
    IUS_ERR_CHECK_NULL_N_RETURN(pFilename, IUIF_INVALID);

    // open  Hdf5 file using default properties.
    hid_t handle = H5Fopen( pFilename, H5F_ACC_RDONLY, H5P_DEFAULT );
    if (handle<=0)
    {
        IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_HDF5, IUS_ERR_MIN_HDF5, "could not open file %s", pFilename);
        return IUIF_INVALID;
    }

    iuif_t inputFile = (iuif_t) iusHistoryNodeLoad(handle);
    iuifi_t instance = iusHistoryNodeGetInstanceData((iuhn_t)inputFile);
    if (instance == IUIFI_INVALID)
    {
        IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_HDF5, IUS_ERR_MIN_HDF5, "could not get instance data %s", pFilename);
        return IUIF_INVALID;
    }
    instance->pFilename = strdup(pFilename);
    return inputFile;
}

int iusInputFileSaveInstance
(
    hid_t handle,
    iuifi_t instanceData
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(instanceData, IUS_ERR_VALUE);
    herr_t status=0;
    status |= iusFrameListSave(instanceData->frameList, handle);
    status |= iusPatternListDictSave(instanceData->patternListDict, handle);
    status |= iusPulseDictSave(instanceData->pulseDict, handle);
    status |= iusSourceDictSave(instanceData->pulseSourceDict, handle);
    status |= iusReceiveChannelMapDictSave(instanceData->receiveChannelMapDict, handle);
    status |= iusTransmitApodizationDictSave(instanceData->transmitApodizationDict, handle);
    status |= iusReceiveSettingsDictSave(instanceData->receiveSettingsDict, handle);

    status |= iusAcquisitionSave(instanceData->acquisition, handle);
    status |= iusTransducerSave(instanceData->transducer, instanceData->handle);
    status |= iusHdf5WriteInt( instanceData->handle, IUS_INPUTFILE_PATH_IUSVERSION, &(instanceData->IusVersion), 1);
    return status;
}


int iusInputFileNodeSave
(
    iuif_t inputFile
)
{
    herr_t status=0;
    IUS_ERR_CHECK_NULL_N_RETURN(inputFile, IUS_ERR_VALUE);
    iuifi_t instance = iusHistoryNodeGetInstanceData((iuhn_t)inputFile);
    status |= iusHistoryNodeSave(inputFile, instance->handle);
	return status;
}


int iusInputFileClose
(
    iuif_t inputFile
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(inputFile, IUS_ERR_VALUE);
    iuifi_t instance = iusHistoryNodeGetInstanceData((iuhn_t)inputFile);
    IUS_ERR_EVAL_N_RETURN(instance->handle  == H5I_INVALID_HID, IUS_ERR_VALUE);

    // Terminate access to the file.
    int status = H5Fclose(instance->handle);
    instance->handle = H5I_INVALID_HID;
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
    if ( strcmp(reference->pFilename, actual->pFilename) != 0 ) return IUS_FALSE;
    if ( iusFrameListCompare(reference->frameList, actual->frameList)  == IUS_FALSE ) return IUS_FALSE;
    if ( iusPatternListDictCompare(reference->patternListDict, actual->patternListDict)  == IUS_FALSE ) return IUS_FALSE;
    if ( iusPulseDictCompare(reference->pulseDict, actual->pulseDict)  == IUS_FALSE ) return IUS_FALSE;
    if ( iusSourceDictCompare(reference->pulseSourceDict, actual->pulseSourceDict)  == IUS_FALSE ) return IUS_FALSE;
    if ( iusReceiveChannelMapDictCompare(reference->receiveChannelMapDict, actual->receiveChannelMapDict) == IUS_FALSE) return IUS_FALSE;
    if ( iusTransmitApodizationDictCompare(reference->transmitApodizationDict, actual->transmitApodizationDict)  == IUS_FALSE ) return IUS_FALSE;
    if ( iusReceiveSettingsDictCompare(reference->receiveSettingsDict, actual->receiveSettingsDict)  == IUS_FALSE ) return IUS_FALSE;
    if ( iusTransducerCompare(reference->transducer, actual->transducer) == IUS_FALSE) return IUS_FALSE;
    if (iusAcquisitionCompare(reference->acquisition, actual->acquisition) == IUS_FALSE) return IUS_FALSE;
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
    iuif_t inputFile
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(inputFile, IUFL_INVALID);
    iuifi_t instance = iusHistoryNodeGetInstanceData((iuhn_t)inputFile);
    return instance->frameList;
}

iupald_t iusInputFileGetPatternListDict
(
    iuif_t inputFile
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(inputFile, IUPALD_INVALID);
    iuifi_t instance = iusHistoryNodeGetInstanceData((iuhn_t)inputFile);
    return instance->patternListDict;
}


iupd_t iusInputFileGetPulseDict
(
    iuif_t inputFile
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(inputFile, IUPD_INVALID);
    iuifi_t instance = iusHistoryNodeGetInstanceData((iuhn_t)inputFile);
    return instance->pulseDict;
}

iusd_t iusInputFileGetSourceDict
(
    iuif_t inputFile
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(inputFile, IUSD_INVALID);
    iuifi_t instance = iusHistoryNodeGetInstanceData((iuhn_t)inputFile);
    return instance->pulseSourceDict;
}


iurcmd_t iusInputFileGetReceiveChannelMapDict
(
    iuif_t inputFile
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(inputFile, IURCMD_INVALID);
    iuifi_t instance = iusHistoryNodeGetInstanceData((iuhn_t)inputFile);
    return instance->receiveChannelMapDict;
}

iutad_t iusInputFileGetTransmitApodizationDict
(
    iuif_t inputFile
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(inputFile, IUTAD_INVALID);
    iuifi_t instance = iusHistoryNodeGetInstanceData((iuhn_t)inputFile);
    return instance->transmitApodizationDict;
}

iursd_t iusInputFileGetReceiveSettingsDict
(
    iuif_t inputFile
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(inputFile, IURSD_INVALID);
    iuifi_t instance = iusHistoryNodeGetInstanceData((iuhn_t)inputFile);
    return instance->receiveSettingsDict;
}

iua_t iusInputFileGetAcquisition
(
   iuif_t inputFile
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(inputFile, IUA_INVALID);
    iuifi_t instance = iusHistoryNodeGetInstanceData((iuhn_t)inputFile);
    return instance->acquisition;
}

iut_t iusInputFileGetTransducer
(
    iuif_t inputFile
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(inputFile, IUT_INVALID);
    iuifi_t instance = iusHistoryNodeGetInstanceData((iuhn_t)inputFile);
    return instance->transducer;
}



// Setters
int iusInputFileSetFrameList
(
    iuif_t inputFile,
    iufl_t frameList
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(inputFile, IUS_ERR_VALUE);
    IUS_ERR_CHECK_NULL_N_RETURN(frameList, IUS_ERR_VALUE);
    iuifi_t instance = iusHistoryNodeGetInstanceData((iuhn_t)inputFile);
    instance->frameList = frameList;
    return IUS_E_OK;
}

int iusInputFileSetPatternListDict
(
    iuif_t inputFile,
    iupald_t patternListDict
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(inputFile, IUS_ERR_VALUE);
    IUS_ERR_CHECK_NULL_N_RETURN(patternListDict, IUS_ERR_VALUE);
    iuifi_t instance = iusHistoryNodeGetInstanceData((iuhn_t)inputFile);
    instance->patternListDict = patternListDict;
    return IUS_E_OK;
}

int iusInputFileSetPulseDict
(
    iuif_t inputFile,
    iupd_t pulseDict
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(inputFile, IUS_ERR_VALUE);
    IUS_ERR_CHECK_NULL_N_RETURN(pulseDict, IUS_ERR_VALUE);
    iuifi_t instance = iusHistoryNodeGetInstanceData((iuhn_t)inputFile);
    instance->pulseDict = pulseDict;
    return IUS_E_OK;
}



int iusInputFileSetSourceDict
(
    iuif_t inputFile,
    iusd_t sourceDict
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(inputFile, IUS_ERR_VALUE);
    IUS_ERR_CHECK_NULL_N_RETURN(sourceDict, IUS_ERR_VALUE);
    iuifi_t instance = iusHistoryNodeGetInstanceData((iuhn_t)inputFile);
    instance->pulseSourceDict = sourceDict;
    return IUS_E_OK;
}

int iusInputFileSetReceiveChannelMapDict
(
	iuif_t inputFile,
	iurcmd_t receiveChannelMapDict
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(inputFile, IUS_ERR_VALUE);
    IUS_ERR_CHECK_NULL_N_RETURN(receiveChannelMapDict, IUS_ERR_VALUE);
    iuifi_t instance = iusHistoryNodeGetInstanceData((iuhn_t)inputFile);
    instance->receiveChannelMapDict = receiveChannelMapDict;
    return IUS_E_OK;
}

int iusInputFileSetTransmitApodizationDict
(
	iuif_t inputFile,
	iutad_t transmitApodizationDict
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(inputFile, IUS_ERR_VALUE);
    IUS_ERR_CHECK_NULL_N_RETURN(transmitApodizationDict, IUS_ERR_VALUE);
    iuifi_t instance = iusHistoryNodeGetInstanceData((iuhn_t)inputFile);
    instance->transmitApodizationDict = transmitApodizationDict;
    return IUS_E_OK;
}

int iusInputFileSetReceiveSettingsDict
(
    iuif_t inputFile,
    iursd_t receiveSettingsDict
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(inputFile, IUS_ERR_VALUE);
    IUS_ERR_CHECK_NULL_N_RETURN(receiveSettingsDict, IUS_ERR_VALUE);
    iuifi_t instance = iusHistoryNodeGetInstanceData((iuhn_t)inputFile);
    instance->receiveSettingsDict = receiveSettingsDict;
    return IUS_E_OK;
}


int iusInputFileSetAcquisition
(
    iuif_t inputFile,
    iua_t acquisition
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(inputFile, IUS_ERR_VALUE);
    IUS_ERR_CHECK_NULL_N_RETURN(acquisition, IUS_ERR_VALUE);
    iuifi_t instance = iusHistoryNodeGetInstanceData((iuhn_t)inputFile);
    instance->acquisition = acquisition;
    return IUS_E_OK;
}


int iusInputFileSetTransducer
(
    iuif_t inputFile,
    iut_t transducer
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(inputFile, IUS_ERR_VALUE);
    IUS_ERR_CHECK_NULL_N_RETURN(transducer, IUS_ERR_VALUE);
    iuifi_t instance = iusHistoryNodeGetInstanceData((iuhn_t)inputFile);
    instance->transducer = transducer;
    return IUS_E_OK;
}

int iusInputFileSetFilename
(
    iuif_t inputFile,
    char * fileName
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(inputFile, IUS_ERR_VALUE);
    IUS_ERR_CHECK_NULL_N_RETURN(fileName, IUS_ERR_VALUE);
    iuifi_t instance = iusHistoryNodeGetInstanceData((iuhn_t)inputFile);
    instance->pFilename = strdup(fileName);
    return IUS_E_OK;
}


static void fillChunkDims
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

    IUS_ERR_CHECK_NULL_N_RETURN(inputFile, H5I_INVALID_HID);
    IUS_ERR_CHECK_NULL_N_RETURN(label, H5I_INVALID_HID);

    iuifi_t instance = iusHistoryNodeGetInstanceData((iuhn_t)inputFile);
    iuds_t dataStream = iusDataStreamDictGet(instance->dataStreamDict,label);
    if ( dataStream == IUDS_INVALID)
    {
        iusErrorLogClear();
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
    IUS_ERR_CHECK_NULL_N_RETURN(inputFile, H5I_INVALID_HID);
    IUS_ERR_CHECK_NULL_N_RETURN(label, H5I_INVALID_HID);
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

    IUS_ERR_CHECK_NULL_N_RETURN(inputFile, H5I_INVALID_HID);
    IUS_ERR_CHECK_NULL_N_RETURN(label, H5I_INVALID_HID);
    IUS_ERR_CHECK_NULL_N_RETURN(frame, H5I_INVALID_HID);
    IUS_ERR_CHECK_NULL_N_RETURN(frame_offset, H5I_INVALID_HID);

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

    IUS_ERR_CHECK_NULL_N_RETURN(inputFile, H5I_INVALID_HID);
    IUS_ERR_CHECK_NULL_N_RETURN(label, H5I_INVALID_HID);
    IUS_ERR_CHECK_NULL_N_RETURN(response, H5I_INVALID_HID);
    IUS_ERR_CHECK_NULL_N_RETURN(response_offset, H5I_INVALID_HID);

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

    IUS_ERR_CHECK_NULL_N_RETURN(inputFile, H5I_INVALID_HID);
    IUS_ERR_CHECK_NULL_N_RETURN(label, H5I_INVALID_HID);
    IUS_ERR_CHECK_NULL_N_RETURN(channel, H5I_INVALID_HID);
    IUS_ERR_CHECK_NULL_N_RETURN(channel_offset, H5I_INVALID_HID);

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

    IUS_ERR_CHECK_NULL_N_RETURN(inputFile, H5I_INVALID_HID);
    IUS_ERR_CHECK_NULL_N_RETURN(label, H5I_INVALID_HID);
    IUS_ERR_CHECK_NULL_N_RETURN(frame, H5I_INVALID_HID);
    IUS_ERR_CHECK_NULL_N_RETURN(frame_offset, H5I_INVALID_HID);

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

    IUS_ERR_CHECK_NULL_N_RETURN(inputFile, H5I_INVALID_HID);
    IUS_ERR_CHECK_NULL_N_RETURN(label, H5I_INVALID_HID);
    IUS_ERR_CHECK_NULL_N_RETURN(response, H5I_INVALID_HID);
    IUS_ERR_CHECK_NULL_N_RETURN(response_offset, H5I_INVALID_HID);

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
    dataspace = iusInputFileGetReadSpace(inputFile, label);
    status = H5Sselect_hyperslab(dataspace, H5S_SELECT_SET, offset, NULL, count, NULL);

    float *pResponse = iusDataGetPointer(response);

    // Load frame
    iuds_t dataStream = iusDataStreamDictGet(instance->dataStreamDict,label);
    status |= H5Dread(dataStream->rfDataset, H5T_NATIVE_FLOAT, memspace, dataspace, H5P_DEFAULT, pResponse);

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

    IUS_ERR_CHECK_NULL_N_RETURN(inputFile, H5I_INVALID_HID);
    IUS_ERR_CHECK_NULL_N_RETURN(label, H5I_INVALID_HID);
    IUS_ERR_CHECK_NULL_N_RETURN(channel, H5I_INVALID_HID);
    IUS_ERR_CHECK_NULL_N_RETURN(channel_offset, H5I_INVALID_HID);

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
