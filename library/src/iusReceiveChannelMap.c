// Created by Frank van Heesch on 18/07/2018.
//
#include <stdlib.h>
#include <string.h>

#include <ius.h>
#include <iusReceiveChannelMapPrivate.h>
#include <math.h>

iurcm_t iusReceiveChannelMapCreate
(
	int numChannels
)
{
	int idx;
	iurcm_t receiveChannelMap = IURCM_INVALID;

	if (numChannels <= 0)
	{
		IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_VALUE, IUS_ERR_MIN_ARG_VALUE,
						   "numChannels argument should be > 0, but was: '%d'", numChannels);
		return IURCM_INVALID;
	}

	receiveChannelMap = calloc(1, sizeof(struct IusReceiveChannelMap));
	if (receiveChannelMap == NULL)
	{
		IUS_ERROR_PUSH(IUS_ERR_MAJ_MEMORY, IUS_ERR_MIN_ALLOC, "calloc failed for IusReceiveChannelMap");
		return IURCM_INVALID;
	}

	receiveChannelMap->startDelay = (float *) calloc(numChannels, sizeof(float));
	receiveChannelMap->numChannels = numChannels;
	receiveChannelMap->map = calloc(numChannels, sizeof(int));
	for (idx = 0; idx < numChannels; idx++)
	{
		receiveChannelMap->map[idx] = -1;
	}
	return receiveChannelMap;
}

int iusReceiveChannelMapDelete
(
	iurcm_t channelMap
)
{
	if (channelMap == NULL)
	{
		IUS_ERROR_PUSH(IUS_ERR_MAJ_VALUE, IUS_ERR_MIN_ARG_NULL_VALUE, "argument channelMap was NULL");
		return IUS_ERR_VALUE;
	}

	if (channelMap->map != NULL)
	{
		free(channelMap->map);
	}

	if (channelMap->startDelay != NULL)
	{
		free(channelMap->startDelay);
	}

	free(channelMap);
	return IUS_E_OK;
}

IUS_BOOL iusReceiveChannelMapCompare
(
	iurcm_t reference,
	iurcm_t actual
)
{
	int idx;
	if (reference == actual)
		return IUS_TRUE;
	if (reference == NULL || actual == NULL)
		return IUS_FALSE;
	if (reference->numChannels != actual->numChannels)
		return IUS_FALSE;
	
	for (idx = 0; idx < reference->numChannels; idx++)
	{
		if (reference->map[idx] != actual->map[idx])
			return IUS_FALSE;
        if (IUS_EQUAL_FLOAT(reference->startDelay[idx], actual->startDelay[idx]) == IUS_FALSE )
            return IUS_FALSE;
	}

	return IUS_TRUE;
}


int iusReceiveChannelMapGetNumChannels
(
	iurcm_t channelMap
)
{
	if (channelMap == NULL)
	{
		IUS_ERROR_PUSH(IUS_ERR_MAJ_VALUE, IUS_ERR_MIN_ARG_NULL_VALUE, "argument channelMap was NULL");
		return -1;
	}
	return channelMap->numChannels;
}

int iusReceiveChannelMapGetChannel
(
	iurcm_t channelMap,
	int index
)
{
	if (channelMap == NULL)
	{
		IUS_ERROR_PUSH(IUS_ERR_MAJ_VALUE, IUS_ERR_MIN_ARG_NULL_VALUE, "argument channelMap was NULL");
		return -1;
	}

	if (index >= channelMap->numChannels || index < 0)
	{
		IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_VALUE, IUS_ERR_MIN_ARG_VALUE,
						   "index >= 0 && index < %d (numChannels) but was '%d'", channelMap->numChannels, index);
		return -1;
	}

	return channelMap->map[index];
}

int iusReceiveChannelMapGetNumDelays
(
    iurcm_t channelMap
)
{
    if (channelMap == NULL)
    {
        IUS_ERROR_PUSH(IUS_ERR_MAJ_VALUE, IUS_ERR_MIN_ARG_NULL_VALUE, "argument channelMap was NULL");
        return -1;
    }
    return channelMap->numChannels;
}

