//
// Created by nlv09165 on 18/07/2018.
//
#include <stdlib.h>

#include <ius.h>
#include <iusPatternPrivate.h>
#include <iusPatternListPrivate.h>

// ADT
struct IusPatternList
{
	int numPatterns;
	iupa_t *   pPatterns;
	IUS_BOOL loadedFromFile;
	iursd_t receiveSettingsDict;
	iurcmd_t receiveChannelMapDict;

};

// ADT
iupal_t iusPatternListCreate
(
	int numPatterns,
	iursd_t receiveSettingsDict,
	iurcmd_t receiveChannelMapDict
)
{
	iupal_t list = calloc(1, sizeof(IusPatternList));
	if (list != NULL)
	{
		list->loadedFromFile = IUS_FALSE;
		list->receiveChannelMapDict = receiveChannelMapDict;
		list->receiveSettingsDict = receiveSettingsDict;
		list->numPatterns = numPatterns;
		list->pPatterns = (iupa_t *)calloc((size_t)numPatterns, sizeof(iupa_t));
		if (list->pPatterns == NULL)
		{
		
			IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_MEMORY, IUS_ERR_MIN_ALLOC, "could not alocate patterns for list");
			list = NULL;
		}
	}
	return list;
}

int iusPatternListDeepDelete
(
	iupal_t list
)
{
	if (list == NULL) return IUS_ERR_VALUE;
	list->loadedFromFile = IUS_TRUE;
	return iusPatternListDelete(list);
}


int iusPatternListDelete
(
	iupal_t list
)
{
	int index;
	if (list == NULL) return IUS_ERR_VALUE;
	if (list->loadedFromFile == IUS_TRUE)
	{
		for (index = 0; index < list->numPatterns; index++)
		{
			iusPatternDelete(list->pPatterns[index]);
		}
	}
	free(list->pPatterns);
	free(list);
	return IUS_E_OK;
}


// operations
int iusPatternListCompare
(
	iupal_t reference,
	iupal_t actual
)
{
	int index;
	if (reference == actual) return IUS_TRUE;
	if (reference == NULL || actual == NULL) return IUS_FALSE;
	if (reference->numPatterns != actual->numPatterns) return IUS_FALSE;
	for (index = 0; index < actual->numPatterns; index++)
	{
		if (iusPatternCompare(reference->pPatterns[index], actual->pPatterns[index])
			== IUS_FALSE)
			return IUS_FALSE;
	}
	return IUS_TRUE;
}


int iusPatternListGetSize
(
	iupal_t list
)
{
	return list->numPatterns;
}

iupa_t iusPatternListGet
(
	iupal_t list,
	int index
)
{
	if (index < 0) return NULL;
	if (list == NULL || index >= list->numPatterns) return NULL;
	return list->pPatterns[index];
}

IUS_BOOL iusPatternListValidateDimensions
(
	iupal_t list,
	iupa_t member
)
{
	IUS_UNUSED(list);
	IUS_UNUSED(member);
	// getSamplesPerLine of first item
	if (list == NULL) return IUS_FALSE;
	char *rsLabel1stItem = (char *)iusPatternGetReceivesettingsLabel(list->pPatterns[0]);
	iurs_t receiveSettings1stItem = iusReceiveSettingsDictGet(list->receiveSettingsDict, rsLabel1stItem);
	int numSamplesPerLine1stItem = iusReceiveSettingsGetNumSamplesPerLine(receiveSettings1stItem);

	char *rsLabelNewItem = (char *)iusPatternGetReceivesettingsLabel(member);
	iurs_t receiveSettingsNewItem = iusReceiveSettingsDictGet(list->receiveSettingsDict, rsLabelNewItem);
	int numSamplesPerLineNewItem = iusReceiveSettingsGetNumSamplesPerLine(receiveSettingsNewItem);
	if (numSamplesPerLine1stItem != numSamplesPerLineNewItem)
		return IUS_FALSE;


	char *rcmLabel1stItem = (char *)iusPatternGetChannelMapLabel(list->pPatterns[0]);
	iurcm_t channelMap1stItem = iusReceiveChannelMapDictGet(list->receiveChannelMapDict, rcmLabel1stItem);
	int numChannels1stItem = iusReceiveChannelMapGetNumChannels(channelMap1stItem);

	char *rcmLabelNewItem = (char *)iusPatternGetChannelMapLabel(member);
	iurcm_t channelMapNewItem = iusReceiveChannelMapDictGet(list->receiveChannelMapDict, rcmLabelNewItem);
	int numChannelsNewItem = iusReceiveChannelMapGetNumChannels(channelMapNewItem);
	if (numChannels1stItem != numChannelsNewItem)
		return IUS_FALSE;

	return IUS_TRUE;
}

