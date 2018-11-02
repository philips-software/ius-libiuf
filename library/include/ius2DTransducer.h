// Created by nlv09165 on 23/05/2018.
#ifndef IUSLIBRARY_IUSHL2DTRANSDUCER_H
#define IUSLIBRARY_IUSHL2DTRANSDUCER_H

#include <ius2DTransducerElement.h>

// ADT
typedef struct Ius2DTransducer Ius2DTransducer;
/** \brief A 2D transducer is described by a name, 2D shape, centerfrequence and a list of elements.   
* A transducer is created first by specifying these parameters with a call to #ius2DTransducerCreate(), 
* followed by setting each of the elements using calls to #ius2DTransducerSetElement()
*/
typedef Ius2DTransducer *iu2dt_t;
#define  IU2DT_INVALID (iu2dt_t) NULL

/** \brief Constructs the 2D transducer
*
*/
iu2dt_t ius2DTransducerCreate
(
  char *name,                       ///< The name of the transducer 
  IusTransducerShape shape,         ///< The 2D shape of the transducer: should be a #IUS_LINE or an IUS_CIRCLE
  float centerFrequency,            ///< The operating frequency the transducer  
  int numElements                   ///< The number of elements that a transducer contains 
);

int ius2DTransducerDelete
(
  iu2dt_t transducer                ///< The 2D trasnducer that will be deleted name of the transducer 
);

//todo: move this to private
int ius2DTransducerDeepDelete
(
    iu2dt_t transducer             
);

/** \brief Comparing two transducers, return #IUS_TRUE if they are equal and #IUS_FALSE if they differ are in case of an error.
*/
int ius2DTransducerCompare
(
  iu2dt_t reference,  ///< one of the 2D transducers 
  iu2dt_t actual      ///< the other 2D transducer 
);

/** \brief Comparing two transducers, return #IUS_TRUE if they are equal and #IUS_FALSE if they differ are in case of an error.
*
*/
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
