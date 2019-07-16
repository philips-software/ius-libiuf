#ifndef IUFLIBRARY_IUFHL2DTRANSDUCER_H
#define IUFLIBRARY_IUFHL2DTRANSDUCER_H

#include <iuf2DTransducerElement.h>

// ADT
typedef struct Iuf2DTransducer Iuf2DTransducer;

/** \brief A 2D transducer is described by a name, 2D shape, centerfrequency and a list of elements.
* A transducer is created first by specifying these parameters with a call to #iuf2DTransducerCreate(),
* followed by setting each of the elements using calls to #iuf2DTransducerSetElement()
*/
typedef Iuf2DTransducer *iu2dt_t;
#define  IU2DT_INVALID (iu2dt_t) NULL

/** \brief Constructs the 2D transducer
* \return Returns an allocated 2D tansducer, with uninitialized elements
*/
iu2dt_t iuf2DTransducerCreate
(
  char *name,                       ///< The name of the transducer 
  IufTransducerShape shape,         ///< The 2D shape of the transducer: should be a #IUF_LINE or an IUF_CIRCLE
  float centerFrequency,            ///< The operating frequency the transducer  
  int numElements                   ///< The number of elements that a transducer contains 
);


int iuf2DTransducerDelete
(
  iu2dt_t transducer                ///< The 2D transducer object that will be deleted
);

//todo: move this to private
int iuf2DTransducerDeepDelete
(
    iu2dt_t transducer             
);

/** \brief Comparing two transducers, return #IUF_TRUE if they are equal and #IUF_FALSE if they differ or in case of an error.
*  The order of the arguments does no matter
*/
int iuf2DTransducerCompare
(
  iu2dt_t reference,  ///< one of the 2D transducers 
  iu2dt_t actual      ///< the other 2D transducer 
);

/** \brief Return the \p elemntIndex -th transducer element from the \p transducer's element list.
*  /return the requested 2D element from the transducer's element list.
*/
iu2dte_t iuf2DTransducerGetElement
(
  iu2dt_t transducer,           ///< the transducer to compare
  int index              ///< the element index from the element list
);

/** \brief Gets the length of the transducer's element list.
*  /return the size of the element list.
*/
int iuf2DTransducerGetNumElements
(
  iu2dt_t transducer ///< the referenced transducer
);

/** \brief Set a \p element in the transducer's element list at position \p elementIndex
*  /return #IUF_ERR_VALUE is case the element can not be set or #IUF_E_OK in case of success.
*/
int iuf2DTransducerSetElement(
  iu2dt_t transducer,           ///< the transducer element
  int index,             ///< the position in the element list
  iu2dte_t element              ///< the transducer element to be set
);

#endif //IUFLIBRARY_IUFHL2DTRANSDUCER_H
