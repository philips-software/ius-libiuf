
#include <stdlib.h>
#include <string.h>

#include <iuf.h>
#include <iufAcquisitionPrivate.h>
#include <iufDataStreamDictPrivate.h>
#include <iufFrameListPrivate.h>
#include <iufHistoryNodePrivate.h>
#include <iufInputFilePrivate.h>
#include <iufInputFileStructure.h>
#include <iufPatternListDictPrivate.h>
#include <iufPulseDictPrivate.h>
#include <iufReceiveChannelMapDictPrivate.h>
#include <iufReceiveSettingsDictPrivate.h>
#include <iufSourceDictPrivate.h>
#include <iufTransducerPrivate.h>
#include <iufTransmitApodizationDictPrivate.h>

struct IufInputFileInstance
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
    int IufVersion;                      /**< The version of input file format */


    //  state variables
    hid_t               handle;           /**< HDF5 file handle     */
    const char          *pFilename;       /**< the filename         */
    iudsd_t             dataStreamDict;   /**< Contains dataset administration */
    IUF_BOOL            deepDelete;
}  ;



struct IufInputFile
{
    iuhn_t history;
};

iuifi_t iufInputFileInstanceCreate
(
    void
)
{
    iuifi_t instanceData = (iuifi_t) calloc(1, sizeof(IufInputFileInstance));
    IUF_ERR_ALLOC_NULL_N_RETURN(instanceData, IufInputFileInstance, IUIFI_INVALID);

    instanceData->IufVersion = iufGetVersionMajor();
    instanceData->pFilename = NULL;
    instanceData->frameList = IUFL_INVALID;
    instanceData->patternListDict = IUPALD_INVALID;
    instanceData->pulseDict = IUPD_INVALID;
    instanceData->pulseSourceDict = IUFD_INVALID;
    instanceData->receiveChannelMapDict = IURCMD_INVALID;
    instanceData->transmitApodizationDict = IUTAD_INVALID;
    instanceData->receiveSettingsDict = IURSD_INVALID;
    instanceData->transducer = IUT_INVALID;
    instanceData->acquisition = IUA_INVALID;
    instanceData->dataStreamDict = iufDataStreamDictCreate();
    instanceData->deepDelete = IUF_FALSE;
    return instanceData;
}

int iufInputFileInstanceDelete
(
    iuifi_t instance
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(instance, IUF_ERR_VALUE);
    iufDataStreamDictDelete(instance->dataStreamDict);
    if(instance->pFilename != NULL)
    {
        free((void *)instance->pFilename);
    }

    if(instance->deepDelete == IUF_TRUE)
    {
        iufFrameListDelete(instance->frameList);
        iufAcquisitionDelete(instance->acquisition);
        iufTransducerDelete(instance->transducer);
        iufReceiveSettingsDictDelete(instance->receiveSettingsDict);
        iufTransmitApodizationDictDelete(instance->transmitApodizationDict);
        iufReceiveChannelMapDictDelete(instance->receiveChannelMapDict);
        iufSourceDictDelete(instance->pulseSourceDict);
        iufPulseDictDelete(instance->pulseDict);
        iufPatternListDictDelete(instance->patternListDict);
    }
    free(instance);
    return IUF_E_OK;
}

// ADT
int iufInputFileDelete
(
    iuif_t inputFile
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(inputFile, IUF_ERR_VALUE);
    iuifi_t instance = iufHistoryNodeGetInstanceData(inputFile);
    iufInputFileInstanceDelete(instance);
    iufHistoryNodeDelete((iuhn_t)inputFile);
    return IUF_E_OK;
}

iuif_t iufInputFileCreate
(
    const char *filename
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(filename, IUIF_INVALID);
    iuifi_t instanceData = iufInputFileInstanceCreate();
    if (instanceData == IUIFI_INVALID)
    {
        IUF_ERROR_PUSH(IUF_ERR_MAJ_MEMORY, IUF_ERR_MIN_ALLOC,  "calloc of IufInputFileInstance failed");
        return IUIF_INVALID;
    }

    instanceData->pFilename = strdup(filename);
    instanceData->handle = H5Fcreate(filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
    if (instanceData->handle == H5I_INVALID_HID)
    {
        IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_ERROR, IUF_ERR_MIN_HDF5,  "unable to create file %s", filename);
        iufInputFileInstanceDelete(instanceData);
        return IUIF_INVALID;
    }
    iuhn_t node = iufHistoryNodeCreate(IUF_INPUT_TYPE);
    iufHistoryNodeSetInstanceData(node,(void *)instanceData);
    return (iuif_t)node;
}


int iufInputFileGetNumFrames
(
    iuif_t inputFile
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(inputFile, -1);
    iuifi_t instance = iufHistoryNodeGetInstanceData((iuhn_t)inputFile);
    if (instance->frameList == IUFL_INVALID) return 0;
    return iufFrameListGetSize(instance->frameList);
}

