
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
#include <include/iusHLPulseDictImp.h>
#include <include/iusHLPulseDict.h>
#include <include/iusHLInputFile.h>
#include <include/iusHLExperiment.h>
#include <include/iusHLExperimentImp.h>

struct IusInputFile
{
    const char *pFilename;
    iupd_t pulseDict;
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
    pFileInst->pulseDict = iusHLPulseDictLoad(pFileInst->handle, "/Pulses");
    if (pFileInst->pulseDict == IUPD_INVALID)
    {
        fprintf( stderr, "Warning from iusHLInputFileLoad: could not load pulses: %s\n", pFilename );
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
    status |= iusHLPulseDictSave(fileHandle->pulseDict,"/Pulses",fileHandle->handle);

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
    if( reference == actual ) return IUS_TRUE;
    if( reference == NULL || actual == NULL ) return IUS_FALSE;
    if( iusHLPulseDictCompare(reference->pulseDict, actual->pulseDict)  == IUS_FALSE ) return IUS_FALSE;
	if (iusHLExperimentCompare(reference->experiment, actual->experiment) == IUS_FALSE) return IUS_FALSE;
    return IUS_TRUE;
}

// Getters
iupd_t iusHLInputFileGetPulseDict
(
    iuif_t iusInputFile
)
{
    if(iusInputFile != NULL)
    {
        return iusInputFile->pulseDict;
    }
    return NULL;
}

iue_t iusHLInputFileGetExperiment
(
	iuif_t iusInputFile
)
{
	if (iusInputFile != NULL)
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

    if(inputFile != NULL)
    {
        inputFile->pulseDict = pulseDict;
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