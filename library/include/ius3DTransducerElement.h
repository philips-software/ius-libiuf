// Created by nlv09165 on 23/05/2018.
#ifndef IUSLIBRARY_IUSHL3DTRANSDUCERELEMENT_H
#define IUSLIBRARY_IUSHL3DTRANSDUCERELEMENT_H

#include <ius3DAngle.h>
#include <ius3DSize.h>
#include <iusPosition.h>

// ADT
typedef struct Ius3DTransducerElement Ius3DTransducerElement;
/** \brief A 3D transducer element describes a physical ultrasound element. In 3D space such an element is described by a 3D size
* at a 3D position under 2 angles theta and phi. Typically, a transducer has a list of these elements. Elements are created first with #ius3DTransducerElementCreate(),
* and then added to a list using #ius3DTransducerSetElement()
*/
typedef Ius3DTransducerElement *iu3dte_t;
#define  IU3DTE_INVALID (iu3dte_t) NULL

/** \brief Creates a 3D transducer element that is decribed by a 3D position, a 3D angle (theta and phi) and a 3D size
* \return returns the created element or #IU2DTE_INVALID in case of invalid parameters.
*/
iu3dte_t ius3DTransducerElementCreate
(
    iu3dp_t pos, ///< the 3D position (x,y,z) in meters
    iu3da_t ang, ///< the angle over the y-axis and x ais in radians of the element
    iu3ds_t siz  ///< the size (sx,sy, sz) in meters of the element
);

/** \brief Removes the 3D element object and its sub-objects (position, angle and size)
* \return #IUS_ERR_VALUE in case something went wrong or #IUS_TRUE when the element has been successfully deleted.
*/
int ius3DTransducerElementDeepDelete
(
	iu3dte_t element ///< the 3D transducer object to be deleted
);

/** \brief Removes the 3D element object without deleting its sub-objects (position, angle and size)
* \return #IUS_ERR_VALUE in case something went wrong or #IUS_TRUE when the element has been successfully deleted.
*/
int ius3DTransducerElementDelete
(
    iu3dte_t element ///< the 3D transducer object to be deleted
);

/** \brief Compares the values of 2 transducer elements. the order of the arguments don't matter.
* \return #IUS_FALSE in case the elements are different and #IUS_TRUE when the element are identical.
*/
int ius3DTransducerElementCompare
(
    iu3dte_t reference,				///< The transducer to compare to 
    iu3dte_t actual                 ///< The transducer to compare with
);

/** \brief returns the 3D position of the \p ius2DTransducerElement.
* \return the 3D position of the \p ius3DTransducerElement (x,y,z) in meters.
*/
iu3dp_t ius3DTransducerElementGetPosition
(
	iu3dte_t element ///< The transducer element that has the requested position.
);

/** \brief returns the angles (theta,phi) of the \p ius2DTransducerElement.
* \return the angles of the \p ius3DTransducerElement (theta, phi) in radians.
*/
iu3da_t ius3DTransducerElementGetAngle
(
	iu3dte_t element ///< The transducer element that has the requested angles.
);

/** \brief returns the 3D size of the \p ius2DTransducerElement.
* \return the 3D size of the \p ius3DTransducerElement (sx,sy,sz) in meters.
*/
iu3ds_t ius3DTransducerElementGetSize
(
	iu3dte_t element  ///< The transducer element that has the requested size.
);
#endif //IUSLIBRARY_IUSHL3DTRANSDUCERELEMENT_H
