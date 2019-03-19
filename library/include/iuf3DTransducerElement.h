// Created by nlv09165 on 23/05/2018.
#ifndef IUFLIBRARY_IUFHL3DTRANSDUCERELEMENT_H
#define IUFLIBRARY_IUFHL3DTRANSDUCERELEMENT_H

#include <iuf3DAngle.h>
#include <iuf3DSize.h>
#include <iufPosition.h>

// ADT
typedef struct Iuf3DTransducerElement Iuf3DTransducerElement;
/** \brief A 3D transducer element describes a physical ultrasound element. In 3D space such an element is described by a 3D size
* at a 3D position under 2 angles theta and phi. Typically, a transducer has a list of these elements. Elements are created first with #iuf3DTransducerElementCreate(),
* and then added to a list using #iuf3DTransducerSetElement()
*/
typedef Iuf3DTransducerElement *iu3dte_t;
#define  IU3DTE_INVALID (iu3dte_t) NULL

/** \brief Creates a 3D transducer element that is decribed by a 3D position, a 3D angle (theta and phi) and a 3D size
* \return returns the created element or #IU2DTE_INVALID in case of invalid parameters.
*/
iu3dte_t iuf3DTransducerElementCreate
(
    iu3dp_t pos, ///< the 3D position (x,y,z) in meters
    iu3da_t ang, ///< the angle over the y-axis and x ais in radians of the element
    iu3ds_t siz  ///< the size (sx,sy, sz) in meters of the element
);

/** \brief Removes the 3D element object and its sub-objects (position, angle and size)
* \return #IUF_ERR_VALUE in case something went wrong or #IUF_TRUE when the element has been successfully deleted.
*/
int iuf3DTransducerElementDeepDelete
(
	iu3dte_t element ///< the 3D transducer object to be deleted
);

/** \brief Removes the 3D element object without deleting its sub-objects (position, angle and size)
* \return #IUF_ERR_VALUE in case something went wrong or #IUF_TRUE when the element has been successfully deleted.
*/
int iuf3DTransducerElementDelete
(
    iu3dte_t element ///< the 3D transducer object to be deleted
);

/** \brief Compares the values of 2 transducer elements. the order of the arguments don't matter.
* \return #IUF_FALSE in case the elements are different and #IUF_TRUE when the element are identical.
*/
int iuf3DTransducerElementCompare
(
    iu3dte_t reference,				///< The transducer to compare to 
    iu3dte_t actual                 ///< The transducer to compare with
);

/** \brief returns the 3D position of the \p iuf2DTransducerElement.
* \return the 3D position of the \p iuf3DTransducerElement (x,y,z) in meters.
*/
iu3dp_t iuf3DTransducerElementGetPosition
(
	iu3dte_t element ///< The transducer element that has the requested position.
);

/** \brief returns the angles (theta,phi) of the \p iuf2DTransducerElement.
* \return the angles of the \p iuf3DTransducerElement (theta, phi) in radians.
*/
iu3da_t iuf3DTransducerElementGetAngle
(
	iu3dte_t element ///< The transducer element that has the requested angles.
);

/** \brief returns the 3D size of the \p iuf2DTransducerElement.
* \return the 3D size of the \p iuf3DTransducerElement (sx,sy,sz) in meters.
*/
iu3ds_t iuf3DTransducerElementGetSize
(
	iu3dte_t element  ///< The transducer element that has the requested size.
);
#endif //IUFLIBRARY_IUFHL3DTRANSDUCERELEMENT_H
