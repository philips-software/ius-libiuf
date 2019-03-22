
//
// Created by nlv09165 on 11/07/2018.
//
#include <stdlib.h>
#include <string.h>

#include <iuf.h>
#include <iufAcquisitionPrivate.h>
#include <iufDataStreamDictPrivate.h>
#include <iufFrameListPrivate.h>
#include <iufHistoryNodePrivate.h>
#include <iufIqFilePrivate.h>
#include <iufIqFileStructure.h>
#include <iufIqPatternListDictPrivate.h>
#include <iufPulseDictPrivate.h>
#include <iufReceiveChannelMapDictPrivate.h>
#include <iufDemodulationDictPrivate.h>
#include <iufSourceDictPrivate.h>
#include <iufTransducerPrivate.h>
#include <iufTransmitApodizationDictPrivate.h>

struct IufIqFileInstance
{
    iufl_t frameList;
	iuiqpald_t iqPatternListDict;            /**< a dictionary of pattern lists              */
    iupd_t pulseDict;                    /**< a dictionary of pulses                      */
    iusd_t pulseSourceDict;              /**< a dictionary of sources                     */
	iurcmd_t receiveChannelMapDict;      /**< a dictionary of receiveChannelMaps          */
	iutad_t transmitApodizationDict;     /**< a dictionary of transmitApodizations        */
	iudmd_t demodulationDict;            /**< a dictionary of receiveSettings             */
	iut_t transducer;                    /**< The transducer description                  */
	iua_t acquisition;                    /**< The description of the performed acquisition */
    int IufVersion;                      /**< The version of iq file format */

    //  state variables
    hid_t               handle;           /**< HDF5 file handle     */
    const char          *pFilename;       /**< the filename         */
    iudsd_t             iDataStreamDict;   /**< Contains dataset administration */
    iudsd_t             qDataStreamDict;   /**< Contains dataset administration */
    IUF_BOOL            deepDelete;
};

struct IufIqFile
{
    iuhn_t history;
};

iuiqfi_t iufIqFileInstanceCreate
(
    void
)
{
    iuiqfi_t instanceData = (iuiqfi_t) calloc(1, sizeof(IufIqFileInstance));
    IUF_ERR_ALLOC_NULL_N_RETURN(instanceData, IufIqFileInstance, IUIQFI_INVALID);

	instanceData->IufVersion = iufGetVersionMajor();
	instanceData->pFilename = "";
	instanceData->frameList = IUFL_INVALID;
	instanceData->iqPatternListDict = IUIQPALD_INVALID;
	instanceData->pulseDict = IUPD_INVALID;
    instanceData->pulseSourceDict = IUFD_INVALID;
	instanceData->receiveChannelMapDict = IURCMD_INVALID;
    instanceData->transmitApodizationDict = IUTAD_INVALID;
    instanceData->demodulationDict = IUDMD_INVALID;
    instanceData->transducer = IUT_INVALID;
	instanceData->acquisition = IUA_INVALID;
	instanceData->iDataStreamDict = iufDataStreamDictCreate();
    instanceData->qDataStreamDict = iufDataStreamDictCreate();
    return instanceData;
}

int iufIqFileInstanceDelete
(
    iuiqfi_t instance
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(instance, IUF_ERR_VALUE);
    iufDataStreamDictDelete(instance->iDataStreamDict);
    iufDataStreamDictDelete(instance->qDataStreamDict);
    free((void *)instance->pFilename);
    if (instance->deepDelete == IUF_TRUE)
    {
        iufFrameListDelete(instance->frameList);
        iufAcquisitionDelete(instance->acquisition);
        iufTransducerDelete(instance->transducer);
        iufDemodulationDictDelete(instance->demodulationDict);
        iufTransmitApodizationDictDelete(instance->transmitApodizationDict);
        iufReceiveChannelMapDictDelete(instance->receiveChannelMapDict);
        iufSourceDictDelete(instance->pulseSourceDict);
        iufPulseDictDelete(instance->pulseDict);
        iufIqPatternListDictDelete(instance->iqPatternListDict);
    }
    free(instance);
    return IUF_E_OK;
}

// ADT
int iufIqFileDelete
(
    iuiqf_t iqFile
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(iqFile, IUF_ERR_VALUE);
    iuiqfi_t instance = iufHistoryNodeGetInstanceData(iqFile);
    iufIqFileInstanceDelete(instance);
    iufHistoryNodeDelete((iuhn_t)iqFile);
    return IUF_E_OK;
}

iuiqf_t iufIqFileCreate
(
    const char *filename
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(filename, IUIQF_INVALID);
	iuiqfi_t instanceData = iufIqFileInstanceCreate();
	if (instanceData == IUIQFI_INVALID)
	{
        IUF_ERROR_PUSH(IUF_ERR_MAJ_MEMORY, IUF_ERR_MIN_ALLOC,  "calloc of IufIqFileInstance failed");
		return IUIQF_INVALID;
	}

    instanceData->pFilename = strdup(filename);
    instanceData->handle = H5Fcreate(filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
	if (instanceData->handle == H5I_INVALID_HID)
	{
		IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_ERROR, IUF_ERR_MIN_HDF5, "unable to create file %s", filename);
        iufIqFileInstanceDelete(instanceData);
		return IUIQF_INVALID;
	}
	iuhn_t node = iufHistoryNodeCreate(IUF_IQ_TYPE);
	iufHistoryNodeSetInstanceData(node,(void *)instanceData);
	return (iuiqf_t)node;
}


