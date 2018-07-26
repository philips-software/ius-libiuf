// Created by nlv09165 on 03/05/2018.
#ifndef IUSLIBRARY_IUSHLRECEIVECHANNELMAPDICT_H
#define IUSLIBRARY_IUSHLRECEIVECHANNELMAPDICT_H

#include <iusHLReceiveChannelMap.h>
#include <iusHLReceiveChannelMapImp.h>

// ADT
typedef struct IusReceiveChannelMapDict IusReceiveChannelMapDict;
typedef IusReceiveChannelMapDict *iurcmd_t;
#define  IURCMD_INVALID (iurcmd_t) NULL

iurcmd_t iusHLReceiveChannelMapDictCreate
(
	void
);

int iusHLReceiveChannelMapDictDelete
(
	iurcmd_t dict
);

int iusHLReceiveChannelMapDictCompare
(
	iurcmd_t reference,
	iurcmd_t actual
);

size_t iusHLReceiveChannelMapDictGetSize
(
	iurcmd_t dict
);

iurcm_t iusHLReceiveChannelMapDictGet
(
	iurcmd_t list,
	char * key
);

int iusHLReceiveChannelMapDictSet
(
	iurcmd_t dict,
	char * key,
	iurcm_t member
);

#endif //IUSLIBRARY_IUSHLRECEIVECHANNELMAPDICT_H

