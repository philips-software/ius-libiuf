// Created by nlv09165 on 15/08/2018.
#ifndef IUSLIBRARY_IUSCOMPOUNDFRAMELIST_H
#define IUSLIBRARY_IUSCOMPOUNDFRAMELIST_H

#include <iusCompoundFrame.h>

// ADT
typedef struct IusCompoundFrameList IusCompoundFrameList;
/** A frame list contains a list of #IusCompoundFrame objects. Typically a compoundFrameList is created with a fixed size with
* #iusCompoundFrameListCreate(). And then frames are set at an index with #iusCompoundFrameListSet().
*/
typedef IusCompoundFrameList *iucfrl_t;
#define  IUCFRL_INVALID (iucfrl_t) NULL

/* \brief Creates a compoundFrameList of size \p numFrames
* \return Returns the created empty compoundFrameList of size \p numFrames or IUCFRL_INVALID in case of an error.
*/
iucfrl_t iusCompoundFrameListCreate
(
	int numFrames             ///< The number of frames to allocate.
);

/* \brief Deletes a compoundFrameList without deleting the frames.
* \return Returns #IUS_E_OK when successful or #IUS_ERR_VALUE in case of an invalid argument.
*/
int iusCompoundFrameListDelete
(
	iucfrl_t compoundFrameList          ///< The compoundFrameList that is to be deleted 
);

/* \brief Deletes a compoundFrameList including its the frames.
* \return Returns #IUS_E_OK when successful or #IUS_ERR_VALUE in case of an error.
*/
int iusCompoundFrameListDeepDelete
(
	iucfrl_t compoundFrameList          ///< The compoundFrameList that is to be deleted 
);

/* \brief Compares two frameLists with each other.
* \return Returns #IUS_TRUE when the lists are equal and #IUS_FALSE otherwise.
*/
int iusCompoundFrameListCompare
(
	iucfrl_t reference,          ///< The compoundFrameList to compare to
	iucfrl_t actual              ///< The compoundFrameList to compare with
);

/* \brief Gets the number of #IusCompoundFrame objects in the list.
* \return Returns the number of frames or -1 in case of an error.
*/
int iusCompoundFrameListGetSize
(
	iucfrl_t compoundFrameList           ///< The compoundFrameList of interest 
);

/* \brief Gets the #IusCompoundFrame at \p index in the list.
* \return Returns the #IusCompoundFrame or NULL in case of an error.
*/
iucfr_t iusCompoundFrameListGet
(
	iucfrl_t compoundFrameList,          ///< The compoundFrameList of interest
	int index                  ///< The index in the list
);

/* \brief Sets the #IusCompoundFrame at \p index in the list.
* \return Returns the #IUS_E_OK in case of success and #IUS_ERR_VALUE otherwise.
*/
int iusCompoundFrameListSet
(
	iucfrl_t compoundFrameList,         ///< The compoundFrameList of interest
	iucfr_t member,            ///< The #IusCompoundFrame to set
	int index                 ///< The index in the list
);

#endif //IUSLIBRARY_IUSHLFRAMELIST_H
