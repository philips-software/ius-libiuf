// Created by nlv12901 on 20/07/2018.
#ifndef IUSLIBRARY_IUSHLTRANSMITAPODIZATIONDICT_H
#define IUSLIBRARY_IUSHLTRANSMITAPODIZATIONDICT_H

#include <stdio.h>
#include <iusTransmitApodization.h>

// ADT
typedef struct IusTransmitApodizationDict IusTransmitApodizationDict;
typedef IusTransmitApodizationDict *iutad_t;
#define  IUTAD_INVALID (iutad_t) NULL

iutad_t iusTransmitApodizationDictCreate
(
	void
);

int iusTransmitApodizationDictDelete
(
	iutad_t dict
);

int iusTransmitApodizationDictCompare
(
	iutad_t reference,
	iutad_t actual
);

size_t iusTransmitApodizationDictGetSize
(
	iutad_t dict
);

iuta_t iusTransmitApodizationDictGet
(
	iutad_t list,
	char * key
);

int iusTransmitApodizationDictSet
(
	iutad_t dict,
	char * key,
	iuta_t member
);


#endif //IUSLIBRARY_IUSHLTRANSMITAPODIZATIONDICT_H

