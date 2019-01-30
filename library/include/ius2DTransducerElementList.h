// Created by nlv09165 on 23/05/2018.
#ifndef IUSLIBRARY_IUSHL2DTRANSDUCERELEMENTLIST_H
#define IUSLIBRARY_IUSHL2DTRANSDUCERELEMENTLIST_H

#include <ius2DTransducerElement.h>

// ADT
typedef struct Ius2DTransducerElementList Ius2DTransducerElementList;
/** \brief A 2D transducer element list is a list object of 2D transducer elements. Elements are created first with #ius2DTransducerElementCreate(),
* and then added to a list using #ius2DTransducerSetElement(), after an empty list is created with #ius2DTransducerElementListCreate().
*/
typedef Ius2DTransducerElementList *iu2dtel_t;
#define  IU2DTEL_INVALID (iu2dtel_t) NULL

/** \brief Create an empty list of 2D elements with length \p num2DTransducerElement
* \return Returns the created list or #IU2DTEL_INVALID in case this failed.
*/
iu2dtel_t ius2DTransducerElementListCreate
(
    int numElements ///< the number of elements that will go in the element list
);

int ius2DTransducerElementListDeepDelete
(
    iu2dtel_t list 
);

/** \brief Delete an existing \p list of elements
* \returns #IUS_TRUE in case the list is deleted successfully or #IUS_ERR_VALUE when something went wrong. 
*/
int ius2DTransducerElementListDelete
(
    iu2dtel_t list ///< the list to be deleted 
);

/** \brief Check if two lists are identical. the order of the paramters doesn't matter.
* \returns #IUS_TRUE in case the list content of the list is th same or #IUS_FALSE when they differ.
*/
int ius2DTransducerElementListCompare
(
    iu2dtel_t reference,         ///< the reference list
    iu2dtel_t actual             ///< the list to be compared
);

/** \brief Returns the length of the element list
* \returns the length of the element list or -1 in case of an error.
*/
int ius2DTransducerElementListGetSize
(
    iu2dtel_t list  ///< the element list of interest
);

/** \brief Gets the transducer element with the given \p index
* \returns the requested element or NULL in case the parameters are incorrect.
*/
iu2dte_t ius2DTransducerElementListGet
(
    iu2dtel_t list,  ///< the transducer's element list
    int index        ///< the referenced index
);

/** \brief Sets the transducer element at the given \p index in the list.
* \returns #IUS_E_OK when successful or #IUS_ERR_VALUE when the function fails.
*/
int ius2DTransducerElementListSet
(
    iu2dtel_t list,                   ///< The 2D transducer element list
    iu2dte_t member,                  ///< The 2D element to set in the list
    int index                         ///< the index in the list to set the element
);

#endif //IUSLIBRARY_IUSHL2DTRANSDUCERELEMENTLIST_H


