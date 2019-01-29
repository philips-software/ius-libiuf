
//
// Created by nlv09165 on 11/07/2018.
//
#include <stdlib.h>
#include <string.h>

#include <ius.h>
#include <iusCompoundFile.h>
#include <iusCompoundFileStructure.h>
#include <iusCompoundFileInstance.h>
#include <iusDataStream.h>
#include <iusHistoryNodePrivate.h>

struct IusCompoundFileInstance
{
	iufl_t frameList;
	int IusVersion;                       /**< The version of input file format */


										  //  state variables
	hid_t               handle;           /**< HDF5 file handle     */
	const char          *pFilename;       /**< the filename         */
	iuds_t             dataStream;        /**< Contains dataset administration */
	IUS_BOOL           deepDelete;
};

struct IusCompoundFile
{
	iuhn_t history;
};

iucfi_t iusCompoundFileInstanceCreate
(
	void
)
{
	iucfi_t instanceData = (iucfi_t)calloc(1, sizeof(IusCompoundFileInstance));
	if (instanceData == NULL)
	{
		return IUCFI_INVALID;
	}

	return instanceData;
}

int iusCompoundFileInstanceDelete
(
	iucfi_t instance
)
{
	if (instance == NULL) return IUS_ERR_VALUE;

	free(instance);
	return IUS_E_OK;
}

// ADT
int iusCompoundFileDelete
(
	iuif_t compoundFile
)
{
	int status = IUS_ERR_VALUE;
	if (compoundFile != NULL)
	{
		iucfi_t instance = iusHistoryNodeGetInstanceData(compoundFile);
		iusCompoundFileInstanceDelete(instance);
		iusHistoryNodeDelete((iuhn_t)compoundFile);
		status = IUS_E_OK;
	}
	return status;
}

iucf_t iusCompoundFileCreate
(
	const char *filename
)
{
	if (filename == NULL)
	{
		fprintf(stderr, "iusCompoundFileAlloc: compound arguments can not be NULL \n");
		return IUIF_INVALID;
	}

	iucfi_t instanceData = iusCompoundFileInstanceCreate();
	if (instanceData == IUCFI_INVALID)
	{
		fprintf(stderr, "iusCompoundFileCreate: calloc of instance failed\n");
		return IUIF_INVALID;
	}

	instanceData->pFilename = strdup(filename);
	instanceData->handle = H5Fcreate(filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
	if (instanceData->handle == H5I_INVALID_HID)
	{
		iusCompoundFileInstanceDelete(instanceData);
		return IUIF_INVALID;
	}
	iuhn_t node = iusHistoryNodeCreate(IUS_COMPOUND_TYPE);
	iusHistoryNodeSetInstanceData(node, (void *)instanceData);
	return (iucf_t)node;
}


static iucfi_t compoundFileInstanceLoad
(
	iucfi_t instance
)
{
	if (instance == NULL) return IUCFI_INVALID;

	return instance;
}

void *iusCompoundFileInstanceLoad
(
	hid_t handle
)
{
	iucfi_t instance = iusCompoundFileInstanceCreate();
	iucfi_t new_instance;
	instance->handle = handle;
	new_instance = compoundFileInstanceLoad(instance);
	if (new_instance == IUCFI_INVALID)
	{
		iusCompoundFileInstanceDelete(instance);
		instance = new_instance;
	}
	instance->deepDelete = IUS_TRUE;
	return (void *)instance;
}


iuhn_t iusCompoundFileLoadNode
(
	hid_t handle
)
{
	iuhn_t node = iusHistoryNodeCreate(IUS_COMPOUND_TYPE);
	iucfi_t instance = iusCompoundFileInstanceCreate();
	instance->handle = handle;
	instance = compoundFileInstanceLoad(instance);
	iusHistoryNodeSetInstanceData(node, instance);
	return node;
}

iucf_t iusCompoundFileNodeLoad
(
	const char *pFilename
)
{
	if (pFilename == NULL)
	{
		fprintf(stderr, "iusCompoundFileAlloc: Compound arguments can not be NULL \n");
		return IUCF_INVALID;
	}

	// open  Hdf5 file using default properties.
	hid_t handle = H5Fopen(pFilename, H5F_ACC_RDONLY, H5P_DEFAULT);
	if (handle <= 0)
	{
		fprintf(stderr, "iusCompoundFileNodeLoad: could not create file: %s\n", pFilename);
		return IUCF_INVALID;
	}

	iucf_t compoundFile = (iucf_t)iusHistoryNodeLoad(handle);
	iucfi_t instance = iusHistoryNodeGetInstanceData((iuhn_t)compoundFile);
	instance->pFilename = strdup(pFilename);
	return compoundFile;
}

int iusCompoundFileSaveInstance
(
	hid_t handle,
	iucfi_t instanceData
)
{
	herr_t status = 0;
	if (!handle)
	{
		return IUS_ERR_VALUE;
	}
	status |= iusHdf5WriteInt(instanceData->handle, IUS_COMPOUNDFILE_PATH_IUSVERSION, &(instanceData->IusVersion), 1);
	return status;
}


int iusCompoundFileNodeSave
(
	iuif_t compoundFile
)
{
	herr_t status = 0;
	if (compoundFile == NULL) return IUS_ERR_VALUE;

	iucfi_t instance = iusHistoryNodeGetInstanceData((iuhn_t)compoundFile);
	status |= iusHistoryNodeSave(compoundFile, instance->handle);
	return status;
}


int iusCompoundFileClose
(
	iuif_t compoundFile
)
{
	int status = 0;
	if (compoundFile == IUCF_INVALID) return IUS_FALSE;
	IUS_ASSERT_MEMORY(compoundFile);

	iucfi_t instance = iusHistoryNodeGetInstanceData((iuhn_t)compoundFile);

	// Terminate access to the file.
	if (instance->handle != H5I_INVALID_HID)
	{
		status |= H5Fclose(instance->handle);
	}
	return status;
}

static int iusCompoundFileCompareInstance
(
	iucfi_t reference,
	iucfi_t actual
)
{
	if (reference == actual) return IUS_TRUE;
	if (reference == NULL || actual == NULL) return IUS_FALSE;
	if (reference->IusVersion != actual->IusVersion) return IUS_FALSE;
	if (strcmp(reference->pFilename, actual->pFilename) != 0) return IUS_FALSE;
	return IUS_TRUE;
}

int iusCompoundFileCompare
(
	iucf_t reference,
	iucf_t actual
)
{
	if (reference == actual) return IUS_TRUE;
	if (reference == NULL || actual == NULL) return IUS_FALSE;
	if (iusHistoryNodeCompareWithId(reference, actual) == IUS_FALSE) return IUS_FALSE;

	// Compare instance data
	iucfi_t refInstance = iusHistoryNodeGetInstanceData((iuhn_t)reference);
	iucfi_t actInstance = iusHistoryNodeGetInstanceData((iuhn_t)actual);
	IUS_BOOL equal = iusCompoundFileCompareInstance(refInstance, actInstance);
	if (equal == IUS_FALSE) return IUS_FALSE;


	return equal;
}


int iusCompoundFileSetFilename
(
	iuif_t compoundFile,
	char * fileName
)
{
	int status = IUS_ERR_VALUE;

	if (compoundFile != NULL)
	{
		iucfi_t instance = iusHistoryNodeGetInstanceData((iuhn_t)compoundFile);
		instance->pFilename = strdup(fileName);
		status = IUS_E_OK;
	}
	return status;
}

