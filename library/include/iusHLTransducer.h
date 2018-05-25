// Created by nlv09165 on 24/05/2018.
#ifndef IUSLIBRARY_IUSHLTRANSDUCER_H
#define IUSLIBRARY_IUSHLTRANSDUCER_H

#include <iusTypes.h>
#include <iusHLTransducerElement.h>

// ADT
typedef struct IusTransducer IusTransducer;
typedef IusTransducer *iut_t;
#define  IUT_INVALID (iut_t) NULL

iut_t iusHLTransducerCreate
(
    char *name,
    IusTransducerShape shape,
    float centerFrequency
);

int iusHLTransducerDelete
(
    iut_t iusTransducer
);


// operations
int iusHLTransducerCompare
(
    iut_t reference,
    iut_t actual
);

// Getters
float iusHLTransducerGetCenterFrequency
(
    iut_t transducer
);

int iusHLTransducerGetNumElements
(
    iut_t transducer
);

char *iusHLTransducerGetName
(
    iut_t transducer
);

IusTransducerShape iusHLTransducerGetShape
(
    iut_t transducer
);

iute_t iusHLTransducerGetElement
(
    iut_t transducer,
    int elementIndex
);

// Setters
int iusHLTransducerSetElement
(
    iut_t transducer,
    int elementIndex,
    iute_t transducerElement
);

#endif //IUSLIBRARY_IUSHLTRANSDUCER_H
