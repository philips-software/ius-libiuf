
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
#include <iusIqFilePrivate.h>
#include <iusIqFileStructure.h>
#include <iusIqPatternListDictPrivate.h>
#include <iusPulseDictPrivate.h>
#include <iusReceiveChannelMapDictPrivate.h>
#include <iusDemodulationDictPrivate.h>
#include <iusSourceDictPrivate.h>
#include <iusTransducerPrivate.h>
#include <iusTransmitApodizationDictPrivate.h>

struct IusIqFileInstance
{
    iufl_t frameList;
	iuiqpald_t iqPatternListDict;        /**< a dictionary of pattern lists              */
    iupd_t pulseDict;                    /**< a dictionary of pulses                      */
    iusd_t pulseSourceDict;              /**< a dictionary of sources                     */
	iurcmd_t receiveChannelMapDict;      /**< a dictionary of receiveChannelMaps          */
	iutad_t transmitApodizationDict;     /**< a dictionary of transmitApodizations        */
	iudmd_t demodulationDict;            /**< a dictionary of demodulations               */
	iut_t transducer;                    /**< The transducer description                  */
	iua_t acquisition;                    /**< The description of the performed acquisition */
    int IusVersion;                      /**< The version of input file format */


    //  state variables
    hid_t               handle;           /**< HDF5 file handle     */
    const char          *pFilename;       /**< the filename         */
    iudsd_t             dataStreamDict;   /**< Contains dataset administration */
    IUS_BOOL            deepDelete;
}  ;



struct IusIqFile
{
    iuhn_t history;
};

iuiqfi_t iusIqFileInstanceCreate
(
    void
)
{
    iuiqfi_t instanceData = (iuiqfi_t) calloc(1, sizeof(IusIqFileInstance));
    IUS_ERR_ALLOC_NULL_N_RETURN(instanceData, IusInputFileInstance, IUIQFI_INVALID);

	instanceData->IusVersion = iusGetVersionMajor();
	instanceData->pFilename = "";
	instanceData->frameList = IUFL_INVALID;
	instanceData->iqPatternListDict = IUIQPALD_INVALID;
	instanceData->pulseDict = IUPD_INVALID;
    instanceData->pulseSourceDict = IUSD_INVALID;
	instanceData->receiveChannelMapDict = IURCMD_INVALID;
    instanceData->transmitApodizationDict = IUTAD_INVALID;
    instanceData->demodulationDict = IUDMD_INVALID;
    instanceData->transducer = IUT_INVALID;
	instanceData->acquisition = IUA_INVALID;
	instanceData->dataStreamDict = iusDataStreamDictCreate();
	instanceData->deepDelete = IUS_FALSE;
    return instanceData;
}

int iusIqFileInstanceDelete
(
    iuiqfi_t instance
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
        iusDemodulationDictDelete(instance->demodulationDict);
        iusTransmitApodizationDictDelete(instance->transmitApodizationDict);
        iusReceiveChannelMapDictDelete(instance->receiveChannelMapDict);
        iusSourceDictDelete(instance->pulseSourceDict);
        iusPulseDictDelete(instance->pulseDict);
        iusIqPatternListDictDelete(instance->iqPatternListDict);
    }
    free(instance);
    return IUS_E_OK;
}

// ADT
int iusIqFileDelete
(
    iuiqf_t iqFile
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(iqFile, IUS_ERR_VALUE);
    iuiqfi_t instance = iusHistoryNodeGetInstanceData(iqFile);
    iusIqFileInstanceDelete(instance);
    iusHistoryNodeDelete((iuhn_t)iqFile);
    return IUS_E_OK;
}

iuiqf_t iusIqFileCreate
(
    const char *filename
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(filename, IUIQF_INVALID);
	iuiqfi_t instanceData = iusIqFileInstanceCreate();
    if (instanceData == IUIQFI_INVALID)
    {
        IUS_ERROR_PUSH(IUS_ERR_MAJ_MEMORY, IUS_ERR_MIN_ALLOC,  "calloc of iusIqFileInstanceCreate failed");
        return IUIQF_INVALID;
    }

    instanceData->pFilename = strdup(filename);
    instanceData->handle = H5Fcreate(filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
	if (instanceData->handle == H5I_INVALID_HID)
	{
        IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_ERROR, IUS_ERR_MIN_HDF5,  "unable to create file %s", filename);
        iusIqFileInstanceDelete(instanceData);
		return IUIQF_INVALID;
	}
	iuhn_t node = iusHistoryNodeCreate(IUS_IQ_TYPE);
	iusHistoryNodeSetInstanceData(node,(void *)instanceData);
	return (iuiqf_t)node;
}


