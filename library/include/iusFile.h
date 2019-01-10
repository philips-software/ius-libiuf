// Created by nlv09165 on 31/08/2018.
#include "iusHistoryNode.h"

#ifndef IUSLIBRARY_IUSFILE_H
#define IUSLIBRARY_IUSFILE_H

// ADT
typedef struct IusFile IusFile;
/** A iusFile contains the file handle and the history node */
typedef IusFile *iuf_t;
#define  IUFI_INVALID (iuf_t) NULL

/** \brief Create an #IusFile from the file \p pFilename 
 * \return Return the #IusFile or IUFI_INVALID if the file could not be loaded.
*/
iuf_t iusFileLoad
(
    char *pFilename     ///< The filename of the #IusFile
);

/** \brief Deletes an #IusFile object. 
 * returns #IUS_E_OK when successful and #IUS_ERR_VALUE otherwise
 */
int iusFileDelete
(
    iuf_t file          ///< The #IusFile to be deleted
);

/** \brief Compares two #IusFile objects with each other.
 * returns #IUS_TRUE when the two files are equal and #IUS_FALSE otherwise
 */
int iusFileCompare
(
    iuf_t reference,    ///< The #IusFile to compare to
    iuf_t actual        ///< The #IusFile to compare with
);

/** \brief Gets the history node of the #IusFile 
 * \return Returns the #IusHistoryNode when successful or NULL in case of an error 
 */
iuhn_t iusFileGetHistoryTree
(
    iuf_t file          ///< The #IusFile of interest
);

/** \brief Gets the type label of the #IusFile.
 * \return Returns the type label
 */
const char *iusFileGetType
(
    iuf_t file          ///< The #IusFile of interest
);

/** \brief Sets the #IusHistoryNode of the #IusFile.
 * \return Returns #IUS_E_OK when successful and #IUS_ERR_VALUE otherwise.
 */
int iusFileSetHistoryTree
(
    iuf_t file,         ///< the #IusFile of interest
    iuhn_t history      ///< the #IusHistoryNode that is set
);
#endif //IUSLIBRARY_IUSFILE_H
