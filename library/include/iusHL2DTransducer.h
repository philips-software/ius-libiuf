// Created by nlv09165 on 23/05/2018.
#ifndef IUSLIBRARY_IUSHL2DTRANSDUCER_H
#define IUSLIBRARY_IUSHL2DTRANSDUCER_H

#include <iusHL2DTransducerElement.h>

// ADT
typedef struct Ius2DTransducer Ius2DTransducer;
typedef Ius2DTransducer *iu2dt_t;
#define  IU2DT_INVALID (iu2dt_t) NULL

iu2dt_t iusHL2DTransducerCreate
(
  char *name,
  IusTransducerShape shape,
  float centerFrequency,
  int numElements
);

int iusHL2DTransducerDelete
(
  iu2dt_t transducer
);


// operations
int iusHL2DTransducerCompare
(
  iu2dt_t reference,
  iu2dt_t actual
);

// getters
iu2dte_t iusHL2DTransducerGetElement
(
  iu2dt_t transducer,
  int elementIndex
);

int iusHL2DTransducerGetNumElements
(
  iu2dt_t transducer
);

// setters
int iusHL2DTransducerSetElement(
  iu2dt_t transducer,
  int elementIndex,
  iu2dte_t element
);

#endif //IUSLIBRARY_IUSHL2DTRANSDUCER_H