int iufIqFileGetNumFrames
(
    iuiqf_t iqFile
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(iqFile, -1);
    iuiqfi_t instance = iufHistoryNodeGetInstanceData((iuhn_t)iqFile);
    if (instance->frameList == IUFL_INVALID) return 0;
    return iufFrameListGetSize(instance->frameList);
}

int iufIqFileGetNumChannels
(
    iuiqf_t iqFile,
    char *label
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(iqFile, -1);
    IUF_ERR_CHECK_NULL_N_RETURN(label, -1);
    iurcmd_t receiveChannelMapDict = iufIqFileGetReceiveChannelMapDict(iqFile);
    if (receiveChannelMapDict == IURCMD_INVALID) return -1;
    iurcm_t receiveChannelMap = iufReceiveChannelMapDictGet(receiveChannelMapDict,label);
    if (receiveChannelMap == IURCM_INVALID) return -1;
    return iufReceiveChannelMapGetNumChannels(receiveChannelMap);
}

int iufIqFileGetSamplesPerLine
(
    iuiqf_t iqFile,
    char *label
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(iqFile, -1);
    IUF_ERR_CHECK_NULL_N_RETURN(label, -1);

    iudmd_t demodulationDict = iufIqFileGetDemodulationDict(iqFile);
    if (demodulationDict == IUDMD_INVALID) return -1;
    iudm_t demodulation = iufDemodulationDictGet(demodulationDict,label);
    if (demodulation == IUDM_INVALID) return -1;
    return iufDemodulationGetNumSamplesPerLine(demodulation);
}

int iufIqFileGetNumResponses
(
	iuiqf_t iqFile,          
	char *label              
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(iqFile, -1);
    IUF_ERR_CHECK_NULL_N_RETURN(label, -1);

    iuiqpald_t listDict = iufIqFileGetIqPatternListDict(iqFile);
    if (listDict == IUIQPALD_INVALID) return -1;
    iuiqpal_t iqPatternList = iufIqPatternListDictGet(listDict,label);
    if (iqPatternList == IUIQPAL_INVALID) return -1;
    return iufIqPatternListGetSize(iqPatternList);
}

iud_t iufIqFileFrameCreate
(
    iuiqf_t iqFile,
    char *label
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(iqFile, IUD_INVALID);
    IUF_ERR_CHECK_NULL_N_RETURN(label, IUD_INVALID);
    // calculate frame size:
    // numchannels * numresponses * numsamples = [x*y*z]
    int numChannels = iufIqFileGetNumChannels(iqFile,label);
    int numResponses = iufIqFileGetNumResponses(iqFile,label);
    int numSamples = iufIqFileGetSamplesPerLine(iqFile,label);
    int frameSize = numChannels * numSamples * numResponses;
    return iufDataCreate(frameSize);
}

iud_t iufIqFileResponseCreate
(
    iuiqf_t iqFile,
    char *label
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(iqFile, IUD_INVALID);
    IUF_ERR_CHECK_NULL_N_RETURN(label, IUD_INVALID);

    int numChannels = iufIqFileGetNumChannels(iqFile,label);
    int numSamples = iufIqFileGetSamplesPerLine(iqFile,label);
    int responseSize = numChannels * numSamples ;
    return iufDataCreate(responseSize);
}

iud_t iufIqFileChannelCreate
(
    iuiqf_t iqFile,
    char *label
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(iqFile, IUD_INVALID);
    IUF_ERR_CHECK_NULL_N_RETURN(label, IUD_INVALID);
    int numSamples = iufIqFileGetSamplesPerLine(iqFile,label);
    return iufDataCreate(numSamples);
}


