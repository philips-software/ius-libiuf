

// Created by nlv09165 on 23/05/2018.
#ifndef IUSLIBRARY_IUSHL3DTRANSDUCERELEMENTLIST_H
#define IUSLIBRARY_IUSHL3DTRANSDUCERELEMENTLIST_H

#include <include/iusHL3DTransducerElement.h>

// ADT
typedef struct Ius3DTransducerElementList Ius3DTransducerElementList;
typedef Ius3DTransducerElementList *iu3dtel_t;
#define  IU3DTEL_INVALID (iu3dtel_t) NULL

iu3dtel_t iusHL3DTransducerElementListCreate
(
    int num3DTransducerElement
);

int iusHL3DTransducerElementListDelete
(
    iu3dtel_t list
);

int iusHL3DTransducerElementListCompare
(
    iu3dtel_t reference,
    iu3dtel_t actual
);

int iusHL3DTransducerElementListGetSize
(
    iu3dtel_t list
);

iu3dte_t iusHL3DTransducerElementListGet
(
    iu3dtel_t list,
    int index
);

int iusHL3DTransducerElementListSet
(
    iu3dtel_t list,
    iu3dte_t member,
    int index
);

#endif //IUSLIBRARY_IUSHL3DTRANSDUCERELEMENTLIST_H
