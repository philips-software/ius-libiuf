// Created by nlv09165 on 15/08/2018.
#ifndef IUSLIBRARY_IUSHLFRAMELIST_H
#define IUSLIBRARY_IUSHLFRAMELIST_H

#include <iusFrame.h>

// ADT
typedef struct IusFrameList IusFrameList;
/** A frame list contains a list of #IusFrame objects. Typically a frameList is created with a fixed size with 
 * #iusFrameListCreate(). And then frames are set at an index with #iusFrameListSet().
 */ 
typedef IusFrameList *iufl_t;
#define  IUFL_INVALID (iufl_t) NULL

/* \brief Creates a frameList of size \p numFrames
 * \return Returns the created empty frameList of size \p numFrames or IUFL_INVALID in case of an error.
 */
iufl_t iusFrameListCreate
(
    int numFrames             ///< The number of frames to allocate.
);

/* \brief Deletes a frameList without deleting the frames.
 * \return Returns #IUS_E_OK when successful or #IUS_ERR_VALUE in case of an invalid argument.
 */
int iusFrameListDelete
(
    iufl_t frameList          ///< The frameList that is to be deleted 
);

/* \brief Deletes a frameList including its the frames.
 * \return Returns #IUS_E_OK when successful or #IUS_ERR_VALUE in case of an error.
 */
int iusFrameListDeepDelete
(
    iufl_t frameList          ///< The frameList that is to be deleted 
);

/* \brief Compares two frameLists with each other.
 * \return Returns #IUS_TRUE when the lists are equal and #IUS_FALSE otherwise.
 */
int iusFrameListCompare
(
    iufl_t reference,          ///< The frameList to compare to
    iufl_t actual              ///< The frameList to compare with
);

/* \brief Gets the number of #IusFrame objects in the list.
 * \return Returns the number of frames or -1 in case of an error.
 */
int iusFrameListGetSize
(
    iufl_t frameList           ///< The frameList of interest 
);

/* \brief Gets the #IusFrame at \p index in the list.
 * \return Returns the #IusFrame or NULL in case of an error.
 */
iufr_t iusFrameListGet
(
    iufl_t frameList,          ///< The frameList of interest
    int index                  ///< The index in the list
);

/* \brief Sets the #IusFrame at \p index in the list.
 * \return Returns the #IUS_E_OK in case of success and #IUS_ERR_VALUE otherwise.
 */
int iusFrameListSet
(
    iufl_t frameList,         ///< The frameList of interest
    iufr_t member,            ///< The #IusFrame to set
    int index                 ///< The index in the list
);

#endif //IUSLIBRARY_IUSHLFRAMELIST_H