int iufInputFileGetNumChannels
(
    iuif_t inputFile,
    char *label
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(inputFile, -1);
    IUF_ERR_CHECK_NULL_N_RETURN(label, -1);
    iurcmd_t receiveChannelMapDict = iufInputFileGetReceiveChannelMapDict(inputFile);
    if (receiveChannelMapDict == IURCMD_INVALID) return -1;
    iurcm_t receiveChannelMap = iufReceiveChannelMapDictGet(receiveChannelMapDict,label);
    if (receiveChannelMap == IURCM_INVALID) return -1;
    return iufReceiveChannelMapGetNumChannels(receiveChannelMap);
}


int iufInputFileGetSamplesPerLine
(
    iuif_t inputFile,
    char *label
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(inputFile, -1);
    IUF_ERR_CHECK_NULL_N_RETURN(label, -1);

    iursd_t receiveSettingsDict = iufInputFileGetReceiveSettingsDict(inputFile);
    if (receiveSettingsDict == IURSD_INVALID)
    {
        IUF_ERROR_PUSH(IUF_ERR_MAJ_ERROR, IUF_ERR_MIN_ARG_VALUE,  "unable to iufInputFileGetReceiveSettingsDict");
        return -1;
    }
    iurs_t receiveSettings = iufReceiveSettingsDictGet(receiveSettingsDict,label);
    if (receiveSettings == IURS_INVALID)
    {
        IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_ERROR, IUF_ERR_MIN_ARG_VALUE,  "unable to get receiveSettings for label %s", label);
        return -1;
    }
    return iufReceiveSettingsGetNumSamplesPerLine(receiveSettings);
}

int iufInputFileGetNumResponses
(
    iuif_t inputFile,
    char *label
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(inputFile, -1);
    IUF_ERR_CHECK_NULL_N_RETURN(label, -1);

    iupald_t listDict = iufInputFileGetPatternListDict(inputFile);
    if (listDict == IUPALD_INVALID)
    {
        IUF_ERROR_PUSH(IUF_ERR_MAJ_ERROR, IUF_ERR_MIN_ARG_VALUE,  "unable to iufInputFileGetPatternListDict");
        return -1;
    }
    iupal_t patternList = iufPatternListDictGet(listDict,label);
    if (patternList == IUPAL_INVALID)
    {
        IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_ERROR, IUF_ERR_MIN_ARG_VALUE,  "unable to get patternLlist for label %s", label);
        return -1;
    }

    return iufPatternListGetSize(patternList);
}

iud_t iufInputFileFrameCreate
(
    iuif_t inputFile,
    char *label
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(inputFile, IUD_INVALID);
    IUF_ERR_CHECK_NULL_N_RETURN(label, IUD_INVALID);
    // calculate frame size:
    // numchannels * numresponses * numsamples = [x*y*z]
    int numChannels = iufInputFileGetNumChannels(inputFile,label);
    int numResponses = iufInputFileGetNumResponses(inputFile,label);
    int numSamples = iufInputFileGetSamplesPerLine(inputFile,label);
    int frameSize = numChannels * numSamples * numResponses;
    return iufDataCreate(frameSize);
}

iud_t iufInputFileResponseCreate
(
    iuif_t inputFile,
    char *label
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(inputFile, IUD_INVALID);
    IUF_ERR_CHECK_NULL_N_RETURN(label, IUD_INVALID);
    int numChannels = iufInputFileGetNumChannels(inputFile,label);
    int numSamples = iufInputFileGetSamplesPerLine(inputFile,label);
    int responseSize = numChannels * numSamples ;
    return iufDataCreate(responseSize);
}

iud_t iufInputFileChannelCreate
(
    iuif_t inputFile,
    char *label
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(inputFile, IUD_INVALID);
    IUF_ERR_CHECK_NULL_N_RETURN(label, IUD_INVALID);
    int numSamples = iufInputFileGetSamplesPerLine(inputFile,label);
    return iufDataCreate(numSamples);
}


