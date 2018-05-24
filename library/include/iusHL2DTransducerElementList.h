

// Created by nlv09165 on 23/05/2018.
#ifndef IUSLIBRARY_IUSHL2DTRANSDUCERELEMENTLIST_H
#define IUSLIBRARY_IUSHL2DTRANSDUCERELEMENTLIST_H

#include <include/iusHL2DTransducerElement.h>

// ADT
typedef struct Ius2DTransducerElementList Ius2DTransducerElementList;
typedef Ius2DTransducerElementList *iu2dtel_t;
#define  IU2DTEL_INVALID (iu2dtel_t) NULL

iu2dtel_t iusHL2DTransducerElementListCreate
(
    int num2DTransducerElement
);

int iusHL2DTransducerElementListDelete
(
    iu2dtel_t list
);

int iusHL2DTransducerElementListCompare
(
    iu2dtel_t reference,
    iu2dtel_t actual
);

int iusHL2DTransducerElementListGetSize
(
    iu2dtel_t list
);

iu2dte_t iusHL2DTransducerElementListGet
(
    iu2dtel_t list,
    int index
);

int iusHL2DTransducerElementListSet
(
    iu2dtel_t list,
    iu2dte_t member,
    int index
);

#endif //IUSLIBRARY_IUSHL2DTRANSDUCERELEMENTLIST_H
