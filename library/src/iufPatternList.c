#include <stdlib.h>

#include <iuf.h>
#include <iufPatternPrivate.h>
#include <iufPatternListPrivate.h>

// ADT
struct IufPatternList
{
	int numPatterns;
	iupa_t *   pPatterns;
	IUF_BOOL loadedFromFile;
	iursd_t receiveSettingsDict;
	iurcmd_t receiveChannelMapDict;

};

// ADT
iupal_t iufPatternListCreate
(
	int numPatterns,
	iursd_t receiveSettingsDict,
	iurcmd_t receiveChannelMapDict
)
{
	iupal_t list = calloc(1, sizeof(IufPatternList));
	if (list != NULL)
	{
		list->loadedFromFile = IUF_FALSE;
		list->receiveChannelMapDict = receiveChannelMapDict;
		list->receiveSettingsDict = receiveSettingsDict;
		list->numPatterns = numPatterns;
		list->pPatterns = (iupa_t *)calloc((size_t)numPatterns, sizeof(iupa_t));
		if (list->pPatterns == NULL)
		{
		
			IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_MEMORY, IUF_ERR_MIN_ALLOC, "could not allocate %d patterns for list", numPatterns);
			list = NULL;
		}
	}
	return list;
}

int iufPatternListDeepDelete
(
	iupal_t list
)
{
	if (list == NULL) return IUF_ERR_VALUE;
	list->loadedFromFile = IUF_TRUE;
	return iufPatternListDelete(list);
}


int iufPatternListDelete
(
	iupal_t list
)
{
	int index;
	if (list == NULL) return IUF_ERR_VALUE;
	if (list->loadedFromFile == IUF_TRUE)
	{
		for (index = 0; index < list->numPatterns; index++)
		{
			iufPatternDelete(list->pPatterns[index]);
		}
	}
	free(list->pPatterns);
	free(list);
	return IUF_E_OK;
}


// operations
int iufPatternListCompare
(
	iupal_t reference,
	iupal_t actual
)
{
	int index;
	if (reference == actual) return IUF_TRUE;
	if (reference == NULL || actual == NULL) return IUF_FALSE;
	if (reference->numPatterns != actual->numPatterns) return IUF_FALSE;
	for (index = 0; index < actual->numPatterns; index++)
	{
		if (iufPatternCompare(reference->pPatterns[index], actual->pPatterns[index])
			== IUF_FALSE)
			return IUF_FALSE;
	}
	return IUF_TRUE;
}


int iufPatternListGetSize
(
	iupal_t list
)
{
	return list->numPatterns;
}

iupa_t iufPatternListGet
(
	iupal_t list,
	int index
)
{
	if (index < 0) return NULL;
	if (list == NULL || index >= list->numPatterns) return NULL;
	return list->pPatterns[index];
}

IUF_BOOL iufPatternListValidateDimensions
(
	iupal_t list,
	iupa_t member
)
{
	IUF_UNUSED(list);
	IUF_UNUSED(member);
	// getSamplesPerLine of first item
	if (list == NULL) return IUF_FALSE;
	char *rsLabel1stItem = (char *)iufPatternGetReceivesettingsLabel(list->pPatterns[0]);
	iurs_t receiveSettings1stItem = iufReceiveSettingsDictGet(list->receiveSettingsDict, rsLabel1stItem);
	int numSamplesPerLine1stItem = iufReceiveSettingsGetNumSamplesPerLine(receiveSettings1stItem);

	char *rsLabelNewItem = (char *)iufPatternGetReceivesettingsLabel(member);
	iurs_t receiveSettingsNewItem = iufReceiveSettingsDictGet(list->receiveSettingsDict, rsLabelNewItem);
	int numSamplesPerLineNewItem = iufReceiveSettingsGetNumSamplesPerLine(receiveSettingsNewItem);
	if (numSamplesPerLine1stItem != numSamplesPerLineNewItem)
		return IUF_FALSE;


	char *rcmLabel1stItem = (char *)iufPatternGetChannelMapLabel(list->pPatterns[0]);
	iurcm_t channelMap1stItem = iufReceiveChannelMapDictGet(list->receiveChannelMapDict, rcmLabel1stItem);
	int numChannels1stItem = iufReceiveChannelMapGetNumChannels(channelMap1stItem);

	char *rcmLabelNewItem = (char *)iufPatternGetChannelMapLabel(member);
	iurcm_t channelMapNewItem = iufReceiveChannelMapDictGet(list->receiveChannelMapDict, rcmLabelNewItem);
	int numChannelsNewItem = iufReceiveChannelMapGetNumChannels(channelMapNewItem);
	if (numChannels1stItem != numChannelsNewItem)
		return IUF_FALSE;

	return IUF_TRUE;
}

