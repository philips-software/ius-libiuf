#include <stdlib.h>
#include <string.h>

#include <hashmap.h>

#include <iuf.h>
#include <iufDimensionListPrivate.h>

#define IUF_PATH_DIMENSIONLIST_SIZE "Size"
#define IUF_PATH_DIMENSIONLIST_DIMENSION "Dimension[%d]"

// ADT
struct IufDimensionList
{
	int numDimensions;
	iudim_t *pDimensions;

};

// ADT
iudiml_t iufDimensionListCreate
(
	int numDimensions
)
{
	if (numDimensions <= 0)
	{
		IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_VALUE, IUF_ERR_MIN_ARG_VALUE,
			"numPatterns argument should be > 0, but was: '%d'", numDimensions);
		return IUDIML_INVALID;
	}

	iudiml_t list = calloc(1, sizeof(IufDimensionList));
	IUF_ERR_ALLOC_NULL_N_RETURN(list, IufDimensionList, IUDIML_INVALID);

	list->numDimensions = numDimensions;
	list->pDimensions = (iudim_t *)calloc((size_t)numDimensions, sizeof(iudim_t));
	if (list->pDimensions == NULL)
	{
		IUF_ERROR_PUSH(IUF_ERR_MAJ_MEMORY, IUF_ERR_MIN_ALLOC, "calloc failed for numDimensions");
		free(list);
		list = IUDIML_INVALID;
	}
	return list;
}

int iufDimensionListDeepDelete
(
	iudiml_t list
)
{
	int index;
	IUF_ERR_CHECK_NULL_N_RETURN(list, IUF_ERR_VALUE);
	for (index = 0; index < list->numDimensions; index++)
	{
		iufDimensionDelete(list->pDimensions[index]);
	}
	free(list->pDimensions);
	free(list);
	return IUF_E_OK;
}


int iufDimensionListDelete
(
	iudiml_t list
)
{
	IUF_ERR_CHECK_NULL_N_RETURN(list, IUF_ERR_VALUE);

	free(list->pDimensions);
	free(list);
	return IUF_E_OK;
}


// operations
int iufDimensionListCompare
(
	iudiml_t reference,
	iudiml_t actual
)
{
	int index;
	if (reference == actual) return IUF_TRUE;
	if (reference == NULL || actual == NULL) return IUF_FALSE;
	if (reference->numDimensions != actual->numDimensions) return IUF_FALSE;
	for (index = 0; index < actual->numDimensions; index++)
	{
		if (iufDimensionCompare(reference->pDimensions[index], actual->pDimensions[index])
			  == IUF_FALSE)
			return IUF_FALSE;
	}
	return IUF_TRUE;
}


size_t iufDimensionListGetSize
(
	iudiml_t list
)
{
	IUF_ERR_CHECK_NULL_N_RETURN(list, IUF_ERR_VALUE);
	return list->numDimensions;
}

iudim_t iufDimensionListGet
(
	iudiml_t list,
	int index
)
{
	IUF_ERR_CHECK_NULL_N_RETURN(list, IUDIM_INVALID);
	if (index >= list->numDimensions || index < 0)
	{
		IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_VALUE, IUF_ERR_MIN_ARG_VALUE,
			"index >= 0 && index < %d (numPatterns) but was '%d'", list->numDimensions, index);
		return IUDIM_INVALID;
	}
	return list->pDimensions[index];
}


int iufDimensionListSet
(
	iudiml_t list,
	int index,
	iudim_t member
	
)
{
	IUF_ERR_CHECK_NULL_N_RETURN(list, IUF_ERR_VALUE);
	if (index >= list->numDimensions || index < 0)
	{
		IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_VALUE, IUF_ERR_MIN_ARG_VALUE,
			"index >= 0 && index < %d (numDimensions) but was '%d'", list->numDimensions, index);
		return IUF_ERR_VALUE;
	}

	list->pDimensions[index] = member;
	return IUF_E_OK;
}


iudiml_t iufDimensionListLoad
(
	hid_t handle
)
{
	char path[IUF_MAX_HDF5_PATH];
	int numDimensions, i;

	IUF_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUDIML_INVALID);

	int status = iufHdf5ReadInt(handle, IUF_PATH_DIMENSIONLIST_SIZE, &(numDimensions));
	if (status != 0) return IUDIML_INVALID;

	iudiml_t dimensionList = iufDimensionListCreate(numDimensions);
	iudim_t dimension;

	// Load patterns
	for (i = 0; i < numDimensions; i++)
	{
        snprintf(path, sizeof(path), IUF_PATH_DIMENSIONLIST_DIMENSION, i);
		hid_t dimensionId = H5Gopen(handle, path, H5P_DEFAULT);
		dimension = iufDimensionLoad(dimensionId);
		if (dimension == IUDIM_INVALID)
		{
			break;
		}
		iufDimensionListSet(dimensionList, i, dimension);
	}
	return dimensionList;
}

IUF_BOOL iufDimensionListComplete
(
	iudiml_t list
)
{
	IUF_BOOL isComplete = IUF_TRUE;
	int i;

	IUF_ERR_CHECK_NULL_N_RETURN(list, IUF_FALSE);

	for (i = 0; i < list->numDimensions; i++)
	{
		if (list->pDimensions[i] == IUDIM_INVALID)
		{
			isComplete = IUF_FALSE;
			break;
		}
	}
	return isComplete;
}

int iufDimensionListSave
(
	iudiml_t list,
	hid_t handle
)
{
	int status = 0;
	int i, size;
	char path[IUF_MAX_HDF5_PATH];
	IUF_ERR_CHECK_NULL_N_RETURN(list, IUF_ERR_VALUE);
	IUF_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUF_ERR_VALUE);

	if (iufDimensionListComplete(list) == IUF_FALSE)
	{
		IUF_ERROR_PUSH(IUF_ERR_MAJ_VALUE, IUF_ERR_MIN_ARG_VALUE, "argument list (dimension list) was not completely filled out");
		return IUF_ERR_VALUE;
	}

	iudim_t dimension;
	size = (int)iufDimensionListGetSize(list);
	status |= iufHdf5WriteInt(handle, IUF_PATH_DIMENSIONLIST_SIZE, &(size), 1);
	if (status != 0)
	{
		IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_HDF5, IUF_ERR_MIN_HDF5, "write failed for %s",
			IUF_PATH_DIMENSIONLIST_SIZE);
		return IUF_ERR_VALUE;
	}

	// iterate over list elements and save'em
	for (i = 0; i < size; i++)
	{
		dimension = iufDimensionListGet(list, i);
		if (dimension == IUDIM_INVALID) continue;
        snprintf(path, sizeof(path), IUF_PATH_DIMENSIONLIST_DIMENSION, i);
		hid_t dimension_id = H5Gcreate(handle, path, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
		status = iufDimensionSave(dimension, dimension_id);
		H5Gclose(dimension_id);
		if (status != IUF_E_OK) break;
	}
	return status;
}