int iusIqFileGetNumFrames
(
    iuiqf_t iqFile
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(iqFile, -1);
    iuiqfi_t instance = iusHistoryNodeGetInstanceData((iuhn_t)iqFile);
    if (instance->frameList == IUFL_INVALID) return 0;
    return iusFrameListGetSize(instance->frameList);
}

int iusIqFileGetNumChannels
(
    iuiqf_t iqFile,
    char *label
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(iqFile, -1);
    IUS_ERR_CHECK_NULL_N_RETURN(label, -1);

    iurcmd_t receiveChannelMapDict = iusIqFileGetReceiveChannelMapDict(iqFile);
    if (receiveChannelMapDict == IURCMD_INVALID) return -1;
    iurcm_t receiveChannelMap = iusReceiveChannelMapDictGet(receiveChannelMapDict,label);
    if (receiveChannelMap == IURCM_INVALID) return -1;
    return iusReceiveChannelMapGetNumChannels(receiveChannelMap);
}


int iusIqFileGetSamplesPerLine
(
    iuiqf_t iqFile,
    char *label
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(iqFile, -1);
    IUS_ERR_CHECK_NULL_N_RETURN(label, -1);
    iudmd_t demodulationDict = iusIqFileGetDemodulationDict(iqFile);
    if (demodulationDict == IUDMD_INVALID) return -1;
    iudm_t demodulation = iusDemodulationDictGet(demodulationDict,label);
    if (demodulation == IUDM_INVALID) return -1;
    return iusDemodulationGetNumSamplesPerLine(demodulation);
}

int iusIqFileGetNumResponses
(
    iuiqf_t iqFile,
    char *label
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(iqFile, -1);
    IUS_ERR_CHECK_NULL_N_RETURN(label, -1);
    iuiqpald_t listDict = iusIqFileGetPatternListDict(iqFile);
    if (listDict == IUIQPALD_INVALID) return -1;
    iuiqpal_t patternList = iusIqPatternListDictGet(listDict,label);
    if (patternList == IUIQPAL_INVALID) return -1;
    return iusIqPatternListGetSize(patternList);
}

iud_t iusIqFileFrameCreate
(
    iuiqf_t iqFile,
    char *label
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(iqFile, IUD_INVALID);
    IUS_ERR_CHECK_NULL_N_RETURN(label, IUD_INVALID);
    // calculate frame size:
    // numchannels * numresponses * numsamples = [x*y*z]
    int numChannels = iusIqFileGetNumChannels(iqFile,label);
    int numResponses = iusIqFileGetNumResponses(iqFile,label);
    int numSamples = iusIqFileGetSamplesPerLine(iqFile,label);
    int frameSize = numChannels * numSamples * numResponses;
    return iusDataCreate(frameSize);
}

iud_t iusIqFileResponseCreate
(
    iuiqf_t iqFile,
    char *label
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(iqFile, IUD_INVALID);
    IUS_ERR_CHECK_NULL_N_RETURN(label, IUD_INVALID);
    int numChannels = iusIqFileGetNumChannels(iqFile,label);
    int numSamples = iusIqFileGetSamplesPerLine(iqFile,label);
    int responseSize = numChannels * numSamples ;
    return iusDataCreate(responseSize);
}

iud_t iusIqFileChannelCreate
(
    iuiqf_t iqFile,
    char *label
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(iqFile, IUD_INVALID);
    IUS_ERR_CHECK_NULL_N_RETURN(label, IUD_INVALID);
    int numSamples = iusIqFileGetSamplesPerLine(iqFile,label);
    return iusDataCreate(numSamples);
}


