// Created by nlv09165 on 23/05/2018.
#ifndef IUSLIBRARY_IUSHL3DTRANSDUCER_H
#define IUSLIBRARY_IUSHL3DTRANSDUCER_H

#include <ius3DTransducerElement.h>
#include <iusHDF5.h>

typedef struct Ius3DTransducer Ius3DTransducer;
/** \brief A 3D transducer is described by a name, 3D shape, centerfrequency and a list of elements.
* A transducer is created first by specifying these parameters with a call to #ius2DTransducerCreate(),
* followed by setting each of the elements using calls to #ius2DTransducerSetElement()
*/
typedef Ius3DTransducer *iu3dt_t;
#define  IU3DT_INVALID (iu3dt_t) NULL

/** \brief Constructs the 3D transducer
* \return Returns an allocated 3D tansducer, with uninitialized elements
*/
iu3dt_t ius3DTransducerCreate
(
    char *name,						///< The name (label) of the transducer 
    IusTransducerShape shape,       ///< The 3D shape of the transducer: should be a #IUS_PLANE, #IUS_CYLINDER, or #IUS_SPHERE.
    float centerFrequency,          ///< ///< The operating frequency the transducer
    int numElements                 ///< The number of transducer elements this transducer has
);

/** \brief Deletes the 3D transducer without deleting the elements
* \return Returns #IUS_E_OK if the object could be deleted or #IUS_ERR_VALUE in case this failed.
*/
int ius3DTransducerDelete
(
    iu3dt_t transducer       ///< The 3D transducer object that will be deleted
);

/** \brief Deletes the 3D transducer including the deletion of its elements
* \return Returns #IUS_E_OK if the object could be deleted or #IUS_ERR_VALUE in case this failed.
*/
int ius3DTransducerDeepDelete
(
	iu3dt_t transducer      ///< The 3D transducer object that will be deleted
);

/** \brief Comparing two transducers, return #IUS_TRUE if they are equal and #IUS_FALSE if they differ or in case of an error.
*  The order of the arguments does no matter
*/
int ius3DTransducerCompare
(
    iu3dt_t reference,		///< the 3D transducer to compare to
    iu3dt_t actual			///< the 3D transducer to compare with
);

/** \brief Return the \p elemntIndex -th transducer element from the \p transducer's element list.
*  /return the requested 3D element from the transducer's element list.
*/
iu3dte_t ius3DTransducerGetElement
(
    iu3dt_t transducer,		///< The 3D transducer of interest
    int index		///< The index of the element that is requested
);

/** \brief Gets the length of the transducer's element list.
*  /return the size of the element list.
*/
int ius3DTransducerGetNumElements
(
    iu3dt_t transducer		///< The transducer of interest
);

/** \brief Set a \p element in the transducer's element list at position \p elementIndex
*  /return #IUS_ERR_VALUE is case the element can not be set or #IUS_E_OK in case of success.
*/
int ius3DTransducerSetElement(
    iu3dt_t transducer,        ///< the transducer element
    int elementIndex,          ///< the position in the element list
    iu3dte_t element           ///< the transducer element to be set
);

//herr_t ius3DTransducerWrite
//(
//	iu3dt_t transducer,
//	hid_t subgroup_id
//);

#endif //IUSLIBRARY_IUSHL3DTRANSDUCER_H
