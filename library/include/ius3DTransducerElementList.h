

// Created by nlv09165 on 23/05/2018.
#ifndef IUSLIBRARY_IUSHL3DTRANSDUCERELEMENTLIST_H
#define IUSLIBRARY_IUSHL3DTRANSDUCERELEMENTLIST_H

#include <include/ius3DTransducerElement.h>

// ADT
typedef struct Ius3DTransducerElementList Ius3DTransducerElementList;
/** \brief A 3D transducer element list is a list object of 3D transducer elements. Elements are created first with #ius3DTransducerElementCreate(),
* and then added to a list using #ius3DTransducerSetElement(), after an empty list is created with #ius3DTransducerElementListCreate().
*/
typedef Ius3DTransducerElementList *iu3dtel_t;
#define  IU3DTEL_INVALID (iu3dtel_t) NULL

/** \brief Create an empty list of 3D elements with length \p num3DTransducerElements
* \return Returns the created list or #IU3DTEL_INVALID in case this failed.
*/
iu3dtel_t ius3DTransducerElementListCreate
(
    int num3DTransducerElements ///< the number of elements that will go in the element list
);

/** \brief Delete an existing \p list of elements including all its elements individually.
* \returns #IUS_TRUE in case the list is deleted successfully or #IUS_ERR_VALUE when something went wrong.
*/
int ius3DTransducerElementListDeepDelete
(
    iu3dtel_t list ///< the list that is to be deleted
);

/** \brief Delete an existing \p list of elements without deleting the individual elements
* \returns #IUS_TRUE in case the list is deleted successfully or #IUS_ERR_VALUE when something went wrong.
*/
int ius3DTransducerElementListDelete
(
    iu3dtel_t list  ///< the list that is to be deleted
);

/** \brief Check if two lists are identical. the order of the parameters doesn't matter.
* \returns #IUS_TRUE in case the list content of the list is th same or #IUS_FALSE when they differ.
*/
int ius3DTransducerElementListCompare
(
    iu3dtel_t reference,  ///< the reference list
    iu3dtel_t actual      ///< the list to be compared
);

/** \brief Returns the length of the element list
* \returns the length of the element list or -1 in case of an error.
*/
int ius3DTransducerElementListGetSize
(
    iu3dtel_t list        ///< The list with elements
);

/** \brief Returns the element from a transducer's element list
* \returns the 3D element \p index.
*/
iu3dte_t ius3DTransducerElementListGet
(
    iu3dtel_t list,    ///< The transducer's element list
    int index          ///< The index of the element in the list
);

/** \brief Sets the 3D element \p member in the transducer's \p list at index location \p index
* \returns #IUS_E_OK in case of success or #IUS_ERR_VALUE in case of invalid parameters.
*/
int ius3DTransducerElementListSet
(
    iu3dtel_t list,    ///< The list that will be updated
    iu3dte_t member,   ///< The #IusTransducerElement to add
    int index          ///< The location in he list
);

#endif //IUSLIBRARY_IUSHL3DTRANSDUCERELEMENTLIST_H