static iuiqfi_t iqFileInstanceLoad
(
    iuiqfi_t instance
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(instance, IUIQFI_INVALID);
    instance->frameList = iusFrameListLoad(instance->handle);
    if (instance->frameList == IUFL_INVALID)
    {
        IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_HDF5, IUS_ERR_MIN_HDF5, "could not load framelist from %s", instance->pFilename);
        return IUIQFI_INVALID;
    }

    instance->iqPatternListDict = iusIqPatternListDictLoad(instance->handle);
    if (instance->iqPatternListDict == IUIQPALD_INVALID)
    {
        IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_HDF5, IUS_ERR_MIN_HDF5, "could not load patternlist from %s", instance->pFilename);
        return IUIQFI_INVALID;
    }

    instance->pulseDict = iusPulseDictLoad(instance->handle);
    if (instance->pulseDict == IUPD_INVALID)
    {
        IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_HDF5, IUS_ERR_MIN_HDF5, "could not load pulses from %s", instance->pFilename);
        return IUIQFI_INVALID;
    }

    instance->pulseSourceDict = iusSourceDictLoad(instance->handle);
    if (instance->pulseSourceDict == IUSD_INVALID)
    {
        IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_HDF5, IUS_ERR_MIN_HDF5, "could not load pulse sources from %s", instance->pFilename);
        return IUIQFI_INVALID;
    }

    instance->receiveChannelMapDict = iusReceiveChannelMapDictLoad(instance->handle);
    if (instance->receiveChannelMapDict == IURCMD_INVALID)
    {
        IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_HDF5, IUS_ERR_MIN_HDF5, "could not load receiveChannelMap from %s", instance->pFilename);
        return IUIQFI_INVALID;
    }

    instance->transmitApodizationDict = iusTransmitApodizationDictLoad(instance->handle);
    if (instance->transmitApodizationDict == IUTAD_INVALID)
    {
        IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_HDF5, IUS_ERR_MIN_HDF5, "could not load transmitApodizationDict from %s", instance->pFilename);
        return IUIQFI_INVALID;
    }

    instance->demodulationDict = iusDemodulationDictLoad(instance->handle);
    if (instance->demodulationDict == IUDMD_INVALID)
    {
        IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_HDF5, IUS_ERR_MIN_HDF5, "could not load demodulationDict from %s", instance->pFilename);
        return IUIQFI_INVALID;
    }

    instance->acquisition = iusAcquisitionLoad(instance->handle);
    if (instance->acquisition == IUA_INVALID)
    {
        IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_HDF5, IUS_ERR_MIN_HDF5, "could not load acquisition from %s", instance->pFilename);
        return IUIQFI_INVALID;
    }

    instance->transducer = iusTransducerLoad(instance->handle);
    if (instance->transducer == IUT_INVALID)
    {
        IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_HDF5, IUS_ERR_MIN_HDF5, "could not load transducer from %s", instance->pFilename);
        return IUIQFI_INVALID;
    }

    int status = iusHdf5ReadInt( instance->handle, IUS_IQFILE_PATH_IUSVERSION, &(instance->IusVersion));
    if( status != IUS_E_OK )
    {
        IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_HDF5, IUS_ERR_MIN_HDF5, "could not load IusVersion from %s", instance->pFilename);
        return IUIQFI_INVALID;
    }

    instance->deepDelete = IUS_TRUE;
    return instance;
}

void *iusIqFileInstanceLoad
(
    hid_t handle
)
{
    iuiqfi_t instance = iusIqFileInstanceCreate();
    iuiqfi_t new_instance;
    instance->handle = handle;
    new_instance = iqFileInstanceLoad(instance);
    if( new_instance == IUIQFI_INVALID )
    {
        iusIqFileInstanceDelete(instance);
        instance = new_instance;
    }
    instance->deepDelete = IUS_TRUE;
    return (void *)instance;
}


iuhn_t iusIqFileLoadNode
(
    hid_t handle
)
{
    iuhn_t node = iusHistoryNodeCreate(IUS_INPUT_TYPE);
    iuiqfi_t instance = iusIqFileInstanceCreate();
    instance->handle = handle;
    instance = iqFileInstanceLoad(instance);
    iusHistoryNodeSetInstanceData(node,instance);
    return node;
}

iuiqf_t iusIqFileNodeLoad
(
    const char *pFilename
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(pFilename, IUIF_INVALID);

    // open  Hdf5 file using default properties.
    hid_t handle = H5Fopen( pFilename, H5F_ACC_RDONLY, H5P_DEFAULT );
    if (handle <=0)
    {
        IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_HDF5, IUS_ERR_MIN_HDF5, "could not open file %s", pFilename);
        return IUIQF_INVALID;
    }

    iuiqf_t iqFile = (iuiqf_t) iusHistoryNodeLoad(handle);
    iuiqfi_t instance = iusHistoryNodeGetInstanceData((iuhn_t)iqFile);
	if (instance == IUIQFI_INVALID)
	{
        IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_HDF5, IUS_ERR_MIN_HDF5, "could not get instance data %s", pFilename);
		return IUIQF_INVALID;
	}
    instance->pFilename = strdup(pFilename);
    return iqFile;
}

