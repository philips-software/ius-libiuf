// Created by nlv09165 on 23/05/2018.
#ifndef IUSLIBRARY_IUSHL2DTRANSDUCERELEMENT_H
#define IUSLIBRARY_IUSHL2DTRANSDUCERELEMENT_H

#include <ius2DSize.h>
#include <iusPosition.h>
#include <iusTransducer.h>

// ADT
typedef struct Ius2DTransducerElement Ius2DTransducerElement;
/** \brief A 2D transducer element describes a physical ultasound element. In 2D space such an element is described by a 2D size
* at a 2D position under a single angle theta.Typically, a transducer has a list of these elements. Elements are created first with #ius2DTransducerElementCreate(),
* and then added to a list using #ius2DTransducerSetElement()
*/
typedef Ius2DTransducerElement *iu2dte_t;
#define  IU2DTE_INVALID (iu2dte_t) NULL

/** \brief Creates a 2D transducer element that is decribed by a 2D position, an angle theta and a 2D size
* \return returns the created element or #IU2DTE_INVALID in case of invalid parameters.
*/
iu2dte_t ius2DTransducerElementCreate
(
    iu2dp_t pos,   ///< the 2D position (x,z) in meters
    float theta,   ///< the angle over the y-axis in radians of the element
    iu2ds_t size   ///< the size (sx,sz) in meters of the element
);

/** \brief Removes the 2D element object an its sub-objects.
* \return #IUS_ERR_VALUE in case something went wrong or #IUS_TRUE when the element has been successfully deleted.
*/
int ius2DTransducerElementDeepDelete
(
    iu2dte_t ius2DTransducerElement
);

/** \brief Removes the 2D element object without deleting the position and size sub-objects.
* \return #IUS_ERR_VALUE in case something went wrong or #IUS_TRUE when the element has been successfully deleted.
*/
int ius2DTransducerElementDelete
(
    iu2dte_t ius2DTransducerElement
);

/** \brief Compares the values of 2 transducer elements. the order of the arguments don't matter.
* \return #IUS_FALSE in case the elements are different and #IUS_TRUE when the element are identical.
*/
int ius2DTransducerElementCompare
(
    iu2dte_t reference,      ///< the transducer element to be compared 
    iu2dte_t actual          ///< the transducer element that is compared
);

/** \brief returns the 2D position of the \p ius2DTransducerElement.
* \return the 2D position of the \p ius2DTransducerElement (x,z) in meters.
*/
iu2dp_t ius2DTransducerElementGetPosition
(
    iu2dte_t ius2DTransducerElement ///<the element of interest
);

/** \brief returns the angle of the \p ius2DTransducerElement.
* \return the angle of the \p ius2DTransducerElement (theta) in radians.
*/
float ius2DTransducerElementGetAngle
(
    iu2dte_t ius2DTransducerElement ///<the element of interest
);

/** \brief returns the 2D size of the \p ius2DTransducerElement.
* \return the 2D size of the \p ius2DTransducerElement (sx,sz) in meters.
*/
iu2ds_t ius2DTransducerElementGetSize
(
    iu2dte_t ius2DTransducerElement ///<the element of interest
);

#endif //IUSLIBRARY_IUSHL2DTRANSDUCERELEMENT_H
