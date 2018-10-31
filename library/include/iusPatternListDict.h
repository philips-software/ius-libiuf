// Created by nlv09165 on 03/05/2018.
#ifndef IUSLIBRARY_IUSPATTERNLISTDICT_H
#define IUSLIBRARY_IUSPATTERNLISTDICT_H

#include <iusHDF5.h>
#include <iusPatternList.h>

// ADT
typedef struct IusPatternListDict IusPatternListDict;
typedef IusPatternListDict *iupald_t;
#define  IUPALD_INVALID (iupald_t) NULL

iupald_t iusPatternListDictCreate
(
	void
);

int iusPatternListDictDelete
(
	iupald_t dict
);

int iusPatternListDictDeepDelete
(
	iupald_t dict
);

int iusPatternListDictCompare
(
	iupald_t reference,
	iupald_t actual
);

int iusPatternListDictGetSize
(
	iupald_t dict
);

iupal_t iusPatternListDictGet
(
	iupald_t dict,
	char * key
);

int iusPatternListDictSet
(
	iupald_t dict,
	char * key,
	iupal_t member
);

#endif //IUSLIBRARY_IUSHLPULSEDICT_H