float iusReceiveChannelMapGetStartDelay
(
    iurcm_t channelMap,
    int index
)
{
    if (channelMap == NULL)
    {
        IUS_ERROR_PUSH(IUS_ERR_MAJ_VALUE, IUS_ERR_MIN_ARG_NULL_VALUE, "argument channelMap was NULL");
        return NAN;
    }

	if (index >= channelMap->numChannels || index < 0)
	{
		IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_VALUE, IUS_ERR_MIN_ARG_VALUE,
						   "index >= 0 && index < %d (numChannels) but was '%d'", channelMap->numChannels, index);
		return NAN;
	}
    return channelMap->startDelay[index];
}


// Setters
int iusReceiveChannelMapSetStartDelay
(
    iurcm_t channelMap,
    int index,
    float delay
)
{
	if (channelMap == NULL)
	{
		IUS_ERROR_PUSH(IUS_ERR_MAJ_VALUE, IUS_ERR_MIN_ARG_NULL_VALUE, "argument channelMap was NULL");
		return NAN;
	}

	if (index >= channelMap->numChannels || index < 0)
	{
		IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_VALUE, IUS_ERR_MIN_ARG_VALUE,
						   "index >= 0 && index < %d (numChannels) but was '%d'", channelMap->numChannels, index);
		return NAN;
	}
	channelMap->startDelay[index] = delay;
    return IUS_E_OK;
}


int iusReceiveChannelMapSetChannel
(
	iurcm_t channelMap,
	int index,
	int transducerIdx
)
{
	if (channelMap == NULL)
	{
		IUS_ERROR_PUSH(IUS_ERR_MAJ_VALUE, IUS_ERR_MIN_ARG_NULL_VALUE, "argument channelMap was NULL");
		return IUS_ERR_VALUE;
	}

	if (index >= channelMap->numChannels || index < 0)
	{
		IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_VALUE, IUS_ERR_MIN_ARG_VALUE,
						   "index >= 0 && index < %d (numChannels) but was '%d'", channelMap->numChannels, index);
		return IUS_ERR_VALUE;
	}

	if (transducerIdx < 0)
	{
		IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_VALUE, IUS_ERR_MIN_ARG_VALUE,
						   "transducerIdx should be >= 0,  but was '%d'", transducerIdx);
		return IUS_ERR_VALUE;
	}

	channelMap->map[index] = transducerIdx;
	return IUS_E_OK;
}

int iusReceiveChannelMapSetMap
(
	iurcm_t channelMap,
	int *map
)
{
	if (channelMap == NULL)
	{
		IUS_ERROR_PUSH(IUS_ERR_MAJ_VALUE, IUS_ERR_MIN_ARG_NULL_VALUE, "argument channelMap was NULL");
		return IUS_ERR_VALUE;
	}

	if (map == NULL)
	{
		IUS_ERROR_PUSH(IUS_ERR_MAJ_VALUE, IUS_ERR_MIN_ARG_NULL_VALUE, "argument map was NULL");
		return IUS_ERR_VALUE;
	}

    if (channelMap->numChannels <= 0)
    {
        IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_VALUE, IUS_ERR_MIN_ARG_VALUE,
                           "numChannels should me  > 0 but was '%d'", channelMap->numChannels);
        return IUS_ERR_VALUE;
    }

	memcpy(channelMap->map, map, channelMap->numChannels * sizeof(int));
    if (channelMap->map == NULL)
    {
        IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_MEMORY, IUS_ERR_MIN_MEMCOPY,
                           "numChannels should me  > 0 but was '%d'", channelMap->numChannels);
        return IUS_ERR_VALUE;
    }

	return IUS_E_OK;
}

