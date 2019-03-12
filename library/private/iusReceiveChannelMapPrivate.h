//
// Created by Frank van Heesch on 18/07/2018.
//

#ifndef IUSLIBRARY_IUSHLRECEIVECHANNELMAPIMP_H
#define IUSLIBRARY_IUSHLRECEIVECHANNELMAPIMP_H
#include <hdf5.h>
#include <iusReceiveChannelMap.h>


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
