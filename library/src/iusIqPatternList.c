
//
// Created by nlv09165 on 18/07/2018.
//
#include <stdlib.h>

#include <include/ius.h>
#include <iusIqPatternPrivate.h>
#include <iusIqPatternListPrivate.h>

// ADT
struct IusIqPatternList
{
	int numPatterns;
	iuiqpa_t *pPatterns;
	IUS_BOOL deepDelete;
	iudmd_t demodulationDict;
	iurcmd_t receiveChannelMapDict;

};

// ADT
iuiqpal_t iusIqPatternListCreate
(
	int numPatterns,
	iudmd_t demodulationDict,
	iurcmd_t receiveChannelMapDict
)
{
    if (numPatterns <= 0)
    {
        IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_VALUE, IUS_ERR_MIN_ARG_VALUE,
                           "numPatterns argument should be > 0, but was: '%d'", numPatterns);
        return IUIQPAL_INVALID;
    }

	iuiqpal_t list = calloc(1, sizeof(IusIqPatternList));
    IUS_ERR_ALLOC_NULL_N_RETURN(list, IusPatternList, IUIQPAL_INVALID);

    list->deepDelete = IUS_FALSE;
    list->receiveChannelMapDict = receiveChannelMapDict;
    list->demodulationDict = demodulationDict;
    list->numPatterns = numPatterns;
    list->pPatterns = (iuiqpa_t *)calloc((size_t)numPatterns, sizeof(iuiqpa_t));
    if (list->pPatterns == NULL)
    {
        IUS_ERROR_PUSH(IUS_ERR_MAJ_MEMORY, IUS_ERR_MIN_ALLOC, "calloc failed for numPatterns");
        free(list);
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
			iusIqPatternDelete(list->pPatterns[index]);
		}
	}
	free(list->pPatterns);
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
	if (reference == actual) return IUS_TRUE;
	if (reference == NULL || actual == NULL) return IUS_FALSE;
	if (reference->numPatterns != actual->numPatterns) return IUS_FALSE;
	for (index = 0; index < actual->numPatterns; index++)
	{
		if (iusIqPatternCompare(reference->pPatterns[index], actual->pPatterns[index])
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
    IUS_ERR_CHECK_NULL_N_RETURN(list, IUS_ERR_VALUE);
	return list->numPatterns;
}

iuiqpa_t iusIqPatternListGet
(
	iuiqpal_t list,
	int index
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(list, IUIQPA_INVALID);
    if (index >= list->numPatterns || index < 0)
    {
        IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_VALUE, IUS_ERR_MIN_ARG_VALUE,
                           "index >= 0 && index < %d (numPatterns) but was '%d'", list->numPatterns, index);
        return IUIQPA_INVALID;
    }
	return list->pPatterns[index];
}