int iusIqFileSaveInstance
(
    hid_t handle,
    iuiqfi_t instanceData
)
{
    herr_t status=0;
    IUS_ERR_CHECK_NULL_N_RETURN(instanceData, IUS_ERR_VALUE);
    status |= iusFrameListSave(instanceData->frameList, handle);
    status |= iusIqPatternListDictSave(instanceData->iqPatternListDict, handle);
    status |= iusPulseDictSave(instanceData->pulseDict, handle);
    status |= iusSourceDictSave(instanceData->pulseSourceDict, handle);
    status |= iusReceiveChannelMapDictSave(instanceData->receiveChannelMapDict, handle);
    status |= iusTransmitApodizationDictSave(instanceData->transmitApodizationDict, handle);
    status |= iusDemodulationDictSave(instanceData->demodulationDict, handle);

    status |= iusAcquisitionSave(instanceData->acquisition, handle);
    status |= iusTransducerSave(instanceData->transducer, instanceData->handle);
    status |= iusHdf5WriteInt( instanceData->handle, IUS_IQFILE_PATH_IUSVERSION, &(instanceData->IusVersion), 1);
    return status;
}


int iusIqFileNodeSave
(
    iuiqf_t iqFile
)
{
    herr_t status=0;
    IUS_ERR_CHECK_NULL_N_RETURN(iqFile, IUS_ERR_VALUE);

    iuiqfi_t instance = iusHistoryNodeGetInstanceData((iuhn_t)iqFile);
    status |= iusHistoryNodeSave(iqFile, instance->handle);
	return status;
}


int iusIqFileClose
(
    iuiqf_t iqFile
)
{
    int status=0;
    IUS_ERR_CHECK_NULL_N_RETURN(iqFile, IUS_ERR_VALUE);
    iuiqfi_t instance = iusHistoryNodeGetInstanceData((iuhn_t)iqFile);

    // Terminate access to the file.
    if( instance->handle != H5I_INVALID_HID )
    {
        status |= H5Fclose(instance->handle);
    }
    return status;
}

static int iusIqFileCompareInstance
(
    iuiqfi_t reference,
    iuiqfi_t actual
)
{
    if ( reference == actual ) return IUS_TRUE;
    if ( reference == NULL || actual == NULL ) return IUS_FALSE;
    if ( reference->IusVersion != actual->IusVersion ) return IUS_FALSE;
    if ( strcmp(reference->pFilename, actual->pFilename) != 0 ) return IUS_FALSE;
    if ( iusFrameListCompare(reference->frameList, actual->frameList)  == IUS_FALSE ) return IUS_FALSE;
    if ( iusIqPatternListDictCompare(reference->iqPatternListDict, actual->iqPatternListDict)  == IUS_FALSE ) return IUS_FALSE;
    if ( iusPulseDictCompare(reference->pulseDict, actual->pulseDict)  == IUS_FALSE ) return IUS_FALSE;
    if ( iusSourceDictCompare(reference->pulseSourceDict, actual->pulseSourceDict)  == IUS_FALSE ) return IUS_FALSE;
    if ( iusReceiveChannelMapDictCompare(reference->receiveChannelMapDict, actual->receiveChannelMapDict) == IUS_FALSE) return IUS_FALSE;
    if ( iusTransmitApodizationDictCompare(reference->transmitApodizationDict, actual->transmitApodizationDict)  == IUS_FALSE ) return IUS_FALSE;
    if ( iusDemodulationDictCompare(reference->demodulationDict, actual->demodulationDict)  == IUS_FALSE ) return IUS_FALSE;
    if ( iusTransducerCompare(reference->transducer, actual->transducer) == IUS_FALSE) return IUS_FALSE;
    if (iusAcquisitionCompare(reference->acquisition, actual->acquisition) == IUS_FALSE) return IUS_FALSE;
    return IUS_TRUE;
}