int iufPatternListSet
(
	iupal_t list,
	iupa_t member,
	int index
)
{
	if (index < 0) return IUF_ERR_VALUE;
	if (list == NULL || index >= list->numPatterns)
	{
		IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_VALUE, IUF_ERR_MIN_ARG_VALUE , "index %d out of range", index);
		return IUF_ERR_VALUE;
	}
	if (index > 0)
	{
		IUF_BOOL validDimensions = iufPatternListValidateDimensions(list, member);
		if (validDimensions == IUF_FALSE)
		{
			IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_VALUE, IUF_ERR_MIN_ARG_VALUE, "inconsistent dimensions for member %d", index);
			return IUF_ERR_VALUE;
		}
	}
	list->pPatterns[index] = member;
	return IUF_E_OK;
}


iupal_t iufPatternListLoad
(
	hid_t handle
)
{
	char path[IUF_MAX_HDF5_PATH];
	int numPatterns, i;

	int status = iufHdf5ReadInt(handle, IUF_INPUTFILE_PATH_PATTERNLIST_SIZE, &(numPatterns));
	if (status != 0) return IUPAL_INVALID;

	iupal_t patternList = iufPatternListCreate(numPatterns, NULL, NULL);
	iupa_t pattern;

	// Load patterns
	for (i = 0; i < numPatterns; i++)
	{
        snprintf(path, sizeof(path), IUF_INPUTFILE_PATH_PATTERNLIST_PATTERN, i);
		hid_t patternId = H5Gopen(handle, path, H5P_DEFAULT);
		pattern = iufPatternLoad(patternId);
		if (pattern == IUPA_INVALID)
		{
			break;
		}
		iufPatternListSet(patternList, pattern, i);
	}
	patternList->loadedFromFile = IUF_TRUE;
	return patternList;
}

IUF_BOOL iufPatternListFull
(
	iupal_t list
)
{
	IUF_BOOL isFull = IUF_TRUE;
	int i;
	for (i = 0; i < list->numPatterns; i++)
	{
		if (list->pPatterns[i] == IUPA_INVALID)
		{
			isFull = IUF_FALSE;
			break;
		}
	}
	return isFull;
}

int iufPatternListSave
(
	iupal_t list,
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
	if (iufPatternListFull(list) == IUF_FALSE)
		return IUF_ERR_VALUE;

	iupa_t pattern;
	size = iufPatternListGetSize(list);
	status |= iufHdf5WriteInt(handle, IUF_INPUTFILE_PATH_PATTERNLIST_SIZE, &(size), 1);

	// iterate over source list elements and save'em
	for (i = 0; i < size; i++)
	{
		pattern = iufPatternListGet(list, i);
		if (pattern == IUPA_INVALID) continue;
        snprintf(path, sizeof(path), IUF_INPUTFILE_PATH_PATTERNLIST_PATTERN, i);
		hid_t pattern_id = H5Gcreate(handle, path, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
		status = iufPatternSave(pattern, pattern_id);
		H5Gclose(pattern_id);
		if (status != IUF_E_OK) break;
	}

	//status |= H5Gclose(patternList_id);
	return status;
}

