//
// Created by nlv12901 on 08/02/2019.
//
#include <stdlib.h>
#include <string.h>

#include <hashmap.h>

#include <ius.h>
#include <iusDimensionListPrivate.h>

#define IUS_PATH_DIMENSIONLIST_SIZE "Size"
#define IUS_PATH_DIMENSIONLIST_DIMENSION "Dimension[%d]"

// ADT
struct IusDimensionList
{
	int numDimensions;
	iudim_t *pDimensions;

};

// ADT
iudiml_t iusDimensionListCreate
(
	int numDimensions
)
{
	if (numDimensions <= 0)
	{
		IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_VALUE, IUS_ERR_MIN_ARG_VALUE,
			"numPatterns argument should be > 0, but was: '%d'", numDimensions);
		return IUDIML_INVALID;
	}

	iudiml_t list = calloc(1, sizeof(IusDimensionList));
	IUS_ERR_ALLOC_NULL_N_RETURN(list, IusDimensionList, IUDIML_INVALID);

	list->numDimensions = numDimensions;
	list->pDimensions = (iudim_t *)calloc((size_t)numDimensions, sizeof(iudim_t));
	if (list->pDimensions == NULL)
	{
		IUS_ERROR_PUSH(IUS_ERR_MAJ_MEMORY, IUS_ERR_MIN_ALLOC, "calloc failed for numDimensions");
		free(list);
		list = IUDIML_INVALID;
	}
	return list;
}

int iusDimensionListDeepDelete
(
	iudiml_t list
)
{
	int index;
	IUS_ERR_CHECK_NULL_N_RETURN(list, IUS_ERR_VALUE);
	for (index = 0; index < list->numDimensions; index++)
	{
		iusDimensionDelete(list->pDimensions[index]);
	}
	free(list->pDimensions);
	free(list);
	return IUS_E_OK;
}


int iusDimensionListDelete
(
	iudiml_t list
)
{
	IUS_ERR_CHECK_NULL_N_RETURN(list, IUS_ERR_VALUE);

	free(list->pDimensions);
	free(list);
	return IUS_E_OK;
}


// operations
int iusDimensionListCompare
(
	iudiml_t reference,
	iudiml_t actual
)
{
	int index;
	if (reference == actual) return IUS_TRUE;
	if (reference == NULL || actual == NULL) return IUS_FALSE;
	if (reference->numDimensions != actual->numDimensions) return IUS_FALSE;
	for (index = 0; index < actual->numDimensions; index++)
	{
		if (iusDimensionCompare(reference->pDimensions[index], actual->pDimensions[index])	
			  == IUS_FALSE)
			return IUS_FALSE;
	}
	return IUS_TRUE;
}


size_t iusDimensionListGetSize
(
	iudiml_t list
)
{
	IUS_ERR_CHECK_NULL_N_RETURN(list, IUS_ERR_VALUE);
	return list->numDimensions;
}

iudim_t iusDimensionListGet
(
	iudiml_t list,
	int index
)
{
	IUS_ERR_CHECK_NULL_N_RETURN(list, IUDIM_INVALID);
	if (index >= list->numDimensions || index < 0)
	{
		IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_VALUE, IUS_ERR_MIN_ARG_VALUE,
			"index >= 0 && index < %d (numPatterns) but was '%d'", list->numDimensions, index);
		return IUDIM_INVALID;
	}
	return list->pDimensions[index];
}


int iusDimensionListSet
(
	iudiml_t list,
	int index,
	iudim_t member
	
)
{
	IUS_ERR_CHECK_NULL_N_RETURN(list, IUS_ERR_VALUE);
	if (index >= list->numDimensions || index < 0)
	{
		IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_VALUE, IUS_ERR_MIN_ARG_VALUE,
			"index >= 0 && index < %d (numDimensions) but was '%d'", list->numDimensions, index);
		return IUS_ERR_VALUE;
	}

	list->pDimensions[index] = member;
	return IUS_E_OK;
}


iudiml_t iusDimensionListLoad
(
	hid_t handle
)
{
	char path[IUS_MAX_HDF5_PATH];
	int numDimensions, i;

	IUS_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUDIML_INVALID);

	int status = iusHdf5ReadInt(handle, IUS_PATH_DIMENSIONLIST_SIZE, &(numDimensions));
	if (status != 0) return IUDIML_INVALID;

	iudiml_t dimensionList = iusDimensionListCreate(numDimensions);
	iudim_t dimension;

	// Load patterns
	for (i = 0; i < numDimensions; i++)
	{
		sprintf(path, IUS_PATH_DIMENSIONLIST_DIMENSION, i);
		hid_t dimensionId = H5Gopen(handle, path, H5P_DEFAULT);
		dimension = iusDimensionLoad(dimensionId);
		if (dimension == IUDIM_INVALID)
		{
			break;
		}
		iusDimensionListSet(dimensionList, i, dimension);
	}
	return dimensionList;
}

IUS_BOOL iusDimensionListComplete
(
	iudiml_t list
)
{
	IUS_BOOL isComplete = IUS_TRUE;
	int i;

	IUS_ERR_CHECK_NULL_N_RETURN(list, IUS_FALSE);

	for (i = 0; i < list->numDimensions; i++)
	{
		if (list->pDimensions[i] == IUDIM_INVALID)
		{
			isComplete = IUS_FALSE;
			break;
		}
	}
	return isComplete;
}

int iusDimensionListSave
(
	iudiml_t list,
	hid_t handle
)
{
	int status = 0;
	int i, size;
	char path[IUS_MAX_HDF5_PATH];
	IUS_ERR_CHECK_NULL_N_RETURN(list, IUS_ERR_VALUE);
	IUS_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUS_ERR_VALUE);

	if (iusDimensionListComplete(list) == IUS_FALSE)
	{
		IUS_ERROR_PUSH(IUS_ERR_MAJ_VALUE, IUS_ERR_MIN_ARG_VALUE, "argument list (dimension list) was not completely filled out");
		return IUS_ERR_VALUE;
	}

	iudim_t dimension;
	size = (int)iusDimensionListGetSize(list);
	status |= iusHdf5WriteInt(handle, IUS_PATH_DIMENSIONLIST_SIZE, &(size), 1);
	if (status != 0)
	{
		IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_HDF5, IUS_ERR_MIN_HDF5, "write failed for %s",
			IUS_PATH_DIMENSIONLIST_SIZE);
		return IUS_ERR_VALUE;
	}

	// iterate over list elements and save'em
	for (i = 0; i < size; i++)
	{
		dimension = iusDimensionListGet(list, i);
		if (dimension == IUDIM_INVALID) continue;
		sprintf(path, IUS_PATH_DIMENSIONLIST_DIMENSION, i);
		hid_t dimension_id = H5Gcreate(handle, path, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
		status = iusDimensionSave(dimension, dimension_id);
		H5Gclose(dimension_id);
		if (status != IUS_E_OK) break;
	}
	return status;
}