static iuifi_t inputFileInstanceLoad
(
    hid_t handle
)
{
    IUF_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUIFI_INVALID);
    iuifi_t instance = iufInputFileInstanceCreate();
    instance->handle = handle;
    instance->frameList = iufFrameListLoad(instance->handle);
    if (instance->frameList == IUFL_INVALID)
    {
        IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_HDF5, IUF_ERR_MIN_HDF5, "could not load framelist from %s", instance->pFilename);
        return IUIFI_INVALID;
    }

    instance->patternListDict = iufPatternListDictLoad(instance->handle);
    if (instance->patternListDict == IUPALD_INVALID)
    {
        IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_HDF5, IUF_ERR_MIN_HDF5, "could not load patternlist from %s", instance->pFilename);
        return IUIFI_INVALID;
    }

    instance->pulseDict = iufPulseDictLoad(instance->handle);
    if (instance->pulseDict == IUPD_INVALID)
    {
        IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_HDF5, IUF_ERR_MIN_HDF5, "could not load pulses from %s", instance->pFilename);
        return IUIFI_INVALID;
    }

    instance->pulseSourceDict = iufSourceDictLoad(instance->handle);
    if (instance->pulseSourceDict == IUFD_INVALID)
    {
        IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_HDF5, IUF_ERR_MIN_HDF5, "could not load pulse sources from %s", instance->pFilename);
        return IUIFI_INVALID;
    }

    instance->receiveChannelMapDict = iufReceiveChannelMapDictLoad(instance->handle);
    if (instance->receiveChannelMapDict == IURCMD_INVALID)
    {
        IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_HDF5, IUF_ERR_MIN_HDF5, "could not load receiveChannelMap from %s", instance->pFilename);
        return IUIFI_INVALID;
    }

    instance->transmitApodizationDict = iufTransmitApodizationDictLoad(instance->handle);
    if (instance->transmitApodizationDict == IUTAD_INVALID)
    {
        IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_HDF5, IUF_ERR_MIN_HDF5, "could not load transmitApodizationDict from %s", instance->pFilename);
        return IUIFI_INVALID;
    }

    instance->receiveSettingsDict = iufReceiveSettingsDictLoad(instance->handle);
    if (instance->receiveSettingsDict == IURSD_INVALID)
    {
        IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_HDF5, IUF_ERR_MIN_HDF5, "could not load receiveSettingsDict from %s", instance->pFilename);
        return IUIFI_INVALID;
    }

    instance->acquisition = iufAcquisitionLoad(instance->handle);
    if (instance->acquisition == IUA_INVALID)
    {
        IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_HDF5, IUF_ERR_MIN_HDF5, "could not load acquisition from %s", instance->pFilename);
        return IUIFI_INVALID;
    }

    instance->transducer = iufTransducerLoad(instance->handle);
    if (instance->transducer == IUT_INVALID)
    {
        IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_HDF5, IUF_ERR_MIN_HDF5, "could not load transducer from %s", instance->pFilename);
        return IUIFI_INVALID;
    }

    int status = iufHdf5ReadInt( instance->handle, IUF_INPUTFILE_PATH_IUFVERSION, &(instance->IufVersion));
    if( status != IUF_E_OK )
    {
        IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_HDF5, IUF_ERR_MIN_HDF5, "could not load IufVersion from %s", instance->pFilename);
        return IUIFI_INVALID;
    }

    instance->deepDelete = IUF_TRUE;
    return instance;
}

void *iufInputFileInstanceLoad
(
    hid_t handle
)
{
    iuifi_t instance = inputFileInstanceLoad(handle);
    if( instance != IUIFI_INVALID )
    {
        instance->deepDelete = IUF_TRUE;
    }
    return (void *)instance;
}

//iuhn_t iufInputFileLoadNode
//(
//    hid_t handle
//)
//{
//    iuhn_t node = iufHistoryNodeCreate(IUF_INPUT_TYPE);
//    iuifi_t instance = inputFileInstanceLoad(handle);
//    iufHistoryNodeSetInstanceData(node,instance);
//    return node;
//}

iuif_t iufInputFileNodeLoad
(
    const char *pFilename
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(pFilename, IUIF_INVALID);

    // open  Hdf5 file using default properties.
    hid_t handle = H5Fopen( pFilename, H5F_ACC_RDONLY, H5P_DEFAULT );
    if (handle<=0)
    {
        IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_HDF5, IUF_ERR_MIN_HDF5, "could not open file %s", pFilename);
        return IUIF_INVALID;
    }

    iuif_t inputFile = (iuif_t) iufHistoryNodeLoad(handle);
    iuifi_t instance = iufHistoryNodeGetInstanceData((iuhn_t)inputFile);
    if (instance == IUIFI_INVALID)
    {
        IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_HDF5, IUF_ERR_MIN_HDF5, "could not get instance data %s", pFilename);
        return IUIF_INVALID;
    }
    instance->pFilename = strdup(pFilename);
    return inputFile;
}

int iufInputFileSaveInstance
(
    hid_t handle,
    iuifi_t instanceData
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(instanceData, IUF_ERR_VALUE);
    herr_t status=0;
    status |= iufFrameListSave(instanceData->frameList, handle);
    status |= iufPatternListDictSave(instanceData->patternListDict, handle);
    status |= iufPulseDictSave(instanceData->pulseDict, handle);
    status |= iufSourceDictSave(instanceData->pulseSourceDict, handle);
    status |= iufReceiveChannelMapDictSave(instanceData->receiveChannelMapDict, handle);
    status |= iufTransmitApodizationDictSave(instanceData->transmitApodizationDict, handle);
    status |= iufReceiveSettingsDictSave(instanceData->receiveSettingsDict, handle);

    status |= iufAcquisitionSave(instanceData->acquisition, handle);
    status |= iufTransducerSave(instanceData->transducer, instanceData->handle);
    status |= iufHdf5WriteInt( instanceData->handle, IUF_INPUTFILE_PATH_IUFVERSION, &(instanceData->IufVersion), 1);
    return status;
}