static iuiqfi_t iqFileInstanceLoad
(
    iuiqfi_t instance
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(instance, IUIQFI_INVALID);
    instance->frameList = iufFrameListLoad(instance->handle);
    if (instance->frameList == IUFL_INVALID)
    {
        IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_HDF5, IUF_ERR_MIN_HDF5, "could not load framelist from %s", instance->pFilename);
        return IUIQFI_INVALID;
    }

    instance->iqPatternListDict = iufIqPatternListDictLoad(instance->handle);
    if (instance->iqPatternListDict == IUIQPALD_INVALID)
    {
        IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_HDF5, IUF_ERR_MIN_HDF5, "could not load patterList from %s", instance->pFilename);;
        return IUIQFI_INVALID;
    }

    instance->pulseDict = iufPulseDictLoad(instance->handle);
    if (instance->pulseDict == IUPD_INVALID)
    {
        IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_HDF5, IUF_ERR_MIN_HDF5, "could not load pulses from %s", instance->pFilename);
        return IUIQFI_INVALID;
    }

    instance->pulseSourceDict = iufSourceDictLoad(instance->handle);
    if (instance->pulseSourceDict == IUFD_INVALID)
    {
        IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_HDF5, IUF_ERR_MIN_HDF5, "could not load source from %s", instance->pFilename);
        return IUIQFI_INVALID;
    }

    instance->receiveChannelMapDict = iufReceiveChannelMapDictLoad(instance->handle);
    if (instance->receiveChannelMapDict == IURCMD_INVALID)
    {
        IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_HDF5, IUF_ERR_MIN_HDF5, "could not load receiveChannelMap from %s", instance->pFilename);
        return IUIQFI_INVALID;
    }

    instance->transmitApodizationDict = iufTransmitApodizationDictLoad(instance->handle);
    if (instance->transmitApodizationDict == IUTAD_INVALID)
    {
        IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_HDF5, IUF_ERR_MIN_HDF5, "could not load transmitApodizationDict from %s", instance->pFilename);
        return IUIQFI_INVALID;
    }

    instance->demodulationDict = iufDemodulationDictLoad(instance->handle);
    if (instance->demodulationDict == IUDMD_INVALID)
    {
        IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_HDF5, IUF_ERR_MIN_HDF5, "could not load demodulation from %s", instance->pFilename);
        return IUIQFI_INVALID;
    }

    instance->acquisition = iufAcquisitionLoad(instance->handle);
    if (instance->acquisition == IUA_INVALID)
    {
        IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_HDF5, IUF_ERR_MIN_HDF5, "could not load acquisition from %s", instance->pFilename);
        return IUIQFI_INVALID;
    }

    instance->transducer = iufTransducerLoad(instance->handle);
    if (instance->transducer == IUT_INVALID)
    {
        IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_HDF5, IUF_ERR_MIN_HDF5, "could not load transducer from %s", instance->pFilename);
        return IUIQFI_INVALID;
    }

    int status = iufHdf5ReadInt( instance->handle, IUF_PATH_IUFVERSION, &(instance->IufVersion));
    if( status != IUF_E_OK )
    {
        IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_HDF5, IUF_ERR_MIN_HDF5, "could not load IufVersion from %s", instance->pFilename);
        return IUIQFI_INVALID;
    }

    instance->deepDelete = IUF_TRUE;
    return instance;
}

void *iufIqFileInstanceLoad
(
    hid_t handle
)
{
    iuiqfi_t instance = iufIqFileInstanceCreate();
    iuiqfi_t new_instance;
    instance->handle = handle;
    new_instance = iqFileInstanceLoad(instance);
    if( new_instance == IUIQFI_INVALID )
    {
        iufIqFileInstanceDelete(instance);
        instance = new_instance;
    }
    instance->deepDelete = IUF_TRUE;
    return (void *)instance;
}


iuhn_t iufIqFileLoadNode
(
    hid_t handle
)
{
    iuhn_t node = iufHistoryNodeCreate(IUF_IQ_TYPE);
    iuiqfi_t instance = iufIqFileInstanceCreate();
    instance->handle = handle;
    instance = iqFileInstanceLoad(instance);
    iufHistoryNodeSetInstanceData(node,instance);
    return node;
}

iuiqf_t iufIqFileNodeLoad
(
    const char *pFilename
)
{
	IUF_ERR_CHECK_NULL_N_RETURN(pFilename, IUIQF_INVALID);

    // open  Hdf5 file using default properties.
    hid_t handle = H5Fopen( pFilename, H5F_ACC_RDONLY, H5P_DEFAULT );
    if (handle <=0)
    {
        IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_HDF5, IUF_ERR_MIN_HDF5, "could not open file %s", pFilename);
        return IUIQF_INVALID;
    }

    iuiqf_t iqFile = (iuiqf_t) iufHistoryNodeLoad(handle);
    iuiqfi_t instance = iufHistoryNodeGetInstanceData((iuhn_t)iqFile);
    instance->pFilename = strdup(pFilename);
    return iqFile;
}

int iufIqFileSaveInstance
(
    hid_t handle,
    iuiqfi_t instanceData
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(instanceData, IUF_ERR_VALUE);
    herr_t status=0;
    status |= iufFrameListSave(instanceData->frameList, handle);
    status |= iufIqPatternListDictSave(instanceData->iqPatternListDict, handle);
    status |= iufPulseDictSave(instanceData->pulseDict, handle);
    status |= iufSourceDictSave(instanceData->pulseSourceDict, handle);
    status |= iufReceiveChannelMapDictSave(instanceData->receiveChannelMapDict, handle);
    status |= iufTransmitApodizationDictSave(instanceData->transmitApodizationDict, handle);
    status |= iufDemodulationDictSave(instanceData->demodulationDict, handle);
    status |= iufAcquisitionSave(instanceData->acquisition, handle);
    status |= iufTransducerSave(instanceData->transducer, instanceData->handle);
    status |= iufHdf5WriteInt( instanceData->handle, IUF_PATH_IUFVERSION, &(instanceData->IufVersion), 1);
    return status;
}


int iufIqFileNodeSave
(
    iuiqf_t iqFile
)
{
    herr_t status=0;
    IUF_ERR_CHECK_NULL_N_RETURN(iqFile, IUF_ERR_VALUE);

    iuiqfi_t instance = iufHistoryNodeGetInstanceData((iuhn_t)iqFile);
    status |= iufHistoryNodeSave(iqFile, instance->handle);
	return status;
}


