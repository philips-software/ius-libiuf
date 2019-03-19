// Created by nlv09165 on 23/05/2018.
#ifndef IUFLIBRARY_IUFHL2DTRANSDUCERELEMENT_H
#define IUFLIBRARY_IUFHL2DTRANSDUCERELEMENT_H

#include <iuf2DSize.h>
#include <iufPosition.h>
#include <iufTransducer.h>

// ADT
typedef struct Iuf2DTransducerElement Iuf2DTransducerElement;
/** \brief A 2D transducer element describes a physical ultasound element. In 2D space such an element is described by a 2D size
* at a 2D position under a single angle theta.Typically, a transducer has a list of these elements. Elements are created first with #iuf2DTransducerElementCreate(),
* and then added to a list using #iuf2DTransducerSetElement()
*/
typedef Iuf2DTransducerElement *iu2dte_t;
#define  IU2DTE_INVALID (iu2dte_t) NULL

/** \brief Creates a 2D transducer element that is decribed by a 2D position, an angle theta and a 2D size
* \return returns the created element or #IU2DTE_INVALID in case of invalid parameters.
*/
iu2dte_t iuf2DTransducerElementCreate
(
    iu2dp_t pos,   ///< the 2D position (x,z) in meters
    float theta,   ///< the angle over the y-axis in radians of the element
    iu2ds_t size    ///< the size (sx,sz) in meters of the element
);

/** \brief Removes the 2D element object an its sub-objects.
* \return #IUF_ERR_VALUE in case something went wrong or #IUF_TRUE when the element has been successfully deleted.
*/
int iuf2DTransducerElementDeepDelete
(
    iu2dte_t element
);

/** \brief Removes the 2D element object without deleting the position and size sub-objects.
* \return #IUF_ERR_VALUE in case something went wrong or #IUF_TRUE when the element has been successfully deleted.
*/
int iuf2DTransducerElementDelete
(
    iu2dte_t element
);

/** \brief Compares the values of 2 transducer elements. the order of the arguments don't matter.
* \return #IUF_FALSE in case the elements are different and #IUF_TRUE when the element are identical.
*/
int iuf2DTransducerElementCompare
(
    iu2dte_t reference,      ///< the transducer element to be compared 
    iu2dte_t actual          ///< the transducer element that is compared
);

/** \brief returns the 2D position of the \p iuf2DTransducerElement.
* \return the 2D position of the \p iuf2DTransducerElement (x,z) in meters.
*/
iu2dp_t iuf2DTransducerElementGetPosition
(
    iu2dte_t element ///<the element of interest
);

/** \brief returns the angle of the \p iuf2DTransducerElement.
* \return the angle of the \p iuf2DTransducerElement (theta) in radians.
*/
float iuf2DTransducerElementGetAngle
(
    iu2dte_t element ///<the element of interest
);

/** \brief returns the 2D size of the \p iuf2DTransducerElement.
* \return the 2D size of the \p iuf2DTransducerElement (sx,sz) in meters.
*/
iu2ds_t iuf2DTransducerElementGetSize
(
    iu2dte_t element ///<the element of interest
);

#endif //IUFLIBRARY_IUFHL2DTRANSDUCERELEMENT_H
