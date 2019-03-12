
//
// Created by nlv09165 on 18/07/2018.
//
#include <stdlib.h>

#include <ius.h>
#include <iusIqPatternListADT.h>
#include <iusIqPatternPrivate.h>
#include <iusIqPatternListPrivate.h>

// ADT
//struct IusIqPatternList
//{
//	int numPatterns;
//	iuiqpa_t *pIqPatterns;
//	IUS_BOOL deepDelete;
//	iudmd_t demodulationDict;
//	iurcmd_t receiveChannelMapDict;
//
//};

iuiqpal_t iusIqPatternListCreate
(
	int numPatterns,
	iudmd_t demodulationDict,
	iurcmd_t receiveChannelMapDict
)
{
	
	IUS_ERR_EVAL_N_RETURN(numPatterns <= 0, IUIQPAL_INVALID);
	//IUS_ERR_CHECK_NULL_N_RETURN(demodulationDict, IUIQPAL_INVALID);        //can be NULL
	//IUS_ERR_CHECK_NULL_N_RETURN(receiveChannelMapDict, IUIQPAL_INVALID);    //can be NULL
	iuiqpal_t list = calloc(1, sizeof(IusIqPatternList));
	IUS_ERR_CHECK_NULL_N_RETURN(list, IUIQPAL_INVALID);

    list->deepDelete = IUS_FALSE;
	list->receiveChannelMapDict = receiveChannelMapDict;
	list->demodulationDict = demodulationDict;
	list->numPatterns = numPatterns;
	list->pIqPatterns = (iuiqpa_t *)calloc((size_t)numPatterns, sizeof(iuiqpa_t));
	if (list->pIqPatterns == NULL)
	{
		IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_MEMORY, IUS_ERR_MIN_ALLOC, "could not allocate %d patterns for list", numPatterns);
		list = IUIQPAL_INVALID;
	}
	return list;
}

int iusIqPatternListDeepDelete
(
	iuiqpal_t list
)
{
	IUS_ERR_CHECK_NULL_N_RETURN(list, IUS_ERR_VALUE);
	list->deepDelete = IUS_TRUE;
	return iusIqPatternListDelete(list);
}


int iusIqPatternListDelete
(
	iuiqpal_t list
)
{
	int index;
	IUS_ERR_CHECK_NULL_N_RETURN(list, IUS_ERR_VALUE);
	if (list->deepDelete == IUS_TRUE)
	{
		for (index = 0; index < list->numPatterns; index++)
		{
			iusIqPatternDelete(list->pIqPatterns[index]);
		}
	}
	free(list->pIqPatterns);
	free(list);
	return IUS_E_OK;
}


// operations
int iusIqPatternListCompare
(
	iuiqpal_t reference,
	iuiqpal_t actual
)
{
	int index;
	IUS_ERR_CHECK_NULL_N_RETURN(reference, IUS_FALSE);
	IUS_ERR_CHECK_NULL_N_RETURN(actual, IUS_FALSE);
	if (reference == actual) return IUS_TRUE;
	if (reference->numPatterns != actual->numPatterns) return IUS_FALSE;
	for (index = 0; index < actual->numPatterns; index++)
	{
		if (iusIqPatternCompare(reference->pIqPatterns[index], actual->pIqPatterns[index])
			== IUS_FALSE)
			return IUS_FALSE;
	}
	return IUS_TRUE;
}


int iusIqPatternListGetSize
(
	iuiqpal_t list
)
{
	return list->numPatterns;
}

iuiqpa_t iusIqPatternListGet
(
	iuiqpal_t list,
	int index
)
{
	if (index < 0) return NULL;
	if (list == NULL || index >= list->numPatterns) return NULL;
	return list->pIqPatterns[index];
}