int iusIqFileCompare
(
    iuiqf_t reference,
    iuiqf_t actual
)
{
    if ( reference == actual ) return IUS_TRUE;
    if ( reference == NULL || actual == NULL ) return IUS_FALSE;
    if ( iusHistoryNodeCompareWithId(reference, actual)  == IUS_FALSE ) return IUS_FALSE;

    // Compare instance data
    iuiqfi_t refInstance = iusHistoryNodeGetInstanceData((iuhn_t)reference);
    iuiqfi_t actInstance = iusHistoryNodeGetInstanceData((iuhn_t)actual);
    IUS_BOOL equal = iusIqFileCompareInstance(refInstance,actInstance);
    if (equal == IUS_FALSE) return IUS_FALSE;


    return equal;
}

// Getters
iufl_t iusIqFileGetFrameList
(
    iuiqf_t iqFile
)
{
    if ( iqFile != NULL )
    {
        iuiqfi_t instance = iusHistoryNodeGetInstanceData((iuhn_t)iqFile);
        return instance->frameList;
    }
    return IUFL_INVALID;
}

iuiqpald_t iusIqFileGetPatternListDict
(
    iuiqf_t iqFile
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(iqFile, IUIQPALD_INVALID);
    iuiqfi_t instance = iusHistoryNodeGetInstanceData((iuhn_t)iqFile);
    return instance->iqPatternListDict;
}


iupd_t iusIqFileGetPulseDict
(
    iuiqf_t iqFile
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(iqFile, IUPD_INVALID);
    iuiqfi_t instance = iusHistoryNodeGetInstanceData((iuhn_t)iqFile);
    return instance->pulseDict;
}

iusd_t iusIqFileGetSourceDict
(
    iuiqf_t iqFile
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(iqFile, IUSD_INVALID);
    iuiqfi_t instance = iusHistoryNodeGetInstanceData((iuhn_t)iqFile);
    return instance->pulseSourceDict;
}


iurcmd_t iusIqFileGetReceiveChannelMapDict
(
    iuiqf_t iqFile
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(iqFile, IURCMD_INVALID);
    iuiqfi_t instance = iusHistoryNodeGetInstanceData((iuhn_t)iqFile);
    return instance->receiveChannelMapDict;
}

iutad_t iusIqFileGetTransmitApodizationDict
(
    iuiqf_t iqFile
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(iqFile, IUTAD_INVALID);
    iuiqfi_t instance = iusHistoryNodeGetInstanceData((iuhn_t)iqFile);
    return instance->transmitApodizationDict;
}

iudmd_t iusIqFileGetDemodulationDict
(
    iuiqf_t iqFile
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(iqFile, IUDMD_INVALID);
    iuiqfi_t instance = iusHistoryNodeGetInstanceData((iuhn_t)iqFile);
    return instance->demodulationDict;
}

iua_t iusIqFileGetAcquisition
(
   iuiqf_t iqFile
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(iqFile, IUA_INVALID);
    iuiqfi_t instance = iusHistoryNodeGetInstanceData((iuhn_t)iqFile);
    return instance->acquisition;
}

iut_t iusIqFileGetTransducer
(
    iuiqf_t iqFile
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(iqFile, IUT_INVALID);
    iuiqfi_t instance = iusHistoryNodeGetInstanceData((iuhn_t)iqFile);
    return instance->transducer;
}



// Setters
int iusIqFileSetFrameList
(
    iuiqf_t iqFile,
    iufl_t frameList
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(iqFile, IUS_ERR_VALUE);
    IUS_ERR_CHECK_NULL_N_RETURN(frameList, IUS_ERR_VALUE);
    iuiqfi_t instance = iusHistoryNodeGetInstanceData((iuhn_t)iqFile);
    instance->frameList = frameList;
    return IUS_E_OK;
}

int iusIqFileSetPatternListDict
(
    iuiqf_t iqFile,
    iuiqpald_t iqPatternListDict
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(iqFile, IUS_ERR_VALUE);
    IUS_ERR_CHECK_NULL_N_RETURN(iqPatternListDict, IUS_ERR_VALUE);
    iuiqfi_t instance = iusHistoryNodeGetInstanceData((iuhn_t)iqFile);
    instance->iqPatternListDict = iqPatternListDict;
    return IUS_E_OK;
}

int iusIqFileSetPulseDict
(
    iuiqf_t iqFile,
    iupd_t pulseDict
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(iqFile, IUS_ERR_VALUE);
    IUS_ERR_CHECK_NULL_N_RETURN(pulseDict, IUS_ERR_VALUE);
    iuiqfi_t instance = iusHistoryNodeGetInstanceData((iuhn_t)iqFile);
    instance->pulseDict = pulseDict;
    return IUS_E_OK;
}



