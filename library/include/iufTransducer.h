// Created by nlv09165 on 24/05/2018.
#ifndef IUFLIBRARY_IUFHLTRANSDUCER_H
#define IUFLIBRARY_IUFHLTRANSDUCER_H

#include <iufTypes.h>
#include <iufHDF5.h>

// ADT
typedef struct IufTransducer IufTransducer;
typedef IufTransducer *iut_t;
#define  IUT_INVALID (iut_t) NULL

//iut_t iufTransducerCreate
//(
//    const char *name,
//    IufTransducerShape shape,
//    float centerFrequency
//);

int iufTransducerDelete
(
    iut_t transducer
);

int iufTransducerDeepDelete
(
    iut_t transducer
);


// operations
int iufTransducerCompare
(
    iut_t reference,
    iut_t actual
);

// Getters
float iufTransducerGetCenterFrequency
(
    iut_t transducer
);

int iufTransducerGetNumElements
(
    iut_t transducer
);

char *iufTransducerGetName
(
    iut_t transducer
);

IufTransducerShape iufTransducerGetShape
(
    iut_t transducer
);

//iute_t iufTransducerGetElement
//(
//    iut_t transducer,
//    int elementIndex
//);

// Setters
//int iufTransducerSetElement
//(
//    iut_t transducer,
//    int elementIndex,
//    iute_t transducerElement
//);

#endif //IUFLIBRARY_IUFHLTRANSDUCER_H