IUS_BOOL iusIqPatternListValidateDimensions
(
	iuiqpal_t list,
	iuiqpa_t member
)
{
	// getSamplesPerLine of first item
	if (list == NULL) return IUS_FALSE;
	char *dmLabel1stItem = (char *)iusIqPatternGetDemodulationLabel(list->pPatterns[0]);
	iudm_t demodulation1stItem = iusDemodulationDictGet(list->demodulationDict, dmLabel1stItem);
	int numSamplesPerLine1stItem = iusDemodulationGetNumSamplesPerLine(demodulation1stItem);

	char *dmLabelNewItem = (char *)iusIqPatternGetDemodulationLabel(member);
	iudm_t demodulationNewItem = iusDemodulationDictGet(list->demodulationDict, dmLabelNewItem);
	int numSamplesPerLineNewItem = iusDemodulationGetNumSamplesPerLine(demodulationNewItem);
    if (numSamplesPerLine1stItem != numSamplesPerLineNewItem)
    {
        IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_VALUE, IUS_ERR_MIN_ARG_VALUE,
                           "Invalid dimensions for member NumSamplesPerLine (%d) != %d",
                           numSamplesPerLineNewItem, numSamplesPerLine1stItem );
        return IUS_FALSE;
    }


	char *rcmLabel1stItem = (char *)iusIqPatternGetChannelMapLabel(list->pPatterns[0]);
	iurcm_t channelMap1stItem = iusReceiveChannelMapDictGet(list->receiveChannelMapDict, rcmLabel1stItem);
	int numChannels1stItem = iusReceiveChannelMapGetNumChannels(channelMap1stItem);

	char *rcmLabelNewItem = (char *)iusIqPatternGetChannelMapLabel(member);
	iurcm_t channelMapNewItem = iusReceiveChannelMapDictGet(list->receiveChannelMapDict, rcmLabelNewItem);
	int numChannelsNewItem = iusReceiveChannelMapGetNumChannels(channelMapNewItem);
    if (numChannels1stItem != numChannelsNewItem)
    {
        IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_VALUE, IUS_ERR_MIN_ARG_VALUE,
                           "Invalid dimensions for member NumChannels (%d) != %d",
                           numChannelsNewItem, numChannels1stItem );
        return IUS_FALSE;
    }

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
    if (index >= list->numPatterns || index < 0)
    {
        IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_VALUE, IUS_ERR_MIN_ARG_VALUE,
                           "index >= 0 && index < %d (numPatterns) but was '%d'", list->numPatterns, index);
        return IUS_ERR_VALUE;
    }

	if (index > 0)
	{
		IUS_BOOL validDimensions = iusIqPatternListValidateDimensions(list, member);
		if (validDimensions == IUS_FALSE)
		{
			return IUS_ERR_VALUE;
		}
	}
	list->pPatterns[index] = member;
	return IUS_E_OK;
}


iuiqpal_t iusIqPatternListLoad
(
	hid_t handle
)
{
	char path[IUS_MAX_HDF5_PATH];
	int numPatterns, i;

    IUS_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUIQPAL_INVALID);

    int status = iusHdf5ReadInt(handle, IUS_IQFILE_PATH_PATTERNLIST_SIZE, &(numPatterns));
	if (status != 0) return IUIQPAL_INVALID;

	iuiqpal_t patternList = iusIqPatternListCreate(numPatterns, NULL, NULL);
	iuiqpa_t pattern;

	// Load patterns
	for (i = 0; i < numPatterns; i++)
	{
		sprintf(path, IUS_IQFILE_PATH_PATTERNLIST_PATTERN, i);
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

    IUS_ERR_CHECK_NULL_N_RETURN(list, IUS_FALSE);

    for (i = 0; i < list->numPatterns; i++)
	{
		if (list->pPatterns[i] == IUIQPA_INVALID)
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
    IUS_ERR_CHECK_NULL_N_RETURN(list, IUS_ERR_VALUE);
    IUS_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUS_ERR_VALUE);

	if (iusIqPatternListFull(list) == IUS_FALSE)
    {
        IUS_ERROR_PUSH(IUS_ERR_MAJ_VALUE, IUS_ERR_MIN_ARG_VALUE, "argument list (pattern list) was not full");
        return IUS_ERR_VALUE;
    }

	iuiqpa_t pattern;
	size = iusIqPatternListGetSize(list);
	status |= iusHdf5WriteInt(handle, IUS_INPUTFILE_PATH_PATTERNLIST_SIZE, &(size), 1);
    if (status != 0)
    {
        IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_HDF5, IUS_ERR_MIN_HDF5, "write failed for %s",
                           IUS_INPUTFILE_PATH_PATTERNLIST_SIZE);
        return IUS_ERR_VALUE;
    }

	// iterate over source list elements and save'em
	for (i = 0; i < size; i++)
	{
		pattern = iusIqPatternListGet(list, i);
		if (pattern == IUIQPA_INVALID) continue;
		sprintf(path, IUS_IQFILE_PATH_PATTERNLIST_PATTERN, i);
		hid_t pattern_id = H5Gcreate(handle, path, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
		status = iusIqPatternSave(pattern, pattern_id);
		H5Gclose(pattern_id);
		if (status != IUS_E_OK) break;
	}

	//status |= H5Gclose(patternList_id);
	return status;
}