int iufInputFileNodeSave
(
    iuif_t inputFile
)
{
    herr_t status=0;
    IUF_ERR_CHECK_NULL_N_RETURN(inputFile, IUF_ERR_VALUE);
    iuifi_t instance = iufHistoryNodeGetInstanceData((iuhn_t)inputFile);
    status |= iufHistoryNodeSave(inputFile, instance->handle);
    return status;
}


int iufInputFileClose
(
    iuif_t inputFile
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(inputFile, IUF_ERR_VALUE);
    iuifi_t instance = iufHistoryNodeGetInstanceData((iuhn_t)inputFile);
    IUF_ERR_EVAL_N_RETURN(instance->handle  == H5I_INVALID_HID, IUF_ERR_VALUE);

    // Terminate access to the file.
    int status = H5Fclose(instance->handle);
    instance->handle = H5I_INVALID_HID;
    return status;
}

int iufInputFileCompareInstance
(
    iuifi_t reference,
    iuifi_t actual
)
{
    if ( reference == actual ) return IUF_TRUE;
    if ( reference == NULL || actual == NULL ) return IUF_FALSE;
    if ( reference->IufVersion != actual->IufVersion ) return IUF_FALSE;
    if ( strcmp(reference->pFilename, actual->pFilename) != 0 ) return IUF_FALSE;
    if ( iufFrameListCompare(reference->frameList, actual->frameList)  == IUF_FALSE ) return IUF_FALSE;
    if ( iufPatternListDictCompare(reference->patternListDict, actual->patternListDict)  == IUF_FALSE ) return IUF_FALSE;
    if ( iufPulseDictCompare(reference->pulseDict, actual->pulseDict)  == IUF_FALSE ) return IUF_FALSE;
    if ( iufSourceDictCompare(reference->pulseSourceDict, actual->pulseSourceDict)  == IUF_FALSE ) return IUF_FALSE;
    if ( iufReceiveChannelMapDictCompare(reference->receiveChannelMapDict, actual->receiveChannelMapDict) == IUF_FALSE ) return IUF_FALSE;
    if ( iufTransmitApodizationDictCompare(reference->transmitApodizationDict, actual->transmitApodizationDict)  == IUF_FALSE ) return IUF_FALSE;
    if ( iufReceiveSettingsDictCompare(reference->receiveSettingsDict, actual->receiveSettingsDict)  == IUF_FALSE ) return IUF_FALSE;
    if ( iufTransducerCompare(reference->transducer, actual->transducer) == IUF_FALSE ) return IUF_FALSE;
    if ( iufAcquisitionCompare(reference->acquisition, actual->acquisition) == IUF_FALSE ) return IUF_FALSE;
    return IUF_TRUE;
}

int iufInputFileCompare
(
    iuif_t reference,
    iuif_t actual
)
{
    if ( reference == actual ) return IUF_TRUE;
    if ( reference == NULL || actual == NULL ) return IUF_FALSE;
    if ( iufHistoryNodeCompareWithId(reference, actual)  == IUF_FALSE ) return IUF_FALSE;

    // Compare instance data
    iuifi_t refInstance = iufHistoryNodeGetInstanceData((iuhn_t)reference);
    iuifi_t actInstance = iufHistoryNodeGetInstanceData((iuhn_t)actual);
    IUF_BOOL equal = iufInputFileCompareInstance(refInstance,actInstance);
    if (equal == IUF_FALSE) return IUF_FALSE;
    return IUF_TRUE;
}

// Getters
iufl_t iufInputFileGetFrameList
(
    iuif_t inputFile
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(inputFile, IUFL_INVALID);
    iuifi_t instance = iufHistoryNodeGetInstanceData((iuhn_t)inputFile);
    return instance->frameList;
}

iupald_t iufInputFileGetPatternListDict
(
    iuif_t inputFile
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(inputFile, IUPALD_INVALID);
    iuifi_t instance = iufHistoryNodeGetInstanceData((iuhn_t)inputFile);
    return instance->patternListDict;
}


iupd_t iufInputFileGetPulseDict
(
    iuif_t inputFile
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(inputFile, IUPD_INVALID);
    iuifi_t instance = iufHistoryNodeGetInstanceData((iuhn_t)inputFile);
    return instance->pulseDict;
}

iusd_t iufInputFileGetSourceDict
(
    iuif_t inputFile
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(inputFile, IUFD_INVALID);
    iuifi_t instance = iufHistoryNodeGetInstanceData((iuhn_t)inputFile);
    return instance->pulseSourceDict;
}


