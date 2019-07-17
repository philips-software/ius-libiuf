#ifndef IUFLIBRARY_IUFIQPATTERNLISTDICT_H
#define IUFLIBRARY_IUFIQPATTERNLISTDICT_H

#include <iufHDF5.h>
#include <iufIqPatternList.h>

// ADT
typedef struct IufIqPatternListDict IufIqPatternListDict;
/** A dictionary of #IufIqPatternList objects. A patternlist typically describes a \p bmode or \p doppler driving scheme
 * and since these can be multiplexed in a single acquisition, suchs lists are put in a dictionary.
 * */
typedef IufIqPatternListDict *iuiqpald_t;
#define  IUIQPALD_INVALID (iuiqpald_t) NULL

/** \brief Create an empty dictionary for #IufPatternList objects
 * \return Returns the created dictionary or #IUIQPALD_INVALID in case of an error.
 */
iuiqpald_t iufIqPatternListDictCreate
(
	void
);

/** \brief Delete the dictionary without deleting its #IufPatternList objects.
 * \return Returns #IUF_E_OK in case of success or #IUF_ERR_VALUE in case of an invalid argument.
 */
int iufIqPatternListDictDelete
(
	iuiqpald_t dict  ///< The dictionary to be deleted
);

/** \brief Delete the dictionary and its #IufIqPatternList objects.
 * \return Returns #IUF_E_OK in case of success or #IUF_ERR_VALUE in case of an error.
 */
int iufIqPatternListDictDeepDelete
(
	iuiqpald_t dict ///< The dictionary to be deleted
);

/** \brief Return an array of char * containing the keys in the dictionary
 * \return Returns the keys as char * array
*/
char **iufIqPatternListDictGetKeys
(
	iuiqpald_t dict
);

/** \brief Compare two dictionaries of #IufIqPatternList objects.
 * \return Returns #IUF_TRUE in case the dictionaries are equal and #IUF_FALSE otherwise.
 */
int iufIqPatternListDictCompare
(
	iuiqpald_t reference, ///< The dictionary to compare to
	iuiqpald_t actual     ///< The dictionary to compare with
);

/** \brief Get the number of #IufPatternList objects in the dictionary.
 * \return Returns the number of #IufPatternList objects or -1 in case of an invalid dictionary.
 */
size_t iufIqPatternListDictGetSize
(
	iuiqpald_t dict ///< The dictionary of interest
);

/** \brief Get the #IufIqPatternList object with label \p key from the dictionary.
 * \return Returns #IufIqPatternList \p key or #IUIQPALD_INVALID if it is not found.
 */
iuiqpal_t iufIqPatternListDictGet
(
	iuiqpald_t dict, ///< The dictionary of interest
	char * key       ///< The label of the #IufIqPatternList
);

/** \brief Add the #IufIqPatternList object with label \p key to the dictionary.
 * \return Returns #IUF_E_OK when successful  or #IUF_ERR_VALUE in case of an error.
 */
int iufIqPatternListDictSet
(
	iuiqpald_t dict,  ///< The dictionary of interest
	char * key,     ///< The label of the #IufPatternList
	iuiqpal_t member  ///< The #IufPatternList
);

#endif //IUFLIBRARY_IUFHLIQPATTERNLISTDICT_H