int iufIqFileClose
(
    iuiqf_t iqFile
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(iqFile, IUF_ERR_VALUE);
    iuiqfi_t instance = iufHistoryNodeGetInstanceData((iuhn_t)iqFile);
    IUF_ERR_EVAL_N_RETURN(instance->handle  == H5I_INVALID_HID, IUF_ERR_VALUE);

	int status = H5Fclose(instance->handle);
	instance->handle = H5I_INVALID_HID;
	return status;
}

static int iufIqFileCompareInstance
(
    iuiqfi_t reference,
    iuiqfi_t actual
)
{
    if ( reference == actual ) return IUF_TRUE;
    if ( reference == NULL || actual == NULL ) return IUF_FALSE;
    if ( reference->IufVersion != actual->IufVersion ) return IUF_FALSE;
    if ( strcmp(reference->pFilename, actual->pFilename) != 0 ) return IUF_FALSE;
    if ( iufFrameListCompare(reference->frameList, actual->frameList)  == IUF_FALSE ) return IUF_FALSE;
    if ( iufIqPatternListDictCompare(reference->iqPatternListDict, actual->iqPatternListDict)  == IUF_FALSE ) return IUF_FALSE;
    if ( iufPulseDictCompare(reference->pulseDict, actual->pulseDict)  == IUF_FALSE ) return IUF_FALSE;
    if ( iufSourceDictCompare(reference->pulseSourceDict, actual->pulseSourceDict)  == IUF_FALSE ) return IUF_FALSE;
    if ( iufReceiveChannelMapDictCompare(reference->receiveChannelMapDict, actual->receiveChannelMapDict) == IUF_FALSE) return IUF_FALSE;
    if ( iufTransmitApodizationDictCompare(reference->transmitApodizationDict, actual->transmitApodizationDict)  == IUF_FALSE ) return IUF_FALSE;
    if ( iufDemodulationDictCompare(reference->demodulationDict, actual->demodulationDict)  == IUF_FALSE ) return IUF_FALSE;
    if ( iufTransducerCompare(reference->transducer, actual->transducer) == IUF_FALSE) return IUF_FALSE;
    if (iufAcquisitionCompare(reference->acquisition, actual->acquisition) == IUF_FALSE) return IUF_FALSE;
    return IUF_TRUE;
}

int iufIqFileCompare
(
    iuiqf_t reference,
    iuiqf_t actual
)
{
    if ( reference == actual ) return IUF_TRUE;
    if ( reference == NULL || actual == NULL ) return IUF_FALSE;
    if ( iufHistoryNodeCompareWithId(reference, actual)  == IUF_FALSE ) return IUF_FALSE;

    // Compare instance data
    iuiqfi_t refInstance = iufHistoryNodeGetInstanceData((iuhn_t)reference);
    iuiqfi_t actInstance = iufHistoryNodeGetInstanceData((iuhn_t)actual);
    IUF_BOOL equal = iufIqFileCompareInstance(refInstance,actInstance);
    if (equal == IUF_FALSE) return IUF_FALSE;

    return equal;
}

// Getters
iufl_t iufIqFileGetFrameList
(
    iuiqf_t iqFile
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(iqFile, IUFL_INVALID);
    iuiqfi_t instance = iufHistoryNodeGetInstanceData((iuhn_t)iqFile);
    return instance->frameList;
}

iuiqpald_t iufIqFileGetIqPatternListDict
(
    iuif_t iqFile
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(iqFile, IUIQPALD_INVALID);
    iuiqfi_t instance = iufHistoryNodeGetInstanceData((iuhn_t)iqFile);
    return instance->iqPatternListDict;
}


iupd_t iufIqFileGetPulseDict
(
    iuiqf_t iqFile
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(iqFile, IUPD_INVALID);
    iuiqfi_t instance = iufHistoryNodeGetInstanceData((iuhn_t)iqFile);
    return instance->pulseDict;
}

iusd_t iufIqFileGetSourceDict
(
    iuiqf_t iqFile
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(iqFile, IUFD_INVALID);
    iuiqfi_t instance = iufHistoryNodeGetInstanceData((iuhn_t)iqFile);
    return instance->pulseSourceDict;
}


iurcmd_t iufIqFileGetReceiveChannelMapDict
(
    iuiqf_t iqFile
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(iqFile, IURCMD_INVALID);
    iuiqfi_t instance = iufHistoryNodeGetInstanceData((iuhn_t)iqFile);
    return instance->receiveChannelMapDict;
}

iutad_t iufIqFileGetTransmitApodizationDict
(
    iuiqf_t iqFile
)
{
   IUF_ERR_CHECK_NULL_N_RETURN(iqFile, IUTAD_INVALID);
   iuiqfi_t instance = iufHistoryNodeGetInstanceData((iuhn_t)iqFile);
   return instance->transmitApodizationDict;
}

iudmd_t iufIqFileGetDemodulationDict
(
    iuiqf_t iqFile
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(iqFile, IUDMD_INVALID);
    iuiqfi_t instance = iufHistoryNodeGetInstanceData((iuhn_t)iqFile);
	return instance->demodulationDict;
}

iua_t iufIqFileGetAcquisition
(
   iuiqf_t iqFile
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(iqFile, IUA_INVALID);
    iuiqfi_t instance = iufHistoryNodeGetInstanceData((iuhn_t)iqFile);
    return instance->acquisition;
}

