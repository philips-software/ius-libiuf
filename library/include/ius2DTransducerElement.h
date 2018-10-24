// Created by nlv09165 on 23/05/2018.
#ifndef IUSLIBRARY_IUSHL2DTRANSDUCERELEMENT_H
#define IUSLIBRARY_IUSHL2DTRANSDUCERELEMENT_H

#include <ius2DSize.h>
#include <iusPosition.h>
#include <iusTransducer.h>

// ADT
typedef struct Ius2DTransducerElement Ius2DTransducerElement;
typedef Ius2DTransducerElement *iu2dte_t;
#define  IU2DTE_INVALID (iu2dte_t) NULL


iu2dte_t ius2DTransducerElementCreate
(
    iu2dp_t pos,
    float theta,
    iu2ds_t siz
);

//int ius2DTransducerElementDeepDelete
//(
//    iu2dte_t ius2DTransducerElement
//);

int ius2DTransducerElementDelete
(
    iu2dte_t ius2DTransducerElement
);


// operations
int ius2DTransducerElementCompare
(
    iu2dte_t reference,
    iu2dte_t actual
);

// Getters
iu2dp_t ius2DTransducerElementGetPosition
(
    iu2dte_t ius2DTransducerElement
);

float ius2DTransducerElementGetAngle
(
    iu2dte_t ius2DTransducerElement
);

iu2ds_t ius2DTransducerElementGetSize
(
    iu2dte_t ius2DTransducerElement
);

#endif //IUSLIBRARY_IUSHL2DTRANSDUCERELEMENT_H