iurcmd_t iufInputFileGetReceiveChannelMapDict
(
    iuif_t inputFile
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(inputFile, IURCMD_INVALID);
    iuifi_t instance = iufHistoryNodeGetInstanceData((iuhn_t)inputFile);
    return instance->receiveChannelMapDict;
}

iutad_t iufInputFileGetTransmitApodizationDict
(
    iuif_t inputFile
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(inputFile, IUTAD_INVALID);
    iuifi_t instance = iufHistoryNodeGetInstanceData((iuhn_t)inputFile);
    return instance->transmitApodizationDict;
}

iursd_t iufInputFileGetReceiveSettingsDict
(
    iuif_t inputFile
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(inputFile, IURSD_INVALID);
    iuifi_t instance = iufHistoryNodeGetInstanceData((iuhn_t)inputFile);
    return instance->receiveSettingsDict;
}

iua_t iufInputFileGetAcquisition
(
   iuif_t inputFile
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(inputFile, IUA_INVALID);
    iuifi_t instance = iufHistoryNodeGetInstanceData((iuhn_t)inputFile);
    return instance->acquisition;
}

iut_t iufInputFileGetTransducer
(
    iuif_t inputFile
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(inputFile, IUT_INVALID);
    iuifi_t instance = iufHistoryNodeGetInstanceData((iuhn_t)inputFile);
    return instance->transducer;
}



// Setters
int iufInputFileSetFrameList
(
    iuif_t inputFile,
    iufl_t frameList
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(inputFile, IUF_ERR_VALUE);
    IUF_ERR_CHECK_NULL_N_RETURN(frameList, IUF_ERR_VALUE);
    iuifi_t instance = iufHistoryNodeGetInstanceData((iuhn_t)inputFile);
    instance->frameList = frameList;
    return IUF_E_OK;
}

int iufInputFileSetPatternListDict
(
    iuif_t inputFile,
    iupald_t patternListDict
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(inputFile, IUF_ERR_VALUE);
    IUF_ERR_CHECK_NULL_N_RETURN(patternListDict, IUF_ERR_VALUE);
    iuifi_t instance = iufHistoryNodeGetInstanceData((iuhn_t)inputFile);
    instance->patternListDict = patternListDict;
    return IUF_E_OK;
}

int iufInputFileSetPulseDict
(
    iuif_t inputFile,
    iupd_t pulseDict
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(inputFile, IUF_ERR_VALUE);
    IUF_ERR_CHECK_NULL_N_RETURN(pulseDict, IUF_ERR_VALUE);
    iuifi_t instance = iufHistoryNodeGetInstanceData((iuhn_t)inputFile);
    instance->pulseDict = pulseDict;
    return IUF_E_OK;
}



int iufInputFileSetSourceDict
(
    iuif_t inputFile,
    iusd_t sourceDict
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(inputFile, IUF_ERR_VALUE);
    IUF_ERR_CHECK_NULL_N_RETURN(sourceDict, IUF_ERR_VALUE);
    iuifi_t instance = iufHistoryNodeGetInstanceData((iuhn_t)inputFile);
    instance->pulseSourceDict = sourceDict;
    return IUF_E_OK;
}

int iufInputFileSetReceiveChannelMapDict
(
    iuif_t inputFile,
    iurcmd_t receiveChannelMapDict
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(inputFile, IUF_ERR_VALUE);
    IUF_ERR_CHECK_NULL_N_RETURN(receiveChannelMapDict, IUF_ERR_VALUE);
    iuifi_t instance = iufHistoryNodeGetInstanceData((iuhn_t)inputFile);
    instance->receiveChannelMapDict = receiveChannelMapDict;
    return IUF_E_OK;
}

int iufInputFileSetTransmitApodizationDict
(
    iuif_t inputFile,
    iutad_t transmitApodizationDict
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(inputFile, IUF_ERR_VALUE);
    IUF_ERR_CHECK_NULL_N_RETURN(transmitApodizationDict, IUF_ERR_VALUE);
    iuifi_t instance = iufHistoryNodeGetInstanceData((iuhn_t)inputFile);
    instance->transmitApodizationDict = transmitApodizationDict;
    return IUF_E_OK;
}

int iufInputFileSetReceiveSettingsDict
(
    iuif_t inputFile,
    iursd_t receiveSettingsDict
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(inputFile, IUF_ERR_VALUE);
    IUF_ERR_CHECK_NULL_N_RETURN(receiveSettingsDict, IUF_ERR_VALUE);
    iuifi_t instance = iufHistoryNodeGetInstanceData((iuhn_t)inputFile);
    instance->receiveSettingsDict = receiveSettingsDict;
    return IUF_E_OK;
}


int iufInputFileSetAcquisition
(
    iuif_t inputFile,
    iua_t acquisition
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(inputFile, IUF_ERR_VALUE);
    IUF_ERR_CHECK_NULL_N_RETURN(acquisition, IUF_ERR_VALUE);
    iuifi_t instance = iufHistoryNodeGetInstanceData((iuhn_t)inputFile);
    instance->acquisition = acquisition;
    return IUF_E_OK;
}