int iusIqFileSetSourceDict
(
    iuiqf_t iqFile,
    iusd_t sourceDict
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(iqFile, IUS_ERR_VALUE);
    IUS_ERR_CHECK_NULL_N_RETURN(sourceDict, IUS_ERR_VALUE);
    iuiqfi_t instance = iusHistoryNodeGetInstanceData((iuhn_t)iqFile);
    instance->pulseSourceDict = sourceDict;
    return IUS_E_OK;
}

int iusIqFileSetReceiveChannelMapDict
(
	iuiqf_t iqFile,
	iurcmd_t receiveChannelMapDict
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(iqFile, IUS_ERR_VALUE);
    IUS_ERR_CHECK_NULL_N_RETURN(receiveChannelMapDict, IUS_ERR_VALUE);
    iuiqfi_t instance = iusHistoryNodeGetInstanceData((iuhn_t)iqFile);
    instance->receiveChannelMapDict = receiveChannelMapDict;
	return IUS_E_OK;
}

int iusIqFileSetTransmitApodizationDict
(
	iuiqf_t iqFile,
	iutad_t transmitApodizationDict
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(iqFile, IUS_ERR_VALUE);
    IUS_ERR_CHECK_NULL_N_RETURN(transmitApodizationDict, IUS_ERR_VALUE);
    iuiqfi_t instance = iusHistoryNodeGetInstanceData((iuhn_t)iqFile);
    instance->transmitApodizationDict = transmitApodizationDict;
	return IUS_E_OK;
}

int iusIqFileSetDemodulationDict
(
    iuiqf_t iqFile,
    iudmd_t demodulationDict
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(iqFile, IUS_ERR_VALUE);
    IUS_ERR_CHECK_NULL_N_RETURN(demodulationDict, IUS_ERR_VALUE);
    iuiqfi_t instance = iusHistoryNodeGetInstanceData((iuhn_t)iqFile);
    instance->demodulationDict = demodulationDict;
    return IUS_E_OK;
}


int iusIqFileSetAcquisition
(
    iuiqf_t iqFile,
    iua_t acquisition
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(iqFile, IUS_ERR_VALUE);
    IUS_ERR_CHECK_NULL_N_RETURN(acquisition, IUS_ERR_VALUE);
    iuiqfi_t instance = iusHistoryNodeGetInstanceData((iuhn_t)iqFile);
    instance->acquisition = acquisition;
	return IUS_E_OK;
}


int iusIqFileSetTransducer
(
    iuiqf_t iqFile,
    iut_t transducer
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(iqFile, IUS_ERR_VALUE);
    IUS_ERR_CHECK_NULL_N_RETURN(transducer, IUS_ERR_VALUE);
    iuiqfi_t instance = iusHistoryNodeGetInstanceData((iuhn_t)iqFile);
    instance->transducer = transducer;
    return IUS_E_OK;
}

