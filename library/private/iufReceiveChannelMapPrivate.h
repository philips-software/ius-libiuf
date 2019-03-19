//
// Created by Frank van Heesch on 18/07/2018.
//

#ifndef IUFLIBRARY_IUFHLRECEIVECHANNELMAPIMP_H
#define IUFLIBRARY_IUFHLRECEIVECHANNELMAPIMP_H
#include <hdf5.h>
#include <iufReceiveChannelMap.h>


int iufReceiveChannelMapSave
(
	iurcm_t channelMap,
	hid_t   handle
);

iurcm_t iufReceiveChannelMapLoad
(
	hid_t handle
);


#endif