IUS_BOOL iusIqPatternListValidateDimensions
(
	iuiqpal_t list,
	iuiqpa_t member
)
{
	IUS_UNUSED(list);
	IUS_UNUSED(member);
	// getSamplesPerLine of first item
	if (list == NULL) return IUS_FALSE;
	char *dmLabel1stItem = (char *)iusIqPatternGetDemodulationLabel(list->pIqPatterns[0]);
	iudm_t demodulation1stItem = iusDemodulationDictGet(list->demodulationDict, dmLabel1stItem);
	int numSamplesPerLine1stItem = iusDemodulationGetNumSamplesPerLine(demodulation1stItem);

	char *dmLabelNewItem = (char *)iusIqPatternGetDemodulationLabel(member);
	iudm_t demodulationNewItem = iusDemodulationDictGet(list->demodulationDict, dmLabelNewItem);
	int numSamplesPerLineNewItem = iusDemodulationGetNumSamplesPerLine(demodulationNewItem);
	if (numSamplesPerLine1stItem != numSamplesPerLineNewItem)
		return IUS_FALSE;


	char *rcmLabel1stItem = (char *)iusIqPatternGetChannelMapLabel(list->pIqPatterns[0]);
	iurcm_t channelMap1stItem = iusReceiveChannelMapDictGet(list->receiveChannelMapDict, rcmLabel1stItem);
	int numChannels1stItem = iusReceiveChannelMapGetNumChannels(channelMap1stItem);

	char *rcmLabelNewItem = (char *)iusIqPatternGetChannelMapLabel(member);
	iurcm_t channelMapNewItem = iusReceiveChannelMapDictGet(list->receiveChannelMapDict, rcmLabelNewItem);
	int numChannelsNewItem = iusReceiveChannelMapGetNumChannels(channelMapNewItem);
	if (numChannels1stItem != numChannelsNewItem)
		return IUS_FALSE;

	return IUS_TRUE;
}

int iusIqPatternListSet
(
	iuiqpal_t list,
	iuiqpa_t member,
	int index
)
{
	IUS_ERR_CHECK_NULL_N_RETURN(list, IUS_ERR_VALUE);
	IUS_ERR_CHECK_NULL_N_RETURN(member, IUS_ERR_VALUE);
	IUS_ERR_EVAL_N_RETURN(index < 0, IUS_ERR_VALUE);

	if (list == NULL || index >= list->numPatterns)
	{
		IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_VALUE, IUS_ERR_MIN_ARG_VALUE, "index %d out of range", index);
		return IUS_ERR_VALUE;
	}

	if (index > 0)
	{
		IUS_BOOL validDimensions = iusIqPatternListValidateDimensions(list, member);
		if (validDimensions == IUS_FALSE)
		{
			IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_VALUE, IUS_ERR_MIN_ARG_VALUE, "inconsistent dimensions for member %d", index);
			return IUS_ERR_VALUE;
		}
	}
	list->pIqPatterns[index] = member;
	return IUS_E_OK;
}


iuiqpal_t iusIqPatternListLoad
(
	hid_t handle
)
{
	char path[IUS_MAX_HDF5_PATH];
	int numPatterns, i;

	int status = iusHdf5ReadInt(handle, IUS_PATH_PATTERNLIST_SIZE, &(numPatterns));
	if (status != 0) return IUIQPAL_INVALID;

	iuiqpal_t patternList = iusIqPatternListCreate(numPatterns, NULL, NULL);
	iuiqpa_t pattern;

	// Load patterns
	for (i = 0; i < numPatterns; i++)
	{
		sprintf(path, IUS_PATH_PATTERNLIST_PATTERN, i);
		hid_t patternId = H5Gopen(handle, path, H5P_DEFAULT);
		pattern = iusIqPatternLoad(patternId);
		if (pattern == IUIQPA_INVALID)
		{
			break;
		}
		iusIqPatternListSet(patternList, pattern, i);
	}
	patternList->deepDelete = IUS_TRUE;
	return patternList;
}

IUS_BOOL iusIqPatternListFull
(
	iuiqpal_t list
)
{
	IUS_BOOL isFull = IUS_TRUE;
	int i;
	for (i = 0; i < list->numPatterns; i++)
	{
		if (list->pIqPatterns[i] == IUIQPA_INVALID)
		{
			isFull = IUS_FALSE;
			break;
		}
	}
	return isFull;
}

int iusIqPatternListSave
(
	iuiqpal_t list,
	hid_t handle
)
{
	int status = 0;
	int i, size;
	char path[IUS_MAX_HDF5_PATH];

	if (list == NULL)
		return IUS_ERR_VALUE;
	if (handle == H5I_INVALID_HID)
		return IUS_ERR_VALUE;
	if (iusIqPatternListFull(list) == IUS_FALSE)
		return IUS_ERR_VALUE;

	iuiqpa_t pattern;
	size = iusIqPatternListGetSize(list);
	status |= iusHdf5WriteInt(handle, IUS_INPUTFILE_PATH_PATTERNLIST_SIZE, &(size), 1);

	// iterate over source list elements and save'em
	for (i = 0; i < size; i++)
	{
		pattern = iusIqPatternListGet(list, i);
		if (pattern == IUIQPA_INVALID) continue;
		sprintf(path, IUS_PATH_PATTERNLIST_PATTERN, i);
		hid_t pattern_id = H5Gcreate(handle, path, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
		status = iusIqPatternSave(pattern, pattern_id);
		H5Gclose(pattern_id);
		if (status != IUS_E_OK) break;
	}

	//status |= H5Gclose(patternList_id);
	return status;
}


