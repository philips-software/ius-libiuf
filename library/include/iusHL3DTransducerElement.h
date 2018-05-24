
// Created by nlv09165 on 23/05/2018.
#ifndef IUSLIBRARY_IUSHL3DTRANSDUCERELEMENT_H
#define IUSLIBRARY_IUSHL3DTRANSDUCERELEMENT_H

//#ifndef IUSLIBRARY_IMPLEMENTATION
//typedef struct Ius3DTransducerElement Ius3DTransducerElement;
//#endif // IUSLIBRARY_IMPLEMENTATION

#include "iusHL3DAngle.h"
#include "iusHL3DSize.h"
#include "iusHLPosition.h"

// ADT
typedef struct Ius3DTransducerElement Ius3DTransducerElement;
typedef Ius3DTransducerElement *iu3dte_t;
#define  IU3DTE_INVALID (iu3dte_t) NULL

iu3dte_t iusHL3DTransducerElementCreate
(
  iu3dp_t pos,
  iu3da_t ang,
  iu3ds_t siz
);

int iusHL3DTransducerElementDelete
(
    iu3dte_t ius3DTransducerElement
);


// operations
int iusHL3DTransducerElementCompare
(
    iu3dte_t reference,
    iu3dte_t actual
);


#endif //IUSLIBRARY_IUSHL3DTRANSDUCERELEMENT_H