iut_t iufIqFileGetTransducer
(
    iuiqf_t iqFile
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(iqFile, IUT_INVALID);
    iuiqfi_t instance = iufHistoryNodeGetInstanceData((iuhn_t)iqFile);
    return instance->transducer;
}



// Setters
int iufIqFileSetFrameList
(
    iuiqf_t iqFile,
    iufl_t frameList
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(iqFile, IUF_ERR_VALUE);
    IUF_ERR_CHECK_NULL_N_RETURN(frameList, IUF_ERR_VALUE);
    iuiqfi_t instance = iufHistoryNodeGetInstanceData((iuhn_t)iqFile);
    instance->frameList = frameList;
    return IUF_E_OK;
}

int iufIqFileSetPatternListDict
(
    iuiqf_t iqFile,
    iuiqpald_t patternListDict
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(iqFile, IUF_ERR_VALUE);
    IUF_ERR_CHECK_NULL_N_RETURN(patternListDict, IUF_ERR_VALUE);
    iuiqfi_t instance = iufHistoryNodeGetInstanceData((iuhn_t)iqFile);
    instance->iqPatternListDict = patternListDict;
    return IUF_E_OK;
}

int iufIqFileSetPulseDict
(
    iuiqf_t iqFile,
    iupd_t pulseDict
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(iqFile, IUF_ERR_VALUE);
    IUF_ERR_CHECK_NULL_N_RETURN(pulseDict, IUF_ERR_VALUE);
    iuiqfi_t instance = iufHistoryNodeGetInstanceData((iuhn_t)iqFile);
	instance->pulseDict = pulseDict;
    return IUF_E_OK;
}

int iufIqFileSetSourceDict
(
    iuiqf_t iqFile,
    iusd_t sourceDict
)
{
   IUF_ERR_CHECK_NULL_N_RETURN(iqFile, IUF_ERR_VALUE);
   IUF_ERR_CHECK_NULL_N_RETURN(sourceDict, IUF_ERR_VALUE);
   iuiqfi_t instance = iufHistoryNodeGetInstanceData((iuhn_t)iqFile);
   instance->pulseSourceDict = sourceDict;
    return IUF_E_OK;
}

int iufIqFileSetReceiveChannelMapDict
(
	iuiqf_t iqFile,
	iurcmd_t receiveChannelMapDict
)
{
   IUF_ERR_CHECK_NULL_N_RETURN(iqFile, IUF_ERR_VALUE);
   IUF_ERR_CHECK_NULL_N_RETURN(receiveChannelMapDict, IUF_ERR_VALUE);
   iuiqfi_t instance = iufHistoryNodeGetInstanceData((iuhn_t)iqFile);
   instance->receiveChannelMapDict = receiveChannelMapDict;
	return IUF_E_OK;
}

int iufIqFileSetTransmitApodizationDict
(
	iuif_t iqFile,
	iutad_t transmitApodizationDict
)
{
   IUF_ERR_CHECK_NULL_N_RETURN(iqFile, IUF_ERR_VALUE);
   IUF_ERR_CHECK_NULL_N_RETURN(transmitApodizationDict, IUF_ERR_VALUE);
   iuiqfi_t instance = iufHistoryNodeGetInstanceData((iuhn_t)iqFile);
   instance->transmitApodizationDict = transmitApodizationDict;
   return IUF_E_OK;
}

int iufIqFileSetDemodulationDict
(
    iuiqf_t iqFile,
    iudmd_t demodulationDict
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(iqFile, IUF_ERR_VALUE);
    IUF_ERR_CHECK_NULL_N_RETURN(demodulationDict, IUF_ERR_VALUE);
    iuiqfi_t instance = iufHistoryNodeGetInstanceData((iuhn_t)iqFile);
    instance->demodulationDict = demodulationDict;
    return IUF_E_OK;
}


int iufIqFileSetAcquisition
(
    iuiqf_t iqFile,
    iua_t acquisition
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(iqFile, IUF_ERR_VALUE);
    IUF_ERR_CHECK_NULL_N_RETURN(acquisition, IUF_ERR_VALUE);
    iuiqfi_t instance = iufHistoryNodeGetInstanceData((iuhn_t)iqFile);
    instance->acquisition = acquisition;
	return IUF_E_OK;
}


int iufIqFileSetTransducer
(
    iuiqf_t iqFile,
    iut_t transducer
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(iqFile, IUF_ERR_VALUE);
    IUF_ERR_CHECK_NULL_N_RETURN(transducer, IUF_ERR_VALUE);
    iuiqfi_t instance = iufHistoryNodeGetInstanceData((iuhn_t)iqFile);
	instance->transducer = transducer;
    return IUF_E_OK;
}

