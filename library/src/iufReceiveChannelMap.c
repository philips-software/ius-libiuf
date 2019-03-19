// Created by Frank van Heesch on 18/07/2018.
//
#include <stdlib.h>
#include <string.h>

#include <iuf.h>
#include <iufReceiveChannelMapADT.h>
#include <iufReceiveChannelMapPrivate.h>
#include <math.h>

iurcm_t iufReceiveChannelMapCreate
(
	int numChannels
)
{
	int idx;
	iurcm_t receiveChannelMap;

	if (numChannels <= 0)
	{
		IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_VALUE, IUF_ERR_MIN_ARG_VALUE,
						   "numChannels argument should be > 0, but was: '%d'", numChannels);
		return IURCM_INVALID;
	}

	receiveChannelMap = calloc(1, sizeof(struct IufReceiveChannelMap));
    IUF_ERR_ALLOC_NULL_N_RETURN(receiveChannelMap, IufReceiveChannelMap, IURCM_INVALID);
	receiveChannelMap->startDelay = (float *) calloc(numChannels, sizeof(float));
	receiveChannelMap->numChannels = numChannels;
	receiveChannelMap->map = calloc(numChannels, sizeof(int));
	for (idx = 0; idx < numChannels; idx++)
	{
		receiveChannelMap->map[idx] = -1;
	}
	return receiveChannelMap;
}

int iufReceiveChannelMapDelete
(
	iurcm_t channelMap
)
{
	IUF_ERR_CHECK_NULL_N_RETURN(channelMap, IUF_ERR_VALUE);
	if (channelMap->map != NULL)
	{
		free(channelMap->map);
	}

	if (channelMap->startDelay != NULL)
	{
		free(channelMap->startDelay);
	}

	free(channelMap);
	return IUF_E_OK;
}

IUF_BOOL iufReceiveChannelMapCompare
(
	iurcm_t reference,
	iurcm_t actual
)
{
	int idx;
	if (reference == actual)
		return IUF_TRUE;
	if (reference == NULL || actual == NULL)
		return IUF_FALSE;
	if (reference->numChannels != actual->numChannels)
		return IUF_FALSE;
	
	for (idx = 0; idx < reference->numChannels; idx++)
	{
		if (reference->map[idx] != actual->map[idx])
			return IUF_FALSE;
        if (IUF_EQUAL_FLOAT(reference->startDelay[idx], actual->startDelay[idx]) == IUF_FALSE )
            return IUF_FALSE;
	}

	return IUF_TRUE;
}


int iufReceiveChannelMapGetNumChannels
(
	iurcm_t channelMap
)
{
	IUF_ERR_CHECK_NULL_N_RETURN(channelMap, -1);
	return channelMap->numChannels;
}

int iufReceiveChannelMapGetChannel
(
	iurcm_t channelMap,
	int index
)
{
	IUF_ERR_CHECK_NULL_N_RETURN(channelMap, -1);
	if (index >= channelMap->numChannels || index < 0)
	{
		IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_VALUE, IUF_ERR_MIN_ARG_VALUE,
						   "index >= 0 && index < %d (numChannels) but was '%d'", channelMap->numChannels, index);
		return -1;
	}

	return channelMap->map[index];
}

int iufReceiveChannelMapGetNumDelays
(
    iurcm_t channelMap
)
{
	IUF_ERR_CHECK_NULL_N_RETURN(channelMap, -1);
    return channelMap->numChannels;
}

float iufReceiveChannelMapGetStartDelay
(
    iurcm_t channelMap,
    int index
)
{
	IUF_ERR_CHECK_NULL_N_RETURN(channelMap, NAN);
	if (index >= channelMap->numChannels || index < 0)
	{
		IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_VALUE, IUF_ERR_MIN_ARG_VALUE,
						   "index >= 0 && index < %d (numChannels) but was '%d'", channelMap->numChannels, index);
		return NAN;
	}
    return channelMap->startDelay[index];
}


// Setters
int iufReceiveChannelMapSetStartDelay
(
    iurcm_t channelMap,
    int index,
    float delay
)
{
	IUF_ERR_CHECK_NULL_N_RETURN(channelMap, IUF_ERR_VALUE);
	if (index >= channelMap->numChannels || index < 0)
	{
		IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_VALUE, IUF_ERR_MIN_ARG_VALUE,
						   "index >= 0 && index < %d (numChannels) but was '%d'", channelMap->numChannels, index);
		return IUF_ERR_VALUE;
	}
	channelMap->startDelay[index] = delay;
    return IUF_E_OK;
}


int iufReceiveChannelMapSetChannel
(
	iurcm_t channelMap,
	int index,
	int transducerIdx
)
{
	IUF_ERR_CHECK_NULL_N_RETURN(channelMap, IUF_ERR_VALUE);
	if (index >= channelMap->numChannels || index < 0)
	{
		IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_VALUE, IUF_ERR_MIN_ARG_VALUE,
						   "index >= 0 && index < %d (numChannels) but was '%d'", channelMap->numChannels, index);
		return IUF_ERR_VALUE;
	}

	if (transducerIdx < 0)
	{
		IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_VALUE, IUF_ERR_MIN_ARG_VALUE,
						   "transducerIdx should be >= 0,  but was '%d'", transducerIdx);
		return IUF_ERR_VALUE;
	}

	channelMap->map[index] = transducerIdx;
	return IUF_E_OK;
}

