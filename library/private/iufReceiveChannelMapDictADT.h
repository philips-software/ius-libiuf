#ifndef IUFLIBRARY_IUFRECEIVECHANNELMAPDICTADT_H
#define IUFLIBRARY_IUFRECEIVECHANNELMAPDICTADT_H

// ADT
#include <hashmap.h>
#include <iufReceiveChannelMap.h>

struct HashableReceiveChannelMap
{
	iurcm_t receiveChannelMap;
	char key[256];
};

typedef struct HashableReceiveChannelMap HashableReceiveChannelMap;

struct IufReceiveChannelMapDict
{
	struct hashmap map;
	IUF_BOOL deepDelete;
    char **kys;
};

#endif