int iufInputFileSetTransducer
(
    iuif_t inputFile,
    iut_t transducer
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(inputFile, IUF_ERR_VALUE);
    IUF_ERR_CHECK_NULL_N_RETURN(transducer, IUF_ERR_VALUE);
    iuifi_t instance = iufHistoryNodeGetInstanceData((iuhn_t)inputFile);
    instance->transducer = transducer;
    return IUF_E_OK;
}

int iufInputFileSetFilename
(
    iuif_t inputFile,
    char * fileName
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(inputFile, IUF_ERR_VALUE);
    IUF_ERR_CHECK_NULL_N_RETURN(fileName, IUF_ERR_VALUE);
    iuifi_t instance = iufHistoryNodeGetInstanceData((iuhn_t)inputFile);
    instance->pFilename = strdup(fileName);
    return IUF_E_OK;
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

static hid_t iufInputFileGetWriteSpace
(
    iuif_t inputFile,
    char *label,
    int numDims
)
{
    hsize_t rfDataDims[4];
    hsize_t chunkDims[4];
    hid_t space, dataChunkConfig;

    IUF_ERR_CHECK_NULL_N_RETURN(inputFile, H5I_INVALID_HID);
    IUF_ERR_CHECK_NULL_N_RETURN(label, H5I_INVALID_HID);

    iuifi_t instance = iufHistoryNodeGetInstanceData((iuhn_t)inputFile);
    iuds_t dataStream = iufDataStreamDictGet(instance->dataStreamDict,label);
    if ( dataStream == IUDS_INVALID)
    {
        iufErrorLogClear();
        // Entry does not exist, create
        rfDataDims[0] = (hsize_t) iufInputFileGetNumChannels(inputFile,label);    // colums in memory
        rfDataDims[1] = (hsize_t) iufInputFileGetSamplesPerLine(inputFile,label); // rows in memory
        rfDataDims[2] = (hsize_t) iufInputFileGetNumResponses(inputFile,label);
        rfDataDims[3] = (hsize_t) iufInputFileGetNumFrames(inputFile);

        fillChunkDims(chunkDims,rfDataDims,numDims,4);

        dataStream = iufDataStreamCreate();

        dataChunkConfig = H5Pcreate(H5P_DATASET_CREATE);
        H5Pset_chunk(dataChunkConfig, 4, chunkDims);
        dataStream->fileChunkConfig = dataChunkConfig;

        space = H5Screate_simple(4, rfDataDims, NULL);
        dataStream->rfDataset = H5Dcreate(instance->handle, label, H5T_NATIVE_FLOAT, space, H5P_DEFAULT, dataChunkConfig, H5P_DEFAULT);
        H5Sclose(space);
        iufDataStreamDictSet(instance->dataStreamDict,label,dataStream);
    }
    return H5Dget_space(dataStream->rfDataset);
}


hid_t iufInputFileGetReadSpace
(
    iuif_t inputFile,
    char *label
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(inputFile, H5I_INVALID_HID);
    IUF_ERR_CHECK_NULL_N_RETURN(label, H5I_INVALID_HID);
    iuifi_t instance = iufHistoryNodeGetInstanceData((iuhn_t)inputFile);
    iuds_t dataStream = iufDataStreamDictGet(instance->dataStreamDict,label);
    if ( dataStream == IUDS_INVALID)
    {
        dataStream = iufDataStreamCreate();
        dataStream->rfDataset = H5Dopen(instance->handle, label, H5P_DEFAULT);
        iufDataStreamDictSet(instance->dataStreamDict,label,dataStream);
    }
    return H5Dget_space(dataStream->rfDataset);
}


int iufInputFileFrameSave
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

    IUF_ERR_CHECK_NULL_N_RETURN(inputFile, H5I_INVALID_HID);
    IUF_ERR_CHECK_NULL_N_RETURN(label, H5I_INVALID_HID);
    IUF_ERR_CHECK_NULL_N_RETURN(frame, H5I_INVALID_HID);
    IUF_ERR_CHECK_NULL_N_RETURN(frame_offset, H5I_INVALID_HID);

    memdim[0] = (hsize_t) iufInputFileGetNumChannels(inputFile,label);
    memdim[1] = (hsize_t) iufInputFileGetSamplesPerLine(inputFile,label);
    memdim[2] = (hsize_t) iufInputFileGetNumResponses(inputFile,label);
    memspace = H5Screate_simple(3, memdim, NULL);

    offset[0] = (hsize_t) frame_offset->x;
    offset[1] = (hsize_t) frame_offset->z;
    offset[2] = (hsize_t) frame_offset->y;
    offset[3] = (hsize_t) frame_offset->t;

    count[0] = memdim[0];
    count[1] = memdim[1];
    count[2] = memdim[2];
    count[3] = 1;

    iuifi_t instance = iufHistoryNodeGetInstanceData((iuhn_t)inputFile);
    dataspace = iufInputFileGetWriteSpace(inputFile, label, 3);
    status = H5Sselect_hyperslab(dataspace, H5S_SELECT_SET, offset, NULL, count, NULL);

    float *pFrame = iufDataGetPointer(frame);

    // Save frame
    iuds_t dataStream = iufDataStreamDictGet(instance->dataStreamDict,label);
    status |= H5Dwrite(dataStream->rfDataset, H5T_NATIVE_FLOAT, memspace, dataspace, H5P_DEFAULT, pFrame);

    // Close and release memspace but not (file)dataspace
    status |= H5Sclose(memspace);
    status |= H5Sclose(dataspace);

    return status;
}