int iusReceiveChannelMapSave
(
	iurcm_t channelMap,
	hid_t   group_id
)
{
	/* write the /Receivechannelmap data */
	herr_t  status;
	if (channelMap == NULL)
	{
		IUS_ERROR_PUSH(IUS_ERR_MAJ_VALUE, IUS_ERR_MIN_ARG_NULL_VALUE, "argument channelMap was NULL");
		return IUS_ERR_VALUE;
	}

	if (group_id == H5I_INVALID_HID)
	{
		IUS_ERROR_PUSH(IUS_ERR_MAJ_VALUE, IUS_ERR_MIN_ARG_VALUE, "argument group_id was invalid");
		return IUS_ERR_VALUE;
	}

	status = iusHdf5WriteInt(group_id, IUS_INPUTFILE_PATH_RECEIVECHANNELMAP_NUMCHANNELS, &(channelMap->numChannels), 1);
	status |= iusHdf5WriteInt(group_id, IUS_INPUTFILE_PATH_RECEIVECHANNELMAP_MAP, channelMap->map, channelMap->numChannels);
    status |= iusHdf5WriteFloat(group_id, IUS_INPUTFILE_PATH_RECEIVESETTINGS_STATRDELAY, channelMap->startDelay, channelMap->numChannels);

	if (status != 0)
	{
		IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_HDF5, IUS_ERR_MIN_HDF5, "write failed for %s, %s and/or %s",
						   IUS_INPUTFILE_PATH_RECEIVECHANNELMAP_NUMCHANNELS,
						   IUS_INPUTFILE_PATH_RECEIVECHANNELMAP_MAP,
						   IUS_INPUTFILE_PATH_RECEIVESETTINGS_STATRDELAY);
		return IUS_ERR_VALUE;
	}
	return status;
}

iurcm_t iusReceiveChannelMapLoad
(
	hid_t group_id
)
{
	herr_t status;
	iurcm_t receiveChannelMap;
	int idx = 0;
	int numChannels=0;
	int *channelMap;

	if (group_id == H5I_INVALID_HID)
	{
		IUS_ERROR_PUSH(IUS_ERR_MAJ_VALUE, IUS_ERR_MIN_ARG_VALUE, "argument group_id was invalid");
		return IURCM_INVALID;
	}

	status = iusHdf5ReadInt(group_id, IUS_INPUTFILE_PATH_RECEIVECHANNELMAP_NUMCHANNELS, &numChannels);
	if (numChannels <= 0)
	{
		IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_HDF5, IUS_ERR_MIN_ARG_VALUE,
						   "numChannels should me > 0 but was '%d'", numChannels);
		return IURCM_INVALID;
	}

	channelMap = (int *)calloc(numChannels, sizeof(int));
	if (channelMap == NULL)
	{
		IUS_ERROR_PUSH(IUS_ERR_MAJ_MEMORY, IUS_ERR_MIN_ALLOC, "calloc failed for numchannels");
		return IURCM_INVALID;
	}

	status |= iusHdf5ReadInt(group_id, IUS_INPUTFILE_PATH_RECEIVECHANNELMAP_MAP, channelMap);
	if (status != 0 )
	{
		IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_HDF5, IUS_ERR_MIN_HDF5, "read failed for %s and/or %s",
						   IUS_INPUTFILE_PATH_RECEIVECHANNELMAP_NUMCHANNELS,
						   IUS_INPUTFILE_PATH_RECEIVECHANNELMAP_MAP);
		free(channelMap);
		return IURCM_INVALID;
	}


	receiveChannelMap = iusReceiveChannelMapCreate(numChannels);
	for (idx = 0; idx < numChannels; idx++)
	{
		status = iusReceiveChannelMapSetChannel(receiveChannelMap, idx, channelMap[idx]);
		if (status != 0)
			break;
	}

	free(channelMap);

    // Memory allocation for startDelays has been done in iusReceiveChannelMapCreate
    // now, the delays can be read into iusReceiveChannelMapCreate->startDelay.
    status |= iusHdf5ReadFloat(group_id, IUS_INPUTFILE_PATH_RECEIVESETTINGS_STATRDELAY, receiveChannelMap->startDelay);
    if (status != 0)
    {
		IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_HDF5, IUS_ERR_MIN_HDF5, "read failed for %s",
						   IUS_INPUTFILE_PATH_RECEIVESETTINGS_STATRDELAY);
		iusReceiveChannelMapDelete(receiveChannelMap);
        receiveChannelMap = IURCM_INVALID;
    }


    return receiveChannelMap;
}

