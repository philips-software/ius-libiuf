// Created by nlv09165 on 03/05/2018.
#ifndef IUSLIBRARY_IUSHLRECEIVECHANNELMAPDICT_H
#define IUSLIBRARY_IUSHLRECEIVECHANNELMAPDICT_H

#include <iusReceiveChannelMap.h>
#include <iusReceiveChannelMapImp.h>

// ADT
typedef struct IusReceiveChannelMapDict IusReceiveChannelMapDict;
typedef IusReceiveChannelMapDict *iurcmd_t;
#define  IURCMD_INVALID (iurcmd_t) NULL

iurcmd_t iusReceiveChannelMapDictCreate
(
	void
);

int iusReceiveChannelMapDictDelete
(
	iurcmd_t dict
);

int iusReceiveChannelMapDictCompare
(
	iurcmd_t reference,
	iurcmd_t actual
);

size_t iusReceiveChannelMapDictGetSize
(
	iurcmd_t dict
);

iurcm_t iusReceiveChannelMapDictGet
(
	iurcmd_t list,
	char * key
);

int iusReceiveChannelMapDictSet
(
	iurcmd_t dict,
	char * key,
	iurcm_t member
);

#endif //IUSLIBRARY_IUSHLRECEIVECHANNELMAPDICT_H