int iufInputFileResponseSave
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

    IUF_ERR_CHECK_NULL_N_RETURN(inputFile, H5I_INVALID_HID);
    IUF_ERR_CHECK_NULL_N_RETURN(label, H5I_INVALID_HID);
    IUF_ERR_CHECK_NULL_N_RETURN(response, H5I_INVALID_HID);
    IUF_ERR_CHECK_NULL_N_RETURN(response_offset, H5I_INVALID_HID);

    memdim[0] = (hsize_t) iufInputFileGetNumChannels(inputFile,label);
    memdim[1] = (hsize_t) iufInputFileGetSamplesPerLine(inputFile,label);
    memspace = H5Screate_simple(2, memdim, NULL);

    offset[0] = (hsize_t) response_offset->x;
    offset[1] = (hsize_t) response_offset->z;
    offset[2] = (hsize_t) response_offset->y;
    offset[3] = (hsize_t) response_offset->t;

    count[0] = memdim[0];
    count[1] = memdim[1];
    count[2] = 1;
    count[3] = 1;

    iuifi_t instance = iufHistoryNodeGetInstanceData((iuhn_t)inputFile);
    dataspace = iufInputFileGetWriteSpace(inputFile, label, 2);
    status = H5Sselect_hyperslab(dataspace, H5S_SELECT_SET, offset, NULL, count, NULL);

    float *pFrame = iufDataGetPointer(response);

    // Save frame
    iuds_t dataStream = iufDataStreamDictGet(instance->dataStreamDict,label);
    status |= H5Dwrite(dataStream->rfDataset, H5T_NATIVE_FLOAT, memspace, dataspace, H5P_DEFAULT, pFrame);

    // Close and release memspace but not (file)dataspace
    status |= H5Sclose(memspace);
    status |= H5Sclose(dataspace);

    return status;
}

int iufInputFileChannelSave
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

    IUF_ERR_CHECK_NULL_N_RETURN(inputFile, H5I_INVALID_HID);
    IUF_ERR_CHECK_NULL_N_RETURN(label, H5I_INVALID_HID);
    IUF_ERR_CHECK_NULL_N_RETURN(channel, H5I_INVALID_HID);
    IUF_ERR_CHECK_NULL_N_RETURN(channel_offset, H5I_INVALID_HID);

    memdim = (hsize_t) iufInputFileGetNumChannels(inputFile,label);
    memspace = H5Screate_simple(1, &memdim, NULL);

    offset[0] = (hsize_t) channel_offset->x;
    offset[1] = (hsize_t) channel_offset->z;
    offset[2] = (hsize_t) channel_offset->y;
    offset[3] = (hsize_t) channel_offset->t;

    count[0] = memdim;
    count[1] = 1;
    count[2] = 1;
    count[3] = 1;

    iuifi_t instance = iufHistoryNodeGetInstanceData((iuhn_t)inputFile);
    dataspace = iufInputFileGetWriteSpace(inputFile, label, 1);
    status = H5Sselect_hyperslab(dataspace, H5S_SELECT_SET, offset, NULL, count, NULL);

    float *pFrame = iufDataGetPointer(channel);

    // Save frame
    iuds_t dataStream = iufDataStreamDictGet(instance->dataStreamDict,label);
    status |= H5Dwrite(dataStream->rfDataset, H5T_NATIVE_FLOAT, memspace, dataspace, H5P_DEFAULT, pFrame);

    // Close and release memspace but not (file)dataspace
    status |= H5Sclose(memspace);
    status |= H5Sclose(dataspace);

    return status;
}



