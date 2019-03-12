// Created by nlv09165 on 03/05/2018.
#ifndef IUSLIBRARY_IUSIQPATTERNLISTDICT_H
#define IUSLIBRARY_IUSIQPATTERNLISTDICT_H

#include <iusHDF5.h>
#include <iusIqPatternList.h>

// ADT
typedef struct IusIqPatternListDict IusIqPatternListDict;
/** A dictionary of #IusIqPatternList objects. A patternlist typically describes a \p bmode or \p doppler driving scheme 
 * and since these can be multiplexed in a single acquisition, suchs lists are put in a dictionary.
 * */
typedef IusIqPatternListDict *iuiqpald_t;
#define  IUIQPALD_INVALID (iuiqpald_t) NULL

/** \brief Create an empty dictionary for #IusPatternList objects
 * \return Returns the created dictionary or #IUIQPALD_INVALID in case of an error.
 */
iuiqpald_t iusIqPatternListDictCreate
(
	void
);

/** \brief Delete the dictionary without deleting its #IusPatternList objects.
 * \return Returns #IUS_E_OK in case of success or #IUS_ERR_VALUE in case of an invalid argument.
 */
int iusIqPatternListDictDelete
(
	iuiqpald_t dict  ///< The dictionary to be deleted
);

/** \brief Delete the dictionary and its #IusIqPatternList objects.
 * \return Returns #IUS_E_OK in case of success or #IUS_ERR_VALUE in case of an error.
 */
int iusIqPatternListDictDeepDelete
(
	iuiqpald_t dict ///< The dictionary to be deleted
);

/** \brief Return an array of char * containing the keys in the dictionary
 * \return Returns the keys as char * array
*/
char **iusIqPatternListDictGetKeys
(
	iuiqpald_t dict
);

/** \brief Compare two dictionaries of #IusIqPatternList objects.
 * \return Returns #IUS_TRUE in case the dictionaries are equal and #IUS_FALSE otherwise.
 */
int iusIqPatternListDictCompare
(
	iuiqpald_t reference, ///< The dictionary to compare to
	iuiqpald_t actual     ///< The dictionary to compare with
);

/** \brief Get the number of #IusPatternList objects in the dictionary.
 * \return Returns the number of #IusPatternList objects or -1 in case of an invalid dictionary.
 */
size_t iusIqPatternListDictGetSize
(
	iuiqpald_t dict ///< The dictionary of interest
);

/** \brief Get the #IusIqPatternList object with label \p key from the dictionary.
 * \return Returns #IusIqPatternList \p key or #IUIQPALD_INVALID if it is not found.
 */
iuiqpal_t iusIqPatternListDictGet
(
	iuiqpald_t dict, ///< The dictionary of interest
	char * key       ///< The label of the #IusIqPatternList
);

/** \brief Add the #IusIqPatternList object with label \p key to the dictionary.
 * \return Returns #IUS_E_OK when successful  or #IUS_ERR_VALUE in case of an error.
 */
int iusIqPatternListDictSet
(
	iuiqpald_t dict,  ///< The dictionary of interest
	char * key,     ///< The label of the #IusPatternList 
	iuiqpal_t member  ///< The #IusPatternList
);

#endif //IUSLIBRARY_IUSHLIQPATTERNLISTDICT_H
