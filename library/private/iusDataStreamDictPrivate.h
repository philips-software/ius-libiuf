// Created by nlv09165 on 03/05/2018.
#ifndef IUSLIBRARY_IUSDATASTREAMDICT_H
#define IUSLIBRARY_IUSDATASTREAMDICT_H

#include <iusHDF5.h>
#include <iusDataStream.h>

// ADT
typedef struct IusDataStreamDict IusDataStreamDict;
typedef IusDataStreamDict *iudsd_t;
#define  IUDSD_INVALID (iudsd_t) NULL

iudsd_t iusDataStreamDictCreate
(
	void
);

int iusDataStreamDictDelete
(
	iudsd_t dict
);

int iusDataStreamDictCompare
(
	iudsd_t reference,
	iudsd_t actual
);

int iusDataStreamDictGetSize
(
	iudsd_t dict
);

iuds_t iusDataStreamDictGet
(
	iudsd_t dict,
	char * key
);

int iusDataStreamDictSet
(
	iudsd_t dict,
	char * key,
	iuds_t member
);

#endif //IUSLIBRARY_IUSHLPULSEDICT_H
