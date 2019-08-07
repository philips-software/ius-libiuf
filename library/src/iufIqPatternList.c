#include <stdlib.h>

#include <iuf.h>
#include <iufIqPatternListADT.h>
#include <iufIqPatternPrivate.h>
#include <iufIqPatternListPrivate.h>

// ADT
//struct IufIqPatternList
//{
//	int numPatterns;
//	iuiqpa_t *pIqPatterns;
//	IUF_BOOL deepDelete;
//	iudmd_t demodulationDict;
//	iurcmd_t receiveChannelMapDict;
//
//};

iuiqpal_t iufIqPatternListCreate
(
	int numPatterns,
	iudmd_t demodulationDict,
	iurcmd_t receiveChannelMapDict
)
{
	
	IUF_ERR_EVAL_N_RETURN(numPatterns <= 0, IUIQPAL_INVALID);
	//IUF_ERR_CHECK_NULL_N_RETURN(demodulationDict, IUIQPAL_INVALID);        //can be NULL
	//IUF_ERR_CHECK_NULL_N_RETURN(receiveChannelMapDict, IUIQPAL_INVALID);    //can be NULL
	iuiqpal_t list = calloc(1, sizeof(IufIqPatternList));
	IUF_ERR_CHECK_NULL_N_RETURN(list, IUIQPAL_INVALID);

    list->deepDelete = IUF_FALSE;
	list->receiveChannelMapDict = receiveChannelMapDict;
	list->demodulationDict = demodulationDict;
	list->numPatterns = numPatterns;
	list->pIqPatterns = (iuiqpa_t *)calloc((size_t)numPatterns, sizeof(iuiqpa_t));
	if (list->pIqPatterns == NULL)
	{
		IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_MEMORY, IUF_ERR_MIN_ALLOC, "could not allocate %d patterns for list", numPatterns);
		list = IUIQPAL_INVALID;
	}
	return list;
}

int iufIqPatternListDeepDelete
(
	iuiqpal_t list
)
{
	IUF_ERR_CHECK_NULL_N_RETURN(list, IUF_ERR_VALUE);
	list->deepDelete = IUF_TRUE;
	return iufIqPatternListDelete(list);
}


int iufIqPatternListDelete
(
	iuiqpal_t list
)
{
	int index;
	IUF_ERR_CHECK_NULL_N_RETURN(list, IUF_ERR_VALUE);
	if (list->deepDelete == IUF_TRUE)
	{
		for (index = 0; index < list->numPatterns; index++)
		{
			iufIqPatternDelete(list->pIqPatterns[index]);
		}
	}
	free(list->pIqPatterns);
	free(list);
	return IUF_E_OK;
}


// operations
int iufIqPatternListCompare
(
	iuiqpal_t reference,
	iuiqpal_t actual
)
{
	int index;
	IUF_ERR_CHECK_NULL_N_RETURN(reference, IUF_FALSE);
	IUF_ERR_CHECK_NULL_N_RETURN(actual, IUF_FALSE);
	if (reference == actual) return IUF_TRUE;
	if (reference->numPatterns != actual->numPatterns) return IUF_FALSE;
	for (index = 0; index < actual->numPatterns; index++)
	{
		if (iufIqPatternCompare(reference->pIqPatterns[index], actual->pIqPatterns[index])
			== IUF_FALSE)
			return IUF_FALSE;
	}
	return IUF_TRUE;
}


int iufIqPatternListGetSize
(
	iuiqpal_t list
)
{
	return list->numPatterns;
}

iuiqpa_t iufIqPatternListGet
(
	iuiqpal_t list,
	int index
)
{
	if (index < 0) return NULL;
	if (list == NULL || index >= list->numPatterns) return NULL;
	return list->pIqPatterns[index];
}