int iufIqFileSetFilename
(
    iuiqf_t iqFile,
    char * fileName
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(iqFile, IUF_ERR_VALUE);
    IUF_ERR_CHECK_NULL_N_RETURN(fileName, IUF_ERR_VALUE);
    iuiqfi_t instance = iufHistoryNodeGetInstanceData((iuhn_t)iqFile);
    instance->pFilename = strdup(fileName);
    return IUF_E_OK;
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

#define IUF_IQ_COMPONENT_I 0
#define IUF_IQ_COMPONENT_Q 1

static hid_t iufIqFileGetWriteSpace
(
    iuiqf_t iqFile,
    char *label,
    IufComponent component,
    int numDims
)
{
    hsize_t rfDataDims[4];
    hsize_t chunkDims[4];
    hid_t space, dataChunkConfig;
    iuds_t dataStream = IUDS_INVALID;
	int status = 0;

    IUF_ERR_CHECK_NULL_N_RETURN(iqFile, H5I_INVALID_HID);
    IUF_ERR_CHECK_NULL_N_RETURN(label, H5I_INVALID_HID);

    iuiqfi_t instance = iufHistoryNodeGetInstanceData((iuhn_t)iqFile);
    if (component == IUF_IQ_COMPONENT_I)
    {
        dataStream = iufDataStreamDictGet(instance->iDataStreamDict, label);
    }
    else if (component == IUF_IQ_COMPONENT_Q)
    {
        dataStream = iufDataStreamDictGet(instance->qDataStreamDict, label);
    }
    else
    {
        IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_HDF5, IUF_ERR_MIN_HDF5, "wrong component specfic, %d", component);
        return H5I_INVALID_HID;
    }

    if ( dataStream == IUDS_INVALID) //the datastream doesn't exist yet, but maybe the group already does...
    {
        iufErrorLogClear();
        // Entry does not exist, create
        rfDataDims[0] = (hsize_t) iufIqFileGetNumChannels(iqFile,label);    // colums in memory
        rfDataDims[1] = (hsize_t) iufIqFileGetSamplesPerLine(iqFile,label); // rows in memory
        rfDataDims[2] = (hsize_t) iufIqFileGetNumResponses(iqFile,label);
        rfDataDims[3] = (hsize_t) iufIqFileGetNumFrames(iqFile);

        fillChunkDims(chunkDims,rfDataDims,numDims,4);

        dataStream = iufDataStreamCreate();

        dataChunkConfig = H5Pcreate(H5P_DATASET_CREATE);
        H5Pset_chunk(dataChunkConfig, 4, chunkDims);
        dataStream->fileChunkConfig = dataChunkConfig;

		space = H5Screate_simple(4, rfDataDims, NULL);
		
		hid_t dataGroup;
		status = H5Gget_objinfo(instance->handle, label, 0, NULL);
		if (status == 0) // a group named label exists. 
		{
			dataGroup = H5Gopen(instance->handle, label, H5P_DEFAULT);
		}
		else // The group either does NOT exist\n or some other error occurred.
		{
			dataGroup = H5Gcreate(instance->handle, label, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
		}
		IUF_ERR_EVAL_N_RETURN(dataGroup == H5I_INVALID_HID, H5I_INVALID_HID);

        if (component == IUF_IQ_COMPONENT_I)
        {
			dataStream->rfDataset = H5Dcreate(dataGroup, "I", H5T_NATIVE_FLOAT, space, H5P_DEFAULT, dataChunkConfig, H5P_DEFAULT);
            iufDataStreamDictSet(instance->iDataStreamDict,label,dataStream);
        }
        else // (component == IUF_IQ_COMPONENT_Q) //no datachunk set
        {
			dataStream->rfDataset = H5Dcreate(dataGroup, "Q", H5T_NATIVE_FLOAT, space, H5P_DEFAULT, dataChunkConfig, H5P_DEFAULT);
            iufDataStreamDictSet(instance->qDataStreamDict,label,dataStream);
        }
		H5Sclose(space);
		H5Gclose(dataGroup);
    }
    return H5Dget_space(dataStream->rfDataset);
}


static hid_t iufIqFileGetReadSpace
(
    iuif_t iqFile,
    char *label,
    IufComponent component
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(iqFile, H5I_INVALID_HID);
    IUF_ERR_CHECK_NULL_N_RETURN(label, H5I_INVALID_HID);
    iuiqfi_t instance = iufHistoryNodeGetInstanceData((iuhn_t)iqFile);
    iuds_t dataStream = IUDS_INVALID;
	char *path;
	char comp = (component == IUF_IQ_COMPONENT_I) ? 'I' : 'Q';

    if (component == IUF_IQ_COMPONENT_I)
    {
        dataStream = iufDataStreamDictGet(instance->iDataStreamDict, label);
    }
    else if (component == IUF_IQ_COMPONENT_Q)
    {
        dataStream = iufDataStreamDictGet(instance->qDataStreamDict, label);
    }
    else 
    {
        IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_HDF5, IUF_ERR_MIN_HDF5, "wrong component specfic, %d", component);
        return H5I_INVALID_HID;
    }
        
    if ( dataStream == IUDS_INVALID)
    {
        dataStream = iufDataStreamCreate();
		path = (char *)calloc(strlen(label) + 3, sizeof(char));
		sprintf(path, "%s/%c", label, comp);

        dataStream->rfDataset = H5Dopen(instance->handle, path, H5P_DEFAULT);
        if (component == IUF_IQ_COMPONENT_I)
        {
            iufDataStreamDictSet(instance->iDataStreamDict,label,dataStream);
        }
        else // (component == IUF_IQ_COMPONENT_Q)
        {
            iufDataStreamDictSet(instance->qDataStreamDict,label,dataStream);
        }
    }
    return H5Dget_space(dataStream->rfDataset);
}


