#ifndef IUFLIBRARY_IUFHL2DTRANSDUCERELEMENTLIST_H
#define IUFLIBRARY_IUFHL2DTRANSDUCERELEMENTLIST_H

#include <iuf2DTransducerElement.h>

// ADT
typedef struct Iuf2DTransducerElementList Iuf2DTransducerElementList;
/** \brief A 2D transducer element list is a list object of 2D transducer elements. Elements are created first with #iuf2DTransducerElementCreate(),
* and then added to a list using #iuf2DTransducerSetElement(), after an empty list is created with #iuf2DTransducerElementListCreate().
*/
typedef Iuf2DTransducerElementList *iu2dtel_t;
#define  IU2DTEL_INVALID (iu2dtel_t) NULL

/** \brief Create an empty list of 2D elements with length \p num2DTransducerElement
* \return Returns the created list or #IU2DTEL_INVALID in case this failed.
*/
iu2dtel_t iuf2DTransducerElementListCreate
(
    int numElements ///< the number of elements that will go in the element list
);

int iuf2DTransducerElementListDeepDelete
(
    iu2dtel_t list 
);

/** \brief Delete an existing \p list of elements
* \returns #IUF_TRUE in case the list is deleted successfully or #IUF_ERR_VALUE when something went wrong.
*/
int iuf2DTransducerElementListDelete
(
    iu2dtel_t list ///< the list to be deleted 
);

/** \brief Check if two lists are identical. the order of the paramters doesn't matter.
* \returns #IUF_TRUE in case the list content of the list is th same or #IUF_FALSE when they differ.
*/
int iuf2DTransducerElementListCompare
(
    iu2dtel_t reference,         ///< the reference list
    iu2dtel_t actual             ///< the list to be compared
);

/** \brief Returns the length of the element list
* \returns the length of the element list or -1 in case of an error.
*/
int iuf2DTransducerElementListGetSize
(
    iu2dtel_t list  ///< the element list of interest
);

/** \brief Gets the transducer element with the given \p index
* \returns the requested element or NULL in case the parameters are incorrect.
*/
iu2dte_t iuf2DTransducerElementListGet
(
    iu2dtel_t list,  ///< the transducer's element list
    int index        ///< the referenced index
);

/** \brief Sets the transducer element at the given \p index in the list.
* \returns #IUF_E_OK when successful or #IUF_ERR_VALUE when the function fails.
*/
int iuf2DTransducerElementListSet
(
    iu2dtel_t list,                   ///< The 2D transducer element list
    iu2dte_t member,                  ///< The 2D element to set in the list
    int index                         ///< the index in the list to set the element
);

#endif //IUFLIBRARY_IUFHL2DTRANSDUCERELEMENTLIST_H


