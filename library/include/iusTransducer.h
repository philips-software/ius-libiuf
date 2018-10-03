// Created by nlv09165 on 24/05/2018.
#ifndef IUSLIBRARY_IUSHLTRANSDUCER_H
#define IUSLIBRARY_IUSHLTRANSDUCER_H

#include <iusTypes.h>
#include <iusHDF5.h>
#include <iusTransducerElement.h>

// ADT
typedef struct IusTransducer IusTransducer;
typedef IusTransducer *iut_t;
#define  IUT_INVALID (iut_t) NULL

//iut_t iusTransducerCreate
//(
//    char *name,
//    IusTransducerShape shape,
//    float centerFrequency
//);

int iusTransducerDelete
(
    iut_t iusTransducer
);


// operations
int iusTransducerCompare
(
    iut_t reference,
    iut_t actual
);

// Getters
float iusTransducerGetCenterFrequency
(
    iut_t transducer
);

int iusTransducerGetNumElements
(
    iut_t transducer
);

char *iusTransducerGetName
(
    iut_t transducer
);

IusTransducerShape iusTransducerGetShape
(
    iut_t transducer
);

//iute_t iusTransducerGetElement
//(
//    iut_t transducer,
//    int elementIndex
//);

// Setters
//int iusTransducerSetElement
//(
//    iut_t transducer,
//    int elementIndex,
//    iute_t transducerElement
//);

#endif //IUSLIBRARY_IUSHLTRANSDUCER_H
