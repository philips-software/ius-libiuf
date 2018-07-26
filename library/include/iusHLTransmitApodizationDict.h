// Created by nlv12901 on 20/07/2018.
#ifndef IUSLIBRARY_IUSHLTRANSMITAPODIZATIONDICT_H
#define IUSLIBRARY_IUSHLTRANSMITAPODIZATIONDICT_H

#include <iusHLTransmitApodization.h>
#include <iusHLTransmitApodizationImp.h>

// ADT
typedef struct IusTransmitApodizationDict IusTransmitApodizationDict;
typedef IusTransmitApodizationDict *iutad_t;
#define  IUTAD_INVALID (iutad_t) NULL

iutad_t iusHLTransmitApodizationDictCreate
(
	void
);

int iusHLTransmitApodizationDictDelete
(
	iutad_t dict
);

int iusHLTransmitApodizationDictCompare
(
	iutad_t reference,
	iutad_t actual
);

size_t iusHLTransmitApodizationDictGetSize
(
	iutad_t dict
);

iuta_t iusHLTransmitApodizationDictGet
(
	iutad_t list,
	char * key
);

int iusHLTransmitApodizationDictSet
(
	iutad_t dict,
	char * key,
	iuta_t member
);

herr_t iusHLTransmitApodizationDictSave
(
	iutad_t dict,
	char *parentPath,
	hid_t handle
);

iutad_t iusHLTransmitApodizationDictLoad
(
	hid_t handle,
	char *parentPath
);

#endif //IUSLIBRARY_IUSHLTRANSMITAPODIZATIONDICT_H

