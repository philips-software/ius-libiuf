//
// Created by Frank van Heesch on 18/07/2018.
//

#ifndef IUSLIBRARY_IUSHLRECEIVECHANNELMAPIMP_H
#define IUSLIBRARY_IUSHLRECEIVECHANNELMAPIMP_H
#include <hdf5.h>
#include <iusReceiveChannelMap.h>

struct IusReceiveChannelMap
{
	int numChannels;
	int *map;
	float   *startDelay;            /**< The start delay of RFlines, array length is the number of pulses per frame, values are in seconds */
};

int iusReceiveChannelMapSave
(
	iurcm_t channelMap,
	hid_t   handle
);

iurcm_t iusReceiveChannelMapLoad
(
	hid_t handle
);


#endif
