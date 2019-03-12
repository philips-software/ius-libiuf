#ifndef IUSLIBRARY_IUSRECEIVECHANNELMAPDICTADT_H
#define IUSLIBRARY_IUSRECEIVECHANNELMAPDICTADT_H

// ADT
#include <hashmap.h>
#include <iusReceiveChannelMap.h>

struct HashableReceiveChannelMap
{
	iurcm_t receiveChannelMap;
	char key[256];
};

typedef struct HashableReceiveChannelMap HashableReceiveChannelMap;

struct IusReceiveChannelMapDict
{
	struct hashmap map;
	IUS_BOOL deepDelete;
    char **keys;
};

#endif