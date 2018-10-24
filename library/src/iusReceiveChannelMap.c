// Created by Frank van Heesch on 18/07/2018.
//
#include <stdlib.h>
#include <string.h>

#include <ius.h>
#include <iusReceiveChannelMapPrivate.h>


iurcm_t iusReceiveChannelMapCreate
(
	int numChannels
)
{
	int idx;
	iurcm_t receiveChannelMap = IURCM_INVALID;
	
	if (numChannels <= 0) return IURCM_INVALID;

	receiveChannelMap = calloc(1, sizeof(struct IusReceiveChannelMap));
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
	iurcm_t receiveChannelMap
)
{
	if (receiveChannelMap == NULL) return IUS_ERR_VALUE;
	if (receiveChannelMap->map != NULL)
	{
		free(receiveChannelMap->map);
	}
	free(receiveChannelMap);
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
	}
	return IUS_TRUE;
}


int iusReceiveChannelMapGetNumChannels
(
	iurcm_t receiveChannelMap
)
{
	if (receiveChannelMap == NULL) return  -1;
	return receiveChannelMap->numChannels;
}

int iusReceiveChannelMapGetChannel
(
	iurcm_t receiveChannelMap,
	int mapIdx
)
{
	if (receiveChannelMap == NULL) return  -1;
	if (mapIdx >= receiveChannelMap->numChannels || mapIdx < 0) return -1;
	return receiveChannelMap->map[mapIdx];
}

int iusReceiveChannelMapSetChannel
(
	iurcm_t receiveChannelMap,
	int mapIdx,
	int transducerIdx
)
{
	int status = 0;
	if (receiveChannelMap == NULL) return  -1;
	if (mapIdx >= receiveChannelMap->numChannels || mapIdx < 0) return -1;
	if (transducerIdx < 0) return -1;

	receiveChannelMap->map[mapIdx] = transducerIdx;

	return status;
}

int iusReceiveChannelMapSetMap
(
	iurcm_t receiveChannelMap,
	int *map
)
{
	if (receiveChannelMap == NULL || map == NULL) return  -1;
	if (receiveChannelMap->numChannels <= 0) return -1;

	memcpy(receiveChannelMap->map, map, receiveChannelMap->numChannels * sizeof(int));

	return (receiveChannelMap->map == NULL);
}

int iusReceiveChannelMapSave
(
	iurcm_t receiveChannelMap,
	hid_t   group_id
)
{
	/* write the /Receivechannelmap data */
	herr_t  status;

	status = iusHdf5WriteInt(group_id, IUS_INPUTFILE_PATH_RECEIVECHANNELMAP_NUMCHANNELS, &(receiveChannelMap->numChannels), 1);
	status |= iusHdf5WriteInt(group_id, IUS_INPUTFILE_PATH_RECEIVECHANNELMAP_MAP, receiveChannelMap->map, receiveChannelMap->numChannels);

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

	status = iusHdf5ReadInt(group_id, IUS_INPUTFILE_PATH_RECEIVECHANNELMAP_NUMCHANNELS, &numChannels);
	if (status != 0 || numChannels <= 0) return NULL;

	channelMap = (int *)calloc(numChannels, sizeof(int));
	status |= iusHdf5ReadInt(group_id, IUS_INPUTFILE_PATH_RECEIVECHANNELMAP_MAP, channelMap);
	if (status != 0) return NULL;
	
	receiveChannelMap = iusReceiveChannelMapCreate(numChannels);
	for (idx = 0; idx < numChannels; idx++)
	{
		status |= iusReceiveChannelMapSetChannel(receiveChannelMap, idx, channelMap[idx]);
	}
	if (status != 0) return NULL;

	if (channelMap != NULL)
	{
		free(channelMap);
	}
	
	return receiveChannelMap;
}