int iusIqFileSetFilename
(
    iuiqf_t iqFile,
    char * fileName
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(iqFile, IUS_ERR_VALUE);
    IUS_ERR_CHECK_NULL_N_RETURN(fileName, IUS_ERR_VALUE);
    iuiqfi_t instance = iusHistoryNodeGetInstanceData((iuhn_t)iqFile);
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

hid_t iusIqFileGetWriteSpace
(
    iuiqf_t iqFile,
    char *label,
    int numDims
)
{
    hsize_t rfDataDims[4];
    hsize_t chunkDims[4];
    hid_t space, dataChunkConfig;
    IUS_ERR_CHECK_NULL_N_RETURN(iqFile, H5I_INVALID_HID);
    IUS_ERR_CHECK_NULL_N_RETURN(label, H5I_INVALID_HID);

    iuiqfi_t instance = iusHistoryNodeGetInstanceData((iuhn_t)iqFile);
    iuds_t dataStream = iusDataStreamDictGet(instance->dataStreamDict,label);
    if ( dataStream == IUDS_INVALID)
    {
        // Entry does not exist, create
        rfDataDims[0] = (hsize_t) iusIqFileGetNumChannels(iqFile,label);    // colums in memory
        rfDataDims[1] = (hsize_t) iusIqFileGetSamplesPerLine(iqFile,label); // rows in memory
        rfDataDims[2] = (hsize_t) iusIqFileGetNumResponses(iqFile,label);
        rfDataDims[3] = (hsize_t) iusIqFileGetNumFrames(iqFile);

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


hid_t iusIqFileGetReadSpace
(
    iuiqf_t iqFile,
    char *label
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(iqFile, H5I_INVALID_HID);
    IUS_ERR_CHECK_NULL_N_RETURN(label, H5I_INVALID_HID);
    iuiqfi_t instance = iusHistoryNodeGetInstanceData((iuhn_t)iqFile);
    iuds_t dataStream = iusDataStreamDictGet(instance->dataStreamDict,label);
    if ( dataStream == IUDS_INVALID)
    {
        dataStream = iusDataStreamCreate();
        dataStream->rfDataset = H5Dopen(instance->handle, label, H5P_DEFAULT);
        iusDataStreamDictSet(instance->dataStreamDict,label,dataStream);
    }
    return H5Dget_space(dataStream->rfDataset);
}


int iusIqFileFrameSave
(
    iuiqf_t iqFile,
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

    IUS_ERR_CHECK_NULL_N_RETURN(iqFile, H5I_INVALID_HID);
    IUS_ERR_CHECK_NULL_N_RETURN(label, H5I_INVALID_HID);
    IUS_ERR_CHECK_NULL_N_RETURN(frame, H5I_INVALID_HID);
    IUS_ERR_CHECK_NULL_N_RETURN(frame_offset, H5I_INVALID_HID);

    memdim[0] = (hsize_t) iusIqFileGetNumChannels(iqFile,label);
    memdim[1] = (hsize_t) iusIqFileGetSamplesPerLine(iqFile,label);
    memdim[2] = (hsize_t) iusIqFileGetNumResponses(iqFile,label);
    memspace = H5Screate_simple(3, memdim, NULL);

    offset[0] = (hsize_t) frame_offset->x;
    offset[1] = (hsize_t) frame_offset->z;
    offset[2] = (hsize_t) frame_offset->y;
    offset[3] = (hsize_t) frame_offset->t;

    count[0] = memdim[0];
    count[1] = memdim[1];
    count[2] = memdim[2];
    count[3] = 1;

    iuiqfi_t instance = iusHistoryNodeGetInstanceData((iuhn_t)iqFile);
    dataspace = iusIqFileGetWriteSpace(iqFile, label, 3);
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



int iusIqFileResponseSave
(
    iuiqf_t iqFile,
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

    IUS_ERR_CHECK_NULL_N_RETURN(iqFile, H5I_INVALID_HID);
    IUS_ERR_CHECK_NULL_N_RETURN(label, H5I_INVALID_HID);
    IUS_ERR_CHECK_NULL_N_RETURN(response, H5I_INVALID_HID);
    IUS_ERR_CHECK_NULL_N_RETURN(response_offset, H5I_INVALID_HID);

    memdim[0] = (hsize_t) iusIqFileGetNumChannels(iqFile,label);
    memdim[1] = (hsize_t) iusIqFileGetSamplesPerLine(iqFile,label);
    memspace = H5Screate_simple(2, memdim, NULL);

    offset[0] = (hsize_t) response_offset->x;
    offset[1] = (hsize_t) response_offset->z;
    offset[2] = (hsize_t) response_offset->y;
    offset[3] = (hsize_t) response_offset->t;

    count[0] = memdim[0];
    count[1] = memdim[1];
    count[2] = 1;
    count[3] = 1;

    iuiqfi_t instance = iusHistoryNodeGetInstanceData((iuhn_t)iqFile);
    dataspace = iusIqFileGetWriteSpace(iqFile, label, 2);
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

int iusIqFileChannelSave
(
    iuiqf_t iqFile,
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

    IUS_ERR_CHECK_NULL_N_RETURN(iqFile, H5I_INVALID_HID);
    IUS_ERR_CHECK_NULL_N_RETURN(label, H5I_INVALID_HID);
    IUS_ERR_CHECK_NULL_N_RETURN(channel, H5I_INVALID_HID);
    IUS_ERR_CHECK_NULL_N_RETURN(channel_offset, H5I_INVALID_HID);

    memdim = (hsize_t) iusIqFileGetNumChannels(iqFile,label);
    memspace = H5Screate_simple(1, &memdim, NULL);

    offset[0] = (hsize_t) channel_offset->x;
    offset[1] = (hsize_t) channel_offset->z;
    offset[2] = (hsize_t) channel_offset->y;
    offset[3] = (hsize_t) channel_offset->t;

    count[0] = memdim;
    count[1] = 1;
    count[2] = 1;
    count[3] = 1;

    iuiqfi_t instance = iusHistoryNodeGetInstanceData((iuhn_t)iqFile);
    dataspace = iusIqFileGetWriteSpace(iqFile, label, 1);
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



int iusIqFileFrameLoad
(
    iuiqf_t iqFile,
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

    IUS_ERR_CHECK_NULL_N_RETURN(iqFile, H5I_INVALID_HID);
    IUS_ERR_CHECK_NULL_N_RETURN(label, H5I_INVALID_HID);
    IUS_ERR_CHECK_NULL_N_RETURN(frame, H5I_INVALID_HID);
    IUS_ERR_CHECK_NULL_N_RETURN(frame_offset, H5I_INVALID_HID);

    memdim[0] = (hsize_t) iusIqFileGetNumChannels(iqFile,label);
    memdim[1] = (hsize_t) iusIqFileGetSamplesPerLine(iqFile,label);
    memdim[2] = (hsize_t) iusIqFileGetNumResponses(iqFile,label);
    memspace = H5Screate_simple(3, memdim, NULL);

    offset[0] = (hsize_t) frame_offset->x;
    offset[1] = (hsize_t) frame_offset->z;
    offset[2] = (hsize_t) frame_offset->y;
    offset[3] = (hsize_t) frame_offset->t;

    count[0] = memdim[0];
    count[1] = memdim[1];
    count[2] = memdim[2];
    count[3] = 1;

    iuiqfi_t instance = iusHistoryNodeGetInstanceData((iuhn_t)iqFile);
    dataspace = iusIqFileGetReadSpace(iqFile, label);
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


int iusIqFileResponseLoad
(
    iuiqf_t iqFile,
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

    IUS_ERR_CHECK_NULL_N_RETURN(iqFile, H5I_INVALID_HID);
    IUS_ERR_CHECK_NULL_N_RETURN(label, H5I_INVALID_HID);
    IUS_ERR_CHECK_NULL_N_RETURN(response, H5I_INVALID_HID);
    IUS_ERR_CHECK_NULL_N_RETURN(response_offset, H5I_INVALID_HID);

    memdim[0] = (hsize_t) iusIqFileGetNumChannels(iqFile,label);
    memdim[1] = (hsize_t) iusIqFileGetSamplesPerLine(iqFile,label);
    memspace = H5Screate_simple(2, memdim, NULL);

    offset[0] = (hsize_t) response_offset->x;
    offset[1] = (hsize_t) response_offset->z;
    offset[2] = (hsize_t) response_offset->y;
    offset[3] = (hsize_t) response_offset->t;

    count[0] = memdim[0];
    count[1] = memdim[1];
    count[2] = 1;
    count[3] = 1;

    iuiqfi_t instance = iusHistoryNodeGetInstanceData((iuhn_t)iqFile);
    dataspace = iusIqFileGetReadSpace(iqFile, label);
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


int iusIqFileChannelLoad
(
    iuiqf_t iqFile,
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

    IUS_ERR_CHECK_NULL_N_RETURN(iqFile, H5I_INVALID_HID);
    IUS_ERR_CHECK_NULL_N_RETURN(label, H5I_INVALID_HID);
    IUS_ERR_CHECK_NULL_N_RETURN(channel, H5I_INVALID_HID);
    IUS_ERR_CHECK_NULL_N_RETURN(channel_offset, H5I_INVALID_HID);

    memdim = (hsize_t) iusIqFileGetNumChannels(iqFile,label);
    memspace = H5Screate_simple(1, &memdim, NULL);

    offset[0] = (hsize_t) channel_offset->x;
    offset[1] = (hsize_t) channel_offset->z;
    offset[2] = (hsize_t) channel_offset->y;
    offset[3] = (hsize_t) channel_offset->t;

    count[0] = memdim;
    count[1] = 1;
    count[2] = 1;
    count[3] = 1;

    iuiqfi_t instance = iusHistoryNodeGetInstanceData((iuhn_t)iqFile);
    dataspace = iusIqFileGetReadSpace(iqFile, label);
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