int iufIqFileFrameSave
(
    iuiqf_t iqFile,
    char *label,
    IufComponent component,
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

    memdim[0] = (hsize_t) iufIqFileGetNumChannels(iqFile,label);
    memdim[1] = (hsize_t) iufIqFileGetSamplesPerLine(iqFile,label);
    memdim[2] = (hsize_t) iufIqFileGetNumResponses(iqFile,label);
    memspace = H5Screate_simple(3, memdim, NULL);

    offset[0] = (hsize_t) frame_offset->x;
    offset[1] = (hsize_t) frame_offset->z;
    offset[2] = (hsize_t) frame_offset->y;
    offset[3] = (hsize_t) frame_offset->t;

    count[0] = memdim[0];
    count[1] = memdim[1];
    count[2] = memdim[2];
    count[3] = 1;

    iuiqfi_t instance = iufHistoryNodeGetInstanceData((iuhn_t)iqFile);
    dataspace = iufIqFileGetWriteSpace(iqFile, label, component, 3);
    status = H5Sselect_hyperslab(dataspace, H5S_SELECT_SET, offset, NULL, count, NULL);
    
    float *pFrame = iufDataGetPointer(frame);

    // Save frame
    iuds_t dataStream;
    if (component == IUF_IQ_COMPONENT_I)
    {
       dataStream = iufDataStreamDictGet(instance->iDataStreamDict,label);
    }
    else // IUF_IQ_COMPONENT_Q
    {
        /* code */
        dataStream = iufDataStreamDictGet(instance->qDataStreamDict,label);
    }
    status |= H5Dwrite(dataStream->rfDataset, H5T_NATIVE_FLOAT, memspace, dataspace, H5P_DEFAULT, pFrame);

    // Close and release memspace but not (file)dataspace
    status |= H5Sclose(memspace);
    status |= H5Sclose(dataspace);

    return status;
}

int iufIqFileResponseSave
(
    iuiqf_t iqFile,
    char *label,
    IufComponent component,
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

    memdim[0] = (hsize_t) iufIqFileGetNumChannels(iqFile,label);
    memdim[1] = (hsize_t) iufIqFileGetSamplesPerLine(iqFile,label);
    memspace = H5Screate_simple(2, memdim, NULL);

    offset[0] = (hsize_t) response_offset->x;
    offset[1] = (hsize_t) response_offset->z;
    offset[2] = (hsize_t) response_offset->y;
    offset[3] = (hsize_t) response_offset->t;

    count[0] = memdim[0];
    count[1] = memdim[1];
    count[2] = 1;
    count[3] = 1;

    iuiqfi_t instance = iufHistoryNodeGetInstanceData((iuhn_t)iqFile);
    dataspace = iufIqFileGetWriteSpace(iqFile, label, component, 2);
    status = H5Sselect_hyperslab(dataspace, H5S_SELECT_SET, offset, NULL, count, NULL);

    float *pFrame = iufDataGetPointer(response);

    // Save frame
    iuds_t dataStream; 
    if (component == IUF_IQ_COMPONENT_I)
    {
       dataStream = iufDataStreamDictGet(instance->iDataStreamDict,label);
    }
    else // IUF_IQ_COMPONENT_Q
    {
        /* code */
        dataStream = iufDataStreamDictGet(instance->qDataStreamDict,label);
    }
    status |= H5Dwrite(dataStream->rfDataset, H5T_NATIVE_FLOAT, memspace, dataspace, H5P_DEFAULT, pFrame);

    // Close and release memspace but not (file)dataspace
    status |= H5Sclose(memspace);
    status |= H5Sclose(dataspace);

    return status;
}

int iufIqFileChannelSave
(
    iuiqf_t iqFile,
    char *label,
    IufComponent component,
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

    memdim = (hsize_t) iufIqFileGetNumChannels(iqFile,label);
    memspace = H5Screate_simple(1, &memdim, NULL);

    offset[0] = (hsize_t) channel_offset->x;
    offset[1] = (hsize_t) channel_offset->z;
    offset[2] = (hsize_t) channel_offset->y;
    offset[3] = (hsize_t) channel_offset->t;

    count[0] = memdim;
    count[1] = 1;
    count[2] = 1;
    count[3] = 1;

    iuiqfi_t instance = iufHistoryNodeGetInstanceData((iuhn_t)iqFile);
    dataspace = iufIqFileGetWriteSpace(iqFile, label, component, 1);
    status = H5Sselect_hyperslab(dataspace, H5S_SELECT_SET, offset, NULL, count, NULL);

    float *pFrame = iufDataGetPointer(channel);

    // Save frame
    iuds_t dataStream; 
    if (component == IUF_IQ_COMPONENT_I)
    {
       dataStream = iufDataStreamDictGet(instance->iDataStreamDict,label);
    }
    else // IUF_IQ_COMPONENT_Q
    {
        /* code */
        dataStream = iufDataStreamDictGet(instance->qDataStreamDict,label);
    }
    status |= H5Dwrite(dataStream->rfDataset, H5T_NATIVE_FLOAT, memspace, dataspace, H5P_DEFAULT, pFrame);

    // Close and release memspace but not (file)dataspace
    status |= H5Sclose(memspace);
    status |= H5Sclose(dataspace);

    return status;
}



