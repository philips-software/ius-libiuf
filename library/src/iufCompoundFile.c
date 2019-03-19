
//
// Created by nlv09165 on 11/07/2018.
//
#include <stdlib.h>
#include <string.h>

#include <iuf.h>
#include <iufCompoundFile.h>
#include <iufCompoundFileStructure.h>
#include <iufCompoundFileInstance.h>
#include <iufDataStream.h>
#include <iufHistoryNodePrivate.h>

struct IufCompoundFileInstance
{
	iufl_t frameList;
	int IufVersion;                       /**< The version of input file format */


										  //  state variables
	hid_t               handle;           /**< HDF5 file handle     */
	const char          *pFilename;       /**< the filename         */
	iuds_t             dataStream;        /**< Contains dataset administration */
	IUF_BOOL           deepDelete;
};

struct IufCompoundFile
{
	iuhn_t history;
};

iucfi_t iufCompoundFileInstanceCreate
(
	void
)
{
	iucfi_t instanceData = (iucfi_t)calloc(1, sizeof(IufCompoundFileInstance));
	if (instanceData == NULL)
	{
		return IUCFI_INVALID;
	}

	return instanceData;
}

int iufCompoundFileInstanceDelete
(
	iucfi_t instance
)
{
	if (instance == NULL) return IUF_ERR_VALUE;

	free(instance);
	return IUF_E_OK;
}

// ADT
int iufCompoundFileDelete
(
	iuif_t compoundFile
)
{
	int status = IUF_ERR_VALUE;
	if (compoundFile != NULL)
	{
		iucfi_t instance = iufHistoryNodeGetInstanceData(compoundFile);
		iufCompoundFileInstanceDelete(instance);
		iufHistoryNodeDelete((iuhn_t)compoundFile);
		status = IUF_E_OK;
	}
	return status;
}

iucf_t iufCompoundFileCreate
(
	const char *filename
)
{
	if (filename == NULL)
	{
		fprintf(stderr, "iufCompoundFileAlloc: compound arguments can not be NULL \n");
		return IUIF_INVALID;
	}

	iucfi_t instanceData = iufCompoundFileInstanceCreate();
	if (instanceData == IUCFI_INVALID)
	{
		fprintf(stderr, "iufCompoundFileCreate: calloc of instance failed\n");
		return IUIF_INVALID;
	}

	instanceData->pFilename = strdup(filename);
	instanceData->handle = H5Fcreate(filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
	if (instanceData->handle == H5I_INVALID_HID)
	{
		iufCompoundFileInstanceDelete(instanceData);
		return IUIF_INVALID;
	}
	iuhn_t node = iufHistoryNodeCreate(IUF_COMPOUND_TYPE);
	iufHistoryNodeSetInstanceData(node, (void *)instanceData);
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

void *iufCompoundFileInstanceLoad
(
	hid_t handle
)
{
	iucfi_t instance = iufCompoundFileInstanceCreate();
	iucfi_t new_instance;
	instance->handle = handle;
	new_instance = compoundFileInstanceLoad(instance);
	if (new_instance == IUCFI_INVALID)
	{
		iufCompoundFileInstanceDelete(instance);
		instance = new_instance;
	}
	instance->deepDelete = IUF_TRUE;
	return (void *)instance;
}


iuhn_t iufCompoundFileLoadNode
(
	hid_t handle
)
{
	iuhn_t node = iufHistoryNodeCreate(IUF_COMPOUND_TYPE);
	iucfi_t instance = iufCompoundFileInstanceCreate();
	instance->handle = handle;
	instance = compoundFileInstanceLoad(instance);
	iufHistoryNodeSetInstanceData(node, instance);
	return node;
}

iucf_t iufCompoundFileNodeLoad
(
	const char *pFilename
)
{
	if (pFilename == NULL)
	{
		fprintf(stderr, "iufCompoundFileAlloc: Compound arguments can not be NULL \n");
		return IUCF_INVALID;
	}

	// open  Hdf5 file using default properties.
	hid_t handle = H5Fopen(pFilename, H5F_ACC_RDONLY, H5P_DEFAULT);
	if (handle <= 0)
	{
		fprintf(stderr, "iufCompoundFileNodeLoad: could not create file: %s\n", pFilename);
		return IUCF_INVALID;
	}

	iucf_t compoundFile = (iucf_t)iufHistoryNodeLoad(handle);
	iucfi_t instance = iufHistoryNodeGetInstanceData((iuhn_t)compoundFile);
	instance->pFilename = strdup(pFilename);
	return compoundFile;
}

int iufCompoundFileSaveInstance
(
	hid_t handle,
	iucfi_t instanceData
)
{
	herr_t status = 0;
	if (!handle)
	{
		return IUF_ERR_VALUE;
	}
	status |= iufHdf5WriteInt(instanceData->handle, IUF_COMPOUNDFILE_PATH_IUFVERSION, &(instanceData->IufVersion), 1);
	return status;
}


int iufCompoundFileNodeSave
(
	iuif_t compoundFile
)
{
	herr_t status = 0;
	if (compoundFile == NULL) return IUF_ERR_VALUE;

	iucfi_t instance = iufHistoryNodeGetInstanceData((iuhn_t)compoundFile);
	status |= iufHistoryNodeSave(compoundFile, instance->handle);
	return status;
}


int iufCompoundFileClose
(
	iuif_t compoundFile
)
{
	int status = 0;
	if (compoundFile == IUCF_INVALID) return IUF_FALSE;
	IUF_ASSERT_MEMORY(compoundFile);

	iucfi_t instance = iufHistoryNodeGetInstanceData((iuhn_t)compoundFile);

	// Terminate access to the file.
	if (instance->handle != H5I_INVALID_HID)
	{
		status |= H5Fclose(instance->handle);
	}
	return status;
}

static int iufCompoundFileCompareInstance
(
	iucfi_t reference,
	iucfi_t actual
)
{
	if (reference == actual) return IUF_TRUE;
	if (reference == NULL || actual == NULL) return IUF_FALSE;
	if (reference->IufVersion != actual->IufVersion) return IUF_FALSE;
	if (strcmp(reference->pFilename, actual->pFilename) != 0) return IUF_FALSE;
	return IUF_TRUE;
}

int iufCompoundFileCompare
(
	iucf_t reference,
	iucf_t actual
)
{
	if (reference == actual) return IUF_TRUE;
	if (reference == NULL || actual == NULL) return IUF_FALSE;
	if (iufHistoryNodeCompareWithId(reference, actual) == IUF_FALSE) return IUF_FALSE;

	// Compare instance data
	iucfi_t refInstance = iufHistoryNodeGetInstanceData((iuhn_t)reference);
	iucfi_t actInstance = iufHistoryNodeGetInstanceData((iuhn_t)actual);
	IUF_BOOL equal = iufCompoundFileCompareInstance(refInstance, actInstance);
	if (equal == IUF_FALSE) return IUF_FALSE;


	return equal;
}


int iufCompoundFileSetFilename
(
	iuif_t compoundFile,
	char * fileName
)
{
	int status = IUF_ERR_VALUE;

	if (compoundFile != NULL)
	{
		iucfi_t instance = iufHistoryNodeGetInstanceData((iuhn_t)compoundFile);
		instance->pFilename = strdup(fileName);
		status = IUF_E_OK;
	}
	return status;
}

