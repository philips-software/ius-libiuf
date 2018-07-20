//
// Created by Ruijzendaal on 30/03/2018.
//
#ifndef IUSLIBRARY_IUSHLRECEIVECHANNELMAP_H
#define IUSLIBRARY_IUSHLRECEIVECHANNELMAP_H

#include <iusTypes.h>

// ADT
struct IusReceiveChannelMap;
typedef  struct IusReceiveChannelMap IusReceiveChannelMap;
typedef  IusReceiveChannelMap    * iurcm_t;
#define  IURCM_INVALID (iurcm_t) NULL

iurcm_t  iusHLReceiveChannelMapCreate
(
	int numChannels
);

int iusHLReceiveChannelMapDelete
(
	iurcm_t receiveChannelMap
);

IUS_BOOL  iusHLReceiveChannelMapCompare
(
	iurcm_t reference,
	iurcm_t actual
);

int iusHLReceiveChannelMapGetChannel
(
	iurcm_t receiveChannelMap,
	int mapIdx
);

int iusHLReceiveChannelMapSetChannel
(
	iurcm_t receiveChannelMap,
	int mapIdx,
	int transducerIdx
);

int iusHLReceiveChannelMapSetMap
(
	iurcm_t receiveChannelMap,
	int *map
);

#endif //IUSLIBRARY_IUSHLRECEIVECHANNELMAP_H

