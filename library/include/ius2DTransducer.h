// Created by nlv09165 on 23/05/2018.
#ifndef IUSLIBRARY_IUSHL2DTRANSDUCER_H
#define IUSLIBRARY_IUSHL2DTRANSDUCER_H

#include <ius2DTransducerElement.h>

// ADT
typedef struct Ius2DTransducer Ius2DTransducer;
typedef Ius2DTransducer *iu2dt_t;
#define  IU2DT_INVALID (iu2dt_t) NULL

iu2dt_t ius2DTransducerCreate
(
  char *name,
  IusTransducerShape shape,
  float centerFrequency,
  int numElements
);

int ius2DTransducerDelete
(
  iu2dt_t transducer
);

int ius2DTransducerDeepDelete
(
    iu2dt_t transducer
);


// operations
int ius2DTransducerCompare
(
  iu2dt_t reference,
  iu2dt_t actual
);

// getters
iu2dte_t ius2DTransducerGetElement
(
  iu2dt_t transducer,
  int elementIndex
);

int ius2DTransducerGetNumElements
(
  iu2dt_t transducer
);

// setters
int ius2DTransducerSetElement(
  iu2dt_t transducer,
  int elementIndex,
  iu2dte_t element
);

#endif //IUSLIBRARY_IUSHL2DTRANSDUCER_H