int iufReceiveChannelMapSetMap
(
	iurcm_t channelMap,
	int *map
)
{
	IUF_ERR_CHECK_NULL_N_RETURN(channelMap, IUF_ERR_VALUE);
	IUF_ERR_CHECK_NULL_N_RETURN(map, IUF_ERR_VALUE);
    if (channelMap->numChannels <= 0)
    {
        IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_VALUE, IUF_ERR_MIN_ARG_VALUE,
                           "numChannels should me  > 0 but was '%d'", channelMap->numChannels);
        return IUF_ERR_VALUE;
    }

	memcpy(channelMap->map, map, channelMap->numChannels * sizeof(int));
    if (channelMap->map == NULL)
    {
        IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_MEMORY, IUF_ERR_MIN_MEMCOPY,
                           "numChannels should me  > 0 but was '%d'", channelMap->numChannels);
        return IUF_ERR_VALUE;
    }

	return IUF_E_OK;
}

int iufReceiveChannelMapSave
(
	iurcm_t channelMap,
	hid_t   group_id
)
{
	/* write the /Receivechannelmap data */
	herr_t  status;
	IUF_ERR_CHECK_NULL_N_RETURN(channelMap, IUF_ERR_VALUE);
	IUF_ERR_EVAL_N_RETURN(group_id == H5I_INVALID_HID, IUF_ERR_VALUE);
	status = iufHdf5WriteInt(group_id, IUF_INPUTFILE_PATH_RECEIVECHANNELMAP_NUMCHANNELS, &(channelMap->numChannels), 1);
	status |= iufHdf5WriteInt(group_id, IUF_INPUTFILE_PATH_RECEIVECHANNELMAP_MAP, channelMap->map, channelMap->numChannels);
    status |= iufHdf5WriteFloat(group_id, IUF_INPUTFILE_PATH_RECEIVESETTINGS_STATRDELAY, channelMap->startDelay, channelMap->numChannels);

	if (status != 0)
	{
		IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_HDF5, IUF_ERR_MIN_HDF5, "write failed for %s, %s and/or %s",
						   IUF_INPUTFILE_PATH_RECEIVECHANNELMAP_NUMCHANNELS,
						   IUF_INPUTFILE_PATH_RECEIVECHANNELMAP_MAP,
						   IUF_INPUTFILE_PATH_RECEIVESETTINGS_STATRDELAY);
		return IUF_ERR_VALUE;
	}
	return status;
}

iurcm_t iufReceiveChannelMapLoad
(
	hid_t group_id
)
{
	herr_t status;
	iurcm_t receiveChannelMap;
	int idx = 0;
	int numChannels=0;
	int *channelMap;

	IUF_ERR_EVAL_N_RETURN(group_id == H5I_INVALID_HID, IURCM_INVALID);
	status = iufHdf5ReadInt(group_id, IUF_INPUTFILE_PATH_RECEIVECHANNELMAP_NUMCHANNELS, &numChannels);
	if (numChannels <= 0)
	{
		IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_HDF5, IUF_ERR_MIN_ARG_VALUE,
						   "numChannels should me > 0 but was '%d'", numChannels);
		return IURCM_INVALID;
	}

	channelMap = (int *)calloc(numChannels, sizeof(int));
	if (channelMap == NULL)
	{
		IUF_ERROR_PUSH(IUF_ERR_MAJ_MEMORY, IUF_ERR_MIN_ALLOC, "calloc failed for numchannels");
		return IURCM_INVALID;
	}

	status |= iufHdf5ReadInt(group_id, IUF_INPUTFILE_PATH_RECEIVECHANNELMAP_MAP, channelMap);
	if (status != 0 )
	{
		IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_HDF5, IUF_ERR_MIN_HDF5, "read failed for %s and/or %s",
						   IUF_INPUTFILE_PATH_RECEIVECHANNELMAP_NUMCHANNELS,
						   IUF_INPUTFILE_PATH_RECEIVECHANNELMAP_MAP);
		free(channelMap);
		return IURCM_INVALID;
	}


	receiveChannelMap = iufReceiveChannelMapCreate(numChannels);
	for (idx = 0; idx < numChannels; idx++)
	{
		status = iufReceiveChannelMapSetChannel(receiveChannelMap, idx, channelMap[idx]);
		if (status != 0)
			break;
	}

	free(channelMap);

    // Memory allocation for startDelays has been done in iufReceiveChannelMapCreate
    // now, the delays can be read into iufReceiveChannelMapCreate->startDelay.
    status |= iufHdf5ReadFloat(group_id, IUF_INPUTFILE_PATH_RECEIVESETTINGS_STATRDELAY, receiveChannelMap->startDelay);
    if (status != 0)
    {
		IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_HDF5, IUF_ERR_MIN_HDF5, "read failed for %s",
						   IUF_INPUTFILE_PATH_RECEIVESETTINGS_STATRDELAY);
		iufReceiveChannelMapDelete(receiveChannelMap);
        receiveChannelMap = IURCM_INVALID;
    }


    return receiveChannelMap;
}

