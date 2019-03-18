// Created by nlv09165 on 31/08/2018.
#include "iufHistoryNode.h"

#ifndef IUFLIBRARY_IUFFILE_H
#define IUFLIBRARY_IUFFILE_H

// ADT
typedef struct IufFile IufFile;
/** A iufFile contains the file handle and the history node */
typedef IufFile *iuf_t;
#define  IUFI_INVALID (iuf_t) NULL

/** \brief Create an #IufFile from the file \p pFilename
 * \return Return the #IufFile or IUFI_INVALID if the file could not be loaded.
*/
iuf_t iufFileLoad
(
    char *pFilename     ///< The filename of the #IufFile
);

/** \brief Deletes an #IufFile object.
 * returns #IUF_E_OK when successful and #IUF_ERR_VALUE otherwise
 */
int iufFileDelete
(
    iuf_t file          ///< The #IufFile to be deleted
);

/** \brief Compares two #IufFile objects with each other.
 * returns #IUF_TRUE when the two files are equal and #IUF_FALSE otherwise
 */
int iufFileCompare
(
    iuf_t reference,    ///< The #IufFile to compare to
    iuf_t actual        ///< The #IufFile to compare with
);

/** \brief Gets the history node of the #IufFile
 * \return Returns the #IufHistoryNode when successful or NULL in case of an error
 */
iuhn_t iufFileGetHistoryTree
(
    iuf_t file          ///< The #IufFile of interest
);

/** \brief Gets the type label of the #IufFile.
 * \return Returns the type label
 */
const char *iufFileGetType
(
    iuf_t file          ///< The #IufFile of interest
);

/** \brief Sets the #IufHistoryNode of the #IufFile.
 * \return Returns #IUF_E_OK when successful and #IUF_ERR_VALUE otherwise.
 */
int iufFileSetHistoryTree
(
    iuf_t file,         ///< the #IufFile of interest
    iuhn_t history      ///< the #IufHistoryNode that is set
);
#endif //IUFLIBRARY_IUFFILE_H
