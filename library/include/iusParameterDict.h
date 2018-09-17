// Created by nlv09165 on 12/09/2018.
#ifndef IUSLIBRARY_IUSPARAMETERDICT_H
#define IUSLIBRARY_IUSPARAMETERDICT_H


// ADT
typedef struct IusParameterDict IusParameterDict;
typedef IusParameterDict *iupad_t;
#define  IUPAD_INVALID (iupad_t) NULL

iupad_t iusParameterDictCreate
(
);

int iusParameterDictDelete
(
    iupad_t iusParameterDict
);


// operations
int iusParameterDictCompare
(
    iupad_t reference,
    iupad_t actual
);

int iusParameterDictGetSize
(
    iupad_t dict
);

char * iusParameterDictGet
(
    iupad_t dict,
    char * key
);

int iusParameterDictSet
(
    iupad_t dict,
    char * key,
    char * value
);
#endif //IUSLIBRARY_IUSPARAMETERDICT_H