int iusPatternListSet
(
	iupal_t list,
	iupa_t member,
	int index
)
{
	if (index < 0) return IUS_ERR_VALUE;
	if (list == NULL || index >= list->numPatterns)
	{
		IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_VALUE, IUS_ERR_MIN_ARG_VALUE , "index %d out of range", index);
		return IUS_ERR_VALUE;
	}
	if (index > 0)
	{
		IUS_BOOL validDimensions = iusPatternListValidateDimensions(list, member);
		if (validDimensions == IUS_FALSE)
		{
			IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_VALUE, IUS_ERR_MIN_ARG_VALUE, "inconsistent dimensions for member %d", index);
			return IUS_ERR_VALUE;
		}
	}
	list->pPatterns[index] = member;
	return IUS_E_OK;
}


iupal_t iusPatternListLoad
(
	hid_t handle
)
{
	char path[IUS_MAX_HDF5_PATH];
	int numPatterns, i;

	int status = iusHdf5ReadInt(handle, IUS_INPUTFILE_PATH_PATTERNLIST_SIZE, &(numPatterns));
	if (status != 0) return IUPAL_INVALID;

	iupal_t patternList = iusPatternListCreate(numPatterns, NULL, NULL);
	iupa_t pattern;

	// Load patterns
	for (i = 0; i < numPatterns; i++)
	{
		sprintf(path, IUS_INPUTFILE_PATH_PATTERNLIST_PATTERN, i);
		hid_t patternId = H5Gopen(handle, path, H5P_DEFAULT);
		pattern = iusPatternLoad(patternId);
		if (pattern == IUPA_INVALID)
		{
			break;
		}
		iusPatternListSet(patternList, pattern, i);
	}
	patternList->loadedFromFile = IUS_TRUE;
	return patternList;
}

IUS_BOOL iusPatternListFull
(
	iupal_t list
)
{
	IUS_BOOL isFull = IUS_TRUE;
	int i;
	for (i = 0; i < list->numPatterns; i++)
	{
		if (list->pPatterns[i] == IUPA_INVALID)
		{
			isFull = IUS_FALSE;
			break;
		}
	}
	return isFull;
}

int iusPatternListSave
(
	iupal_t list,
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
	if (iusPatternListFull(list) == IUS_FALSE)
		return IUS_ERR_VALUE;

	iupa_t pattern;
	size = iusPatternListGetSize(list);
	status |= iusHdf5WriteInt(handle, IUS_INPUTFILE_PATH_PATTERNLIST_SIZE, &(size), 1);

	// iterate over source list elements and save'em
	for (i = 0; i < size; i++)
	{
		pattern = iusPatternListGet(list, i);
		if (pattern == IUPA_INVALID) continue;
		sprintf(path, IUS_INPUTFILE_PATH_PATTERNLIST_PATTERN, i);
		hid_t pattern_id = H5Gcreate(handle, path, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
		status = iusPatternSave(pattern, pattern_id);
		H5Gclose(pattern_id);
		if (status != IUS_E_OK) break;
	}

	//status |= H5Gclose(patternList_id);
	return status;
}

