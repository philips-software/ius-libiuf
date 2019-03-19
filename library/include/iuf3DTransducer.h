// Created by nlv09165 on 23/05/2018.
#ifndef IUFLIBRARY_IUFHL3DTRANSDUCER_H
#define IUFLIBRARY_IUFHL3DTRANSDUCER_H

#include <iuf3DTransducerElement.h>
#include <iufHDF5.h>

typedef struct Iuf3DTransducer Iuf3DTransducer;
/** \brief A 3D transducer is described by a name, 3D shape, centerfrequency and a list of elements.
* A transducer is created first by specifying these parameters with a call to #iuf2DTransducerCreate(),
* followed by setting each of the elements using calls to #iuf2DTransducerSetElement()
*/
typedef Iuf3DTransducer *iu3dt_t;
#define  IU3DT_INVALID (iu3dt_t) NULL

/** \brief Constructs the 3D transducer
* \return Returns an allocated 3D tansducer, with uninitialized elements
*/
iu3dt_t iuf3DTransducerCreate
(
    char *name,						///< The name (label) of the transducer 
    IufTransducerShape shape,       ///< The 3D shape of the transducer: should be a #IUF_PLANE, #IUF_CYLINDER, or #IUF_SPHERE.
    float centerFrequency,          ///< ///< The operating frequency the transducer
    int numElements                 ///< The number of transducer elements this transducer has
);

/** \brief Deletes the 3D transducer without deleting the elements
* \return Returns #IUF_E_OK if the object could be deleted or #IUF_ERR_VALUE in case this failed.
*/
int iuf3DTransducerDelete
(
    iu3dt_t transducer       ///< The 3D transducer object that will be deleted
);

/** \brief Deletes the 3D transducer including the deletion of its elements
* \return Returns #IUF_E_OK if the object could be deleted or #IUF_ERR_VALUE in case this failed.
*/
int iuf3DTransducerDeepDelete
(
	iu3dt_t transducer      ///< The 3D transducer object that will be deleted
);

/** \brief Comparing two transducers, return #IUF_TRUE if they are equal and #IUF_FALSE if they differ or in case of an error.
*  The order of the arguments does no matter
*/
int iuf3DTransducerCompare
(
    iu3dt_t reference,		///< the 3D transducer to compare to
    iu3dt_t actual			///< the 3D transducer to compare with
);

/** \brief Return the \p elemntIndex -th transducer element from the \p transducer's element list.
*  /return the requested 3D element from the transducer's element list.
*/
iu3dte_t iuf3DTransducerGetElement
(
    iu3dt_t transducer,		///< The 3D transducer of interest
    int index		///< The index of the element that is requested
);

/** \brief Gets the length of the transducer's element list.
*  /return the size of the element list.
*/
int iuf3DTransducerGetNumElements
(
    iu3dt_t transducer		///< The transducer of interest
);

/** \brief Set a \p element in the transducer's element list at position \p elementIndex
*  /return #IUF_ERR_VALUE is case the element can not be set or #IUF_E_OK in case of success.
*/
int iuf3DTransducerSetElement(
    iu3dt_t transducer,        ///< the transducer element
    int elementIndex,          ///< the position in the element list
    iu3dte_t element           ///< the transducer element to be set
);

//herr_t iuf3DTransducerWrite
//(
//	iu3dt_t transducer,
//	hid_t subgroup_id
//);

#endif //IUFLIBRARY_IUFHL3DTRANSDUCER_H
