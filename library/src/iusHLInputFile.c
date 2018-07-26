
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


static const char PULSES_PATH[]="/Pulses";
static const char PATTERN_LIST_PATH[]="/PatternList";


struct IusInputFile
{
    const char *pFilename;
    iupd_t pulseDict;                    /**< a dictionary of pulses */
	iurcmd_t receiveChannelMapDict;      /**< a dictionary of receiveChannelMaps */
	iutad_t transmitApodizationDict;     /**< a dictionary of transmitApodizations */
    iupal_t patternList;
	iue_t experiment;


    //  state variables
    hid_t fileChunkConfig;                /**< file chunck handle */
    hid_t handle;                         /**< file handle */
    hid_t rfDataset;                      /**< dataset handle */
    int currentFrame;                     /**< current frame number */
    int currentPulse;                     /**< current pulse number */

    int numFrames;        /**< The number of frames in the data */
    int IusVersion;       /**< version of input file format */
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
	pFileInst->pulseDict = IUPD_INVALID;
	pFileInst->receiveChannelMapDict = IURCMD_INVALID;
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
        iusInputFile = NULL;
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
    pFileInst->pulseDict = iusHLPulseDictLoad(pFileInst->handle, PULSES_PATH);
    if (pFileInst->pulseDict == IUPD_INVALID)
    {
        fprintf( stderr, "Warning from iusHLInputFileLoad: could not load pulses: %s\n", pFilename );
        return IUIF_INVALID;
    }


	pFileInst->receiveChannelMapDict = iusHLReceiveChannelMapDictLoad(pFileInst->handle, "/ReceiveChannelMap");
	if (pFileInst->receiveChannelMapDict == IURCMD_INVALID)
	{
		fprintf(stderr, "Warning from iusHLInputFileLoad: could not load receiveChannelMap: %s\n", pFilename);
		return IUIF_INVALID;
	}

    // Todo: create group @here instead of in Load, see experiment
    pFileInst->patternList = iusHLPatternListLoad(pFileInst->handle, PATTERN_LIST_PATH);
    if (pFileInst->patternList == IUPAL_INVALID)
    {
        fprintf( stderr, "Warning from iusHLInputFileLoad: could not load patterns: %s\n", pFilename );
        return IUIF_INVALID;
    }

	hid_t group_id = H5Gopen(pFileInst->handle, "/Experiment", H5P_DEFAULT);
	pFileInst->experiment = iusHLExperimentLoad(group_id);
	if (pFileInst->experiment == IUE_INVALID)
	{
		fprintf(stderr, "Warning from iusHLInputFileLoad: could not load experiment: %s\n", pFilename);
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

    // Make dataset for Node
//    status |=iusWriteNode(&pInst->iusNode, handle, verbose);

    // Make dataset for input type
    status |=H5LTmake_dataset_int( fileHandle->handle,    "/IusVersion", 1, dims, &(fileHandle->IusVersion));
    status |=H5LTmake_dataset_int( fileHandle->handle,    "/numFrames",  1, dims, &(fileHandle->numFrames) );

    // Make dataset for Experiment
//    hid_t group_id = H5Gcreate(fileHandle->handle, "/Experiment", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
//    status |= iusWriteExperiment(pInst->pExperiment, handle, verbose);
//    status |= H5Gclose(group_id );

    // Make dataset for Transducer
//    group_id = H5Gcreate(handle, "/Transducer", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
//    iusWriteTransducer(pInst->pTransducer, group_id, verbose);
//    status |= H5Gclose(group_id );


    // Make dataset for DrivingScheme
//    group_id = H5Gcreate(handle, "/DrivingScheme", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
//    iusWriteDrivingScheme(pInst->pDrivingScheme, group_id, verbose);
//    status |= H5Gclose(group_id );


    // Todo: Handle creation in iusHLInputFileSave iso iusHLPulseDictSave, iusHLPatternListSave, iusHLReceiveChannelMapDictSave,iusHLTransmitApodizationDictSave
    // new signature: iusHLPulseDictSave(fileHandle->pulseDict,fileHandle->handle);
    status |= iusHLPulseDictSave(fileHandle->pulseDict,PULSES_PATH,fileHandle->handle);
	status |= iusHLReceiveChannelMapDictSave(fileHandle->receiveChannelMapDict, "/ReceiveChannelMap", fileHandle->handle);
	status |= iusHLTransmitApodizationDictSave(fileHandle->transmitApodizationDict, "/TransmitApodization", fileHandle->handle);
    status |= iusHLPatternListSave(fileHandle->patternList,PATTERN_LIST_PATH,fileHandle->handle);
	hid_t group_id = H5Gcreate(fileHandle->handle, "/Experiment", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
	status |= iusHLExperimentSave(fileHandle->experiment, group_id);
	status |= H5Gclose(group_id);
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
    if ( iusHLPulseDictCompare(reference->pulseDict, actual->pulseDict)  == IUS_FALSE ) return IUS_FALSE;
	if ( iusHLReceiveChannelMapDictCompare(reference->receiveChannelMapDict, actual->receiveChannelMapDict) == IUS_FALSE) return IUS_FALSE;
	if ( iusHLPatternListCompare(reference->patternList, actual->patternList)  == IUS_FALSE ) return IUS_FALSE;
	if ( iusHLExperimentCompare(reference->experiment, actual->experiment) == IUS_FALSE) return IUS_FALSE;
    return IUS_TRUE;
}

// Getters
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
