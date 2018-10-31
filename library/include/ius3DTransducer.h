// Created by nlv09165 on 23/05/2018.
#ifndef IUSLIBRARY_IUSHL3DTRANSDUCER_H
#define IUSLIBRARY_IUSHL3DTRANSDUCER_H

#include <ius3DTransducerElement.h>
#include <iusHDF5.h>

// ADT
typedef struct Ius3DTransducer Ius3DTransducer;
typedef Ius3DTransducer *iu3dt_t;
#define  IU3DT_INVALID (iu3dt_t) NULL

iu3dt_t ius3DTransducerCreate
(
    char *name,
    IusTransducerShape shape,
    float centerFrequency,
    int numElements
);

int ius3DTransducerDelete
(
    iu3dt_t ius3DTransducer
);

int ius3DTransducerDeepDelete
(
	iu3dt_t ius3DTransducer
);

// operations
int ius3DTransducerCompare
(
    iu3dt_t reference,
    iu3dt_t actual
);

// getters
iu3dte_t ius3DTransducerGetElement
(
    iu3dt_t transducer,
    int elementIndex
);

int ius3DTransducerGetNumElements
(
    iu3dt_t transducer
);

// setters
int ius3DTransducerSetElement(
    iu3dt_t transducer,
    int elementIndex,
    iu3dte_t element
);


herr_t ius3DTransducerWrite
(
	iu3dt_t transducer,
	hid_t subgroup_id
);

#endif //IUSLIBRARY_IUSHL3DTRANSDUCER_H