int iufInputFileFrameLoad
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

    IUF_ERR_CHECK_NULL_N_RETURN(inputFile, H5I_INVALID_HID);
    IUF_ERR_CHECK_NULL_N_RETURN(label, H5I_INVALID_HID);
    IUF_ERR_CHECK_NULL_N_RETURN(frame, H5I_INVALID_HID);
    IUF_ERR_CHECK_NULL_N_RETURN(frame_offset, H5I_INVALID_HID);

    memdim[0] = (hsize_t) iufInputFileGetNumChannels(inputFile,label);
    memdim[1] = (hsize_t) iufInputFileGetSamplesPerLine(inputFile,label);
    memdim[2] = (hsize_t) iufInputFileGetNumResponses(inputFile,label);
    memspace = H5Screate_simple(3, memdim, NULL);

    offset[0] = (hsize_t) frame_offset->x;
    offset[1] = (hsize_t) frame_offset->z;
    offset[2] = (hsize_t) frame_offset->y;
    offset[3] = (hsize_t) frame_offset->t;

    count[0] = memdim[0];
    count[1] = memdim[1];
    count[2] = memdim[2];
    count[3] = 1;

    iuifi_t instance = iufHistoryNodeGetInstanceData((iuhn_t)inputFile);
    dataspace = iufInputFileGetReadSpace(inputFile, label);
    status = H5Sselect_hyperslab(dataspace, H5S_SELECT_SET, offset, NULL, count, NULL);

    float *pFrame = iufDataGetPointer(frame);

    // Load frame
    iuds_t dataStream = iufDataStreamDictGet(instance->dataStreamDict,label);
    status |= H5Dread(dataStream->rfDataset, H5T_NATIVE_FLOAT, memspace, dataspace, H5P_DEFAULT, pFrame);

    // Close and release memspace but not (file)dataspace
    status |= H5Sclose(memspace);
    status |= H5Sclose(dataspace);

    return status;

}


int iufInputFileResponseLoad
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

    IUF_ERR_CHECK_NULL_N_RETURN(inputFile, H5I_INVALID_HID);
    IUF_ERR_CHECK_NULL_N_RETURN(label, H5I_INVALID_HID);
    IUF_ERR_CHECK_NULL_N_RETURN(response, H5I_INVALID_HID);
    IUF_ERR_CHECK_NULL_N_RETURN(response_offset, H5I_INVALID_HID);

    memdim[0] = (hsize_t) iufInputFileGetNumChannels(inputFile,label);
    memdim[1] = (hsize_t) iufInputFileGetSamplesPerLine(inputFile,label);
    memspace = H5Screate_simple(2, memdim, NULL);

    offset[0] = (hsize_t) response_offset->x;
    offset[1] = (hsize_t) response_offset->z;
    offset[2] = (hsize_t) response_offset->y;
    offset[3] = (hsize_t) response_offset->t;

    count[0] = memdim[0];
    count[1] = memdim[1];
    count[2] = 1;
    count[3] = 1;

    iuifi_t instance = iufHistoryNodeGetInstanceData((iuhn_t)inputFile);
    dataspace = iufInputFileGetReadSpace(inputFile, label);
    status = H5Sselect_hyperslab(dataspace, H5S_SELECT_SET, offset, NULL, count, NULL);

    float *pResponse = iufDataGetPointer(response);

    // Load frame
    iuds_t dataStream = iufDataStreamDictGet(instance->dataStreamDict,label);
    status |= H5Dread(dataStream->rfDataset, H5T_NATIVE_FLOAT, memspace, dataspace, H5P_DEFAULT, pResponse);

    // Close and release memspace but not (file)dataspace
    status |= H5Sclose(memspace);
    status |= H5Sclose(dataspace);

    return status;

}


int iufInputFileChannelLoad
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

    IUF_ERR_CHECK_NULL_N_RETURN(inputFile, H5I_INVALID_HID);
    IUF_ERR_CHECK_NULL_N_RETURN(label, H5I_INVALID_HID);
    IUF_ERR_CHECK_NULL_N_RETURN(channel, H5I_INVALID_HID);
    IUF_ERR_CHECK_NULL_N_RETURN(channel_offset, H5I_INVALID_HID);

    memdim = (hsize_t) iufInputFileGetNumChannels(inputFile,label);
    memspace = H5Screate_simple(1, &memdim, NULL);

    offset[0] = (hsize_t) channel_offset->x;
    offset[1] = (hsize_t) channel_offset->z;
    offset[2] = (hsize_t) channel_offset->y;
    offset[3] = (hsize_t) channel_offset->t;

    count[0] = memdim;
    count[1] = 1;
    count[2] = 1;
    count[3] = 1;

    iuifi_t instance = iufHistoryNodeGetInstanceData((iuhn_t)inputFile);
    dataspace = iufInputFileGetReadSpace(inputFile, label);
    status = H5Sselect_hyperslab(dataspace, H5S_SELECT_SET, offset, NULL, count, NULL);

    float *pFrame = iufDataGetPointer(channel);

    // Load channel
    iuds_t dataStream = iufDataStreamDictGet(instance->dataStreamDict,label);
    status |= H5Dread(dataStream->rfDataset, H5T_NATIVE_FLOAT, memspace, dataspace, H5P_DEFAULT, pFrame);

    // Close and release memspace but not (file)dataspace
    status |= H5Sclose(memspace);
    status |= H5Sclose(dataspace);

    return status;
}
