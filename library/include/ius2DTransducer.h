// Created by nlv09165 on 23/05/2018.
#ifndef IUSLIBRARY_IUSHL2DTRANSDUCER_H
#define IUSLIBRARY_IUSHL2DTRANSDUCER_H

#include <ius2DTransducerElement.h>

// ADT
typedef struct Ius2DTransducer Ius2DTransducer;
/** \brief A 2D transducer is described by a name, 2D shape, centerfrequency and a list of elements.   
* A transducer is created first by specifying these parameters with a call to #ius2DTransducerCreate(), 
* followed by setting each of the elements using calls to #ius2DTransducerSetElement()
*/
typedef Ius2DTransducer *iu2dt_t;
#define  IU2DT_INVALID (iu2dt_t) NULL

/** \brief Constructs the 2D transducer
* \return Returns an allocated 2D tansducer, with uninitialized elements
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
  iu2dt_t transducer                ///< The 2D transducer object that will be deleted
);

//todo: move this to private
int ius2DTransducerDeepDelete
(
    iu2dt_t transducer             
);

/** \brief Comparing two transducers, return #IUS_TRUE if they are equal and #IUS_FALSE if they differ or in case of an error.
*  The order of the arguments does no matter
*/
int ius2DTransducerCompare
(
  iu2dt_t reference,  ///< one of the 2D transducers 
  iu2dt_t actual      ///< the other 2D transducer 
);

/** \brief Return the \p elemntIndex -th transducer element from the \p transducer's element list.
*  /return the requested 2D element from the transducer's element list.
*/
iu2dte_t ius2DTransducerGetElement
(
  iu2dt_t transducer,           ///< the transducer to compare
  int index              ///< the element index from the element list
);

/** \brief Gets the length of the transducer's element list.
*  /return the size of the element list.
*/
int ius2DTransducerGetNumElements
(
  iu2dt_t transducer ///< the referenced transducer
);

/** \brief Set a \p element in the transducer's element list at position \p elementIndex
*  /return #IUS_ERR_VALUE is case the element can not be set or #IUS_E_OK in case of success.
*/
int ius2DTransducerSetElement(
  iu2dt_t transducer,           ///< the transducer element
  int index,             ///< the position in the element list
  iu2dte_t element              ///< the transducer element to be set
);

#endif //IUSLIBRARY_IUSHL2DTRANSDUCER_H
