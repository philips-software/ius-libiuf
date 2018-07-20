//
// Created by Frank van Heesch on 18/07/2018.
//

#ifndef IUSLIBRARY_IUSHLRECEIVECHANNELMAPIMP_H
#define IUSLIBRARY_IUSHLRECEIVECHANNELMAPIMP_H
#include <hdf5.h>
#include <iusHLReceiveChannelMap.h>

struct IusReceiveChannelMap
{
	int numChannels;
	int *map;
};

int iusHLReceiveChannelMapSave
(
	iurcm_t receiveChannelMap,
	hid_t   handle
);

iurcm_t iusHLReceiveChannelMapLoad
(
	hid_t handle
);


#endif