int iufIqFileFrameLoad
(
    iuiqf_t iqFile,
    char *label,
    IufComponent component,
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

    memdim[0] = (hsize_t) iufIqFileGetNumChannels(iqFile,label);
    memdim[1] = (hsize_t) iufIqFileGetSamplesPerLine(iqFile,label);
    memdim[2] = (hsize_t) iufIqFileGetNumResponses(iqFile,label);
    memspace = H5Screate_simple(3, memdim, NULL);

    offset[0] = (hsize_t) frame_offset->x;
    offset[1] = (hsize_t) frame_offset->z;
    offset[2] = (hsize_t) frame_offset->y;
    offset[3] = (hsize_t) frame_offset->t;

    count[0] = memdim[0];
    count[1] = memdim[1];
    count[2] = memdim[2];
    count[3] = 1;

    iuiqfi_t instance = iufHistoryNodeGetInstanceData((iuhn_t)iqFile);
    dataspace = iufIqFileGetReadSpace(iqFile, label, component);
    status = H5Sselect_hyperslab(dataspace, H5S_SELECT_SET, offset, NULL, count, NULL);

    float *pFrame = iufDataGetPointer(frame);

    // Load frame
    iuds_t dataStream; 
    if (component == IUF_IQ_COMPONENT_I)
    {
       dataStream = iufDataStreamDictGet(instance->iDataStreamDict,label);
    }
    else // IUF_IQ_COMPONENT_Q
    {
        /* code */
        dataStream = iufDataStreamDictGet(instance->qDataStreamDict,label);
    }
    status |= H5Dread(dataStream->rfDataset, H5T_NATIVE_FLOAT, memspace, dataspace, H5P_DEFAULT, pFrame);

    // Close and release memspace but not (file)dataspace
    status |= H5Sclose(memspace);
    status |= H5Sclose(dataspace);

    return status;
}


int iufIqFileResponseLoad
(
    iuiqf_t iqFile,
    char *label,
    IufComponent component,
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

    memdim[0] = (hsize_t) iufIqFileGetNumChannels(iqFile,label);
    memdim[1] = (hsize_t) iufIqFileGetSamplesPerLine(iqFile,label);
    memspace = H5Screate_simple(2, memdim, NULL);

    offset[0] = (hsize_t) response_offset->x;
    offset[1] = (hsize_t) response_offset->z;
    offset[2] = (hsize_t) response_offset->y;
    offset[3] = (hsize_t) response_offset->t;

    count[0] = memdim[0];
    count[1] = memdim[1];
    count[2] = 1;
    count[3] = 1;

    iuiqfi_t instance = iufHistoryNodeGetInstanceData((iuhn_t)iqFile);
    dataspace = iufIqFileGetReadSpace(iqFile, label, component);
    status = H5Sselect_hyperslab(dataspace, H5S_SELECT_SET, offset, NULL, count, NULL);

    float *pResponse = iufDataGetPointer(response);

    // Load frame
    iuds_t dataStream; 
    if (component == IUF_IQ_COMPONENT_I)
    {
       dataStream = iufDataStreamDictGet(instance->iDataStreamDict,label);
    }
    else // IUF_IQ_COMPONENT_Q
    {
        /* code */
        dataStream = iufDataStreamDictGet(instance->qDataStreamDict,label);
    }
    status |= H5Dread(dataStream->rfDataset, H5T_NATIVE_FLOAT, memspace, dataspace, H5P_DEFAULT, pResponse);

    // Close and release memspace but not (file)dataspace
    status |= H5Sclose(memspace);
    status |= H5Sclose(dataspace);

    return status;

}


int iufIqFileChannelLoad
(
    iuiqf_t iqFile,
    char *label,
    IufComponent component,
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

    memdim = (hsize_t) iufIqFileGetNumChannels(iqFile,label);
    memspace = H5Screate_simple(1, &memdim, NULL);

    offset[0] = (hsize_t) channel_offset->x;
    offset[1] = (hsize_t) channel_offset->z;
    offset[2] = (hsize_t) channel_offset->y;
    offset[3] = (hsize_t) channel_offset->t;

    count[0] = memdim;
    count[1] = 1;
    count[2] = 1;
    count[3] = 1;

    iuiqfi_t instance = iufHistoryNodeGetInstanceData((iuhn_t)iqFile);
    dataspace = iufIqFileGetReadSpace(iqFile, label, component);
    status = H5Sselect_hyperslab(dataspace, H5S_SELECT_SET, offset, NULL, count, NULL);

    float *pFrame = iufDataGetPointer(channel);

    // Load channel
    iuds_t dataStream; 
    if (component == IUF_IQ_COMPONENT_I)
    {
       dataStream = iufDataStreamDictGet(instance->iDataStreamDict,label);
    }
    else // IUF_IQ_COMPONENT_Q
    {
        /* code */
        dataStream = iufDataStreamDictGet(instance->qDataStreamDict,label);
    }
    status |= H5Dread(dataStream->rfDataset, H5T_NATIVE_FLOAT, memspace, dataspace, H5P_DEFAULT, pFrame);

    // Close and release memspace but not (file)dataspace
    status |= H5Sclose(memspace);
    status |= H5Sclose(dataspace);

    return status;
}
