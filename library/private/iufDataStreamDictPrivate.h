// Created by nlv09165 on 03/05/2018.
#ifndef IUFLIBRARY_IUFDATASTREAMDICT_H
#define IUFLIBRARY_IUFDATASTREAMDICT_H

#include <iufHDF5.h>
#include <iufDataStream.h>

// ADT
typedef struct IufDataStreamDict IufDataStreamDict;
typedef IufDataStreamDict *iudsd_t;
#define  IUDSD_INVALID (iudsd_t) NULL

iudsd_t iufDataStreamDictCreate
(
	void
);

int iufDataStreamDictDelete
(
	iudsd_t dict
);

int iufDataStreamDictCompare
(
	iudsd_t reference,
	iudsd_t actual
);

int iufDataStreamDictGetSize
(
	iudsd_t dict
);

iuds_t iufDataStreamDictGet
(
	iudsd_t dict,
	char * key
);

int iufDataStreamDictSet
(
	iudsd_t dict,
	char * key,
	iuds_t member
);

#endif //IUFLIBRARY_IUFHLPULSEDICT_H
