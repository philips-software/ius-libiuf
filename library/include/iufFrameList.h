// Created by nlv09165 on 15/08/2018.
#ifndef IUFLIBRARY_IUFHLFRAMELIST_H
#define IUFLIBRARY_IUFHLFRAMELIST_H

#include <iufFrame.h>

// ADT
typedef struct IufFrameList IufFrameList;
/** A frame list contains a list of #IufFrame objects. Typically a frameList is created with a fixed size with
 * #iufFrameListCreate(). And then frames are set at an index with #iufFrameListSet().
 */ 
typedef IufFrameList *iufl_t;
#define  IUFL_INVALID (iufl_t) NULL

/* \brief Creates a frameList of size \p numFrames
 * \return Returns the created empty frameList of size \p numFrames or IUFL_INVALID in case of an error.
 */
iufl_t iufFrameListCreate
(
    int numFrames             ///< The number of frames to allocate.
);

/* \brief Deletes a frameList without deleting the frames.
 * \return Returns #IUF_E_OK when successful or #IUF_ERR_VALUE in case of an invalid argument.
 */
int iufFrameListDelete
(
    iufl_t frameList          ///< The frameList that is to be deleted 
);

/* \brief Deletes a frameList including its the frames.
 * \return Returns #IUF_E_OK when successful or #IUF_ERR_VALUE in case of an error.
 */
int iufFrameListDeepDelete
(
    iufl_t frameList          ///< The frameList that is to be deleted 
);

/* \brief Compares two frameLists with each other.
 * \return Returns #IUF_TRUE when the lists are equal and #IUF_FALSE otherwise.
 */
int iufFrameListCompare
(
    iufl_t reference,          ///< The frameList to compare to
    iufl_t actual              ///< The frameList to compare with
);

/* \brief Gets the number of #IufFrame objects in the list.
 * \return Returns the number of frames or -1 in case of an error.
 */
int iufFrameListGetSize
(
    iufl_t frameList           ///< The frameList of interest 
);

/* \brief Gets the #IufFrame at \p index in the list.
 * \return Returns the #IufFrame or NULL in case of an error.
 */
iufr_t iufFrameListGet
(
    iufl_t frameList,          ///< The frameList of interest
    int index                  ///< The index in the list
);

/* \brief Sets the #IufFrame at \p index in the list.
 * \return Returns the #IUF_E_OK in case of success and #IUF_ERR_VALUE otherwise.
 */
int iufFrameListSet
(
    iufl_t frameList,         ///< The frameList of interest
    iufr_t member,            ///< The #IufFrame to set
    int index                 ///< The index in the list
);

#endif //IUFLIBRARY_IUFHLFRAMELIST_H