IUF_BOOL iufIqPatternListValidateDimensions
(
	iuiqpal_t list,
	iuiqpa_t member
)
{
	IUF_UNUSED(list);
	IUF_UNUSED(member);
	// getSamplesPerLine of first item
	if (list == NULL) return IUF_FALSE;
	char *dmLabel1stItem = (char *)iufIqPatternGetDemodulationLabel(list->pIqPatterns[0]);
	iudm_t demodulation1stItem = iufDemodulationDictGet(list->demodulationDict, dmLabel1stItem);
	int numSamplesPerLine1stItem = iufDemodulationGetNumSamplesPerLine(demodulation1stItem);

	char *dmLabelNewItem = (char *)iufIqPatternGetDemodulationLabel(member);
	iudm_t demodulationNewItem = iufDemodulationDictGet(list->demodulationDict, dmLabelNewItem);
	int numSamplesPerLineNewItem = iufDemodulationGetNumSamplesPerLine(demodulationNewItem);
	if (numSamplesPerLine1stItem != numSamplesPerLineNewItem)
		return IUF_FALSE;


	char *rcmLabel1stItem = (char *)iufIqPatternGetChannelMapLabel(list->pIqPatterns[0]);
	iurcm_t channelMap1stItem = iufReceiveChannelMapDictGet(list->receiveChannelMapDict, rcmLabel1stItem);
	int numChannels1stItem = iufReceiveChannelMapGetNumChannels(channelMap1stItem);

	char *rcmLabelNewItem = (char *)iufIqPatternGetChannelMapLabel(member);
	iurcm_t channelMapNewItem = iufReceiveChannelMapDictGet(list->receiveChannelMapDict, rcmLabelNewItem);
	int numChannelsNewItem = iufReceiveChannelMapGetNumChannels(channelMapNewItem);
	if (numChannels1stItem != numChannelsNewItem)
		return IUF_FALSE;

	return IUF_TRUE;
}

int iufIqPatternListSet
(
	iuiqpal_t list,
	iuiqpa_t member,
	int index
)
{
	IUF_ERR_CHECK_NULL_N_RETURN(list, IUF_ERR_VALUE);
	IUF_ERR_CHECK_NULL_N_RETURN(member, IUF_ERR_VALUE);
	IUF_ERR_EVAL_N_RETURN(index < 0, IUF_ERR_VALUE);

	if (list == NULL || index >= list->numPatterns)
	{
		IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_VALUE, IUF_ERR_MIN_ARG_VALUE, "index %d out of range", index);
		return IUF_ERR_VALUE;
	}

	if (index > 0)
	{
		IUF_BOOL validDimensions = iufIqPatternListValidateDimensions(list, member);
		if (validDimensions == IUF_FALSE)
		{
			IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_VALUE, IUF_ERR_MIN_ARG_VALUE, "inconsistent dimensions for member %d", index);
			return IUF_ERR_VALUE;
		}
	}
	list->pIqPatterns[index] = member;
	return IUF_E_OK;
}


iuiqpal_t iufIqPatternListLoad
(
	hid_t handle
)
{
	char path[IUF_MAX_HDF5_PATH];
	int numPatterns, i;

	int status = iufHdf5ReadInt(handle, IUF_PATH_PATTERNLIST_SIZE, &(numPatterns));
	if (status != 0) return IUIQPAL_INVALID;

	iuiqpal_t patternList = iufIqPatternListCreate(numPatterns, NULL, NULL);
	iuiqpa_t pattern;

	// Load patterns
	for (i = 0; i < numPatterns; i++)
	{
		snprintf(path, sizeof(path), IUF_PATH_PATTERNLIST_PATTERN, i);
		hid_t patternId = H5Gopen(handle, path, H5P_DEFAULT);
		pattern = iufIqPatternLoad(patternId);
		if (pattern == IUIQPA_INVALID)
		{
			break;
		}
		iufIqPatternListSet(patternList, pattern, i);
	}
	patternList->deepDelete = IUF_TRUE;
	return patternList;
}

IUF_BOOL iufIqPatternListFull
(
	iuiqpal_t list
)
{
	IUF_BOOL isFull = IUF_TRUE;
	int i;
	for (i = 0; i < list->numPatterns; i++)
	{
		if (list->pIqPatterns[i] == IUIQPA_INVALID)
		{
			isFull = IUF_FALSE;
			break;
		}
	}
	return isFull;
}

int iufIqPatternListSave
(
	iuiqpal_t list,
	hid_t handle
)
{
	int status = 0;
	int i, size;
	char path[IUF_MAX_HDF5_PATH];

	if (list == NULL)
		return IUF_ERR_VALUE;
	if (handle == H5I_INVALID_HID)
		return IUF_ERR_VALUE;
	if (iufIqPatternListFull(list) == IUF_FALSE)
		return IUF_ERR_VALUE;

	iuiqpa_t pattern;
	size = iufIqPatternListGetSize(list);
	status |= iufHdf5WriteInt(handle, IUF_INPUTFILE_PATH_PATTERNLIST_SIZE, &(size), 1);

	// iterate over source list elements and save'em
	for (i = 0; i < size; i++)
	{
		pattern = iufIqPatternListGet(list, i);
		if (pattern == IUIQPA_INVALID) continue;
        snprintf(path, sizeof(path), IUF_PATH_PATTERNLIST_PATTERN, i);
		hid_t pattern_id = H5Gcreate(handle, path, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
		status = iufIqPatternSave(pattern, pattern_id);
		H5Gclose(pattern_id);
		if (status != IUF_E_OK) break;
	}

	//status |= H5Gclose(patternList_id);
	return status;
}


