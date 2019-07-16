
#ifndef IUFLIBRARY_IUFHL3DTRANSDUCERELEMENTLIST_H
#define IUFLIBRARY_IUFHL3DTRANSDUCERELEMENTLIST_H

#include <iuf3DTransducerElement.h>

// ADT
typedef struct Iuf3DTransducerElementList Iuf3DTransducerElementList;
/** \brief A 3D transducer element list is a list object of 3D transducer elements. Elements are created first with #iuf3DTransducerElementCreate(),
* and then added to a list using #iuf3DTransducerSetElement(), after an empty list is created with #iuf3DTransducerElementListCreate().
*/
typedef Iuf3DTransducerElementList *iu3dtel_t;
#define  IU3DTEL_INVALID (iu3dtel_t) NULL

/** \brief Create an empty list of 3D elements with length \p num3DTransducerElements
* \return Returns the created list or #IU3DTEL_INVALID in case this failed.
*/
iu3dtel_t iuf3DTransducerElementListCreate
(
    int numElements ///< the number of elements that will go in the element list
);

/** \brief Delete an existing \p list of elements including all its elements individually.
* \returns #IUF_TRUE in case the list is deleted successfully or #IUF_ERR_VALUE when something went wrong.
*/
int iuf3DTransducerElementListDeepDelete
(
    iu3dtel_t list ///< the list that is to be deleted
);

/** \brief Delete an existing \p list of elements without deleting the individual elements
* \returns #IUF_TRUE in case the list is deleted successfully or #IUF_ERR_VALUE when something went wrong.
*/
int iuf3DTransducerElementListDelete
(
    iu3dtel_t list  ///< the list that is to be deleted
);

/** \brief Check if two lists are identical. the order of the parameters doesn't matter.
* \returns #IUF_TRUE in case the list content of the list is th same or #IUF_FALSE when they differ.
*/
int iuf3DTransducerElementListCompare
(
    iu3dtel_t reference,  ///< the reference list
    iu3dtel_t actual      ///< the list to be compared
);

/** \brief Returns the length of the element list
* \returns the length of the element list or -1 in case of an error.
*/
int iuf3DTransducerElementListGetSize
(
    iu3dtel_t list        ///< The list with elements
);

/** \brief Returns the element from a transducer's element list
* \returns the 3D element \p index.
*/
iu3dte_t iuf3DTransducerElementListGet
(
    iu3dtel_t list,    ///< The transducer's element list
    int index          ///< The index of the element in the list
);

/** \brief Sets the 3D element \p member in the transducer's \p list at index location \p index
* \returns #IUF_E_OK in case of success or #IUF_ERR_VALUE in case of invalid parameters.
*/
int iuf3DTransducerElementListSet
(
    iu3dtel_t list,    ///< The list that will be updated
    iu3dte_t member,   ///< The #IufTransducerElement to add
    int index          ///< The location in he list
);

#endif //IUFLIBRARY_IUFHL3DTRANSDUCERELEMENTLIST_H
