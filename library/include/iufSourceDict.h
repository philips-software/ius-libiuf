#ifndef IUFLIBRARY_IUFHLSOURCEDICT_H
#define IUFLIBRARY_IUFHLSOURCEDICT_H

#include <iufHDF5.h>
#include <iufSource.h>

// ADT
typedef struct IufSourceDict IufSourceDict;
/** A dictionary of sources. See the documentation of #IufSource for further details. The dictionary allows managing
 * multiple of them giving each a label. Typically one first creates an empty dictionary (#iufSourceDictCreate()) and
 * adds sources using #iufSourceDictSet() and retreives them with #iufSourceDictGet().
 */
typedef IufSourceDict *iusd_t;
#define  IUFD_INVALID (iusd_t) NULL

/** \brief Create an empty dictionary
 * \return Returns the created dictionary or NULL in case of an error.
 */
iusd_t iufSourceDictCreate
(
    void
);

/** \brief Deletes a source dictionary without deleting all the sources
 * \return Returns #IUF_E_OK in case of success and #IUF_ERR_VALUE in case of an invalid argument.
 */
int iufSourceDictDelete
(
    iusd_t dict ///< The dictionary of interest
);

/** \brief Deletes a source dictionary and all its sources
 * \return Returns #IUF_E_OK in case of success and #IUF_ERR_VALUE in case of an error.
 */
int iufSourceDictDeepDelete
(
    iusd_t dict ///< The dictionary of interest
);

/** \brief Compare two dictionaries with each other (order of arguments doesn't matter)
 * \return Returns #IUF_TRUE when the dictionaries are equal and #IUF_FALSE otherwise.
 */
int iufSourceDictCompare
(
    iusd_t reference,  ///< The source dictionary to compare to
    iusd_t actual      ///< The source dictionary to compare with
);

/** \brief Get the number of sources in the dictionary
 *  \return returns the number of sources or -1 in case of an error
 */ 
int iufSourceDictGetSize
(
    iusd_t dict        ///< The dictionary of interest
);

/** \brief Get a sources from the dictionary
 *  \return returns the source that has label \p key or #IUF_INVALID if it could not be found
 */ 
ius_t iufSourceDictGet
(
    iusd_t dict,       ///< The dictionary of interest
    char * key         ///< The label of the source
);

/** \brief Get the keys currently available in the dict
 *  \return array (with size #iufTransmitApodizationDictGetSize) of keys.
 */
char **iufSourceDictGetKeys
(
    iusd_t dict       ///< The dictionary of interest
);

/** \brief Add a sources to the dictionary
 *  \return Returns #IUF_E_OK in case of success and #IUF_ERR_VALUE in case of invalid arguments
 */ 
int iufSourceDictSet
(
    iusd_t dict,        ///< THe dictionary of interest
    char * key,         ///< The label of the source that is added
    ius_t member        ///< The source that is added
);

#endif //IUFLIBRARY_IUFHLSOURCEDICT_H
