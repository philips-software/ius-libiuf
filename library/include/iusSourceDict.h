// Created by nlv09165 on 31/07/2018.
#ifndef IUSLIBRARY_IUSHLSOURCEDICT_H
#define IUSLIBRARY_IUSHLSOURCEDICT_H

#include <iusHDF5.h>
#include <iusSource.h>

// ADT
typedef struct IusSourceDict IusSourceDict;
/** A dictionary of sources. See the documentation of #IusSource for further details. The dictionary allows managing 
 * multiple of them giving each a label. Typically one first creates an empty dictionary (#iusSourceDictCreate()) and
 * adds sources using #iusSourceDictSet() and retreives them with #iusSourceDictGet().   
 */
typedef IusSourceDict *iusd_t;
#define  IUSD_INVALID (iusd_t) NULL

/** \brief Create an empty dictionary
 * \return Returns the created dictionary or NULL in case of an error.
 */
iusd_t iusSourceDictCreate
(
    void
);

/** \brief Deletes a source dictionary without deleting all the sources
 * \return Returns #IUS_E_OK in case of success and #IUS_ERR_VALUE in case of an invalid argument.
 */
int iusSourceDictDelete
(
    iusd_t dict ///< The dictionary of interest
);

/** \brief Deletes a source dictionary and all its sources
 * \return Returns #IUS_E_OK in case of success and #IUS_ERR_VALUE in case of an error.
 */
int iusSourceDictDeepDelete
(
    iusd_t dict ///< The dictionary of interest
);

/** \brief Compare two dictionaries with each other (order of arguments doesn't matter)
 * \return Returns #IUS_TRUE when the dictionaries are equal and #IUS_FALSE otherwise.
 */
int iusSourceDictCompare
(
    iusd_t reference,  ///< The source dictionary to compare to
    iusd_t actual      ///< The source dictionary to compare with
);

/** \brief Get the number of sources in the dictionary
 *  \return returns the number of sources or -1 in case of an error
 */ 
int iusSourceDictGetSize
(
    iusd_t dict        ///< The dictionary of interest
);

/** \brief Get a sources from the dictionary
 *  \return returns the source that has label \p key or #IUS_INVALID if it could not be found
 */ 
ius_t iusSourceDictGet
(
    iusd_t dict,       ///< The dictionary of interest
    char * key         ///< The label of the source
);

/** \brief Get the keys currently available in the dict
 *  \return array (with size #iusTransmitApodizationDictGetSize) of keys.
 */
char **iusSourceDictGetKeys
(
    iusd_t dict       ///< The dictionary of interest
);

/** \brief Add a sources to the dictionary
 *  \return Returns #IUS_E_OK in case of success and #IUS_ERR_VALUE in case of invalid arguments
 */ 
int iusSourceDictSet
(
    iusd_t dict,        ///< THe dictionary of interest
    char * key,         ///< The label of the source that is added
    ius_t member        ///< The source that is added
);

#endif //IUSLIBRARY_IUSHLSOURCEDICT_H
