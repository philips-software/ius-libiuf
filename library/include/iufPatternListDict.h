// Created by nlv09165 on 03/05/2018.
#ifndef IUFLIBRARY_IUFPATTERNLISTDICT_H
#define IUFLIBRARY_IUFPATTERNLISTDICT_H

#include <iufHDF5.h>
#include <iufPatternList.h>

// ADT
typedef struct IufPatternListDict IufPatternListDict;
/** A dictionary of #IufPatternList objects. A patternlist typically describes a \p bmode or \p doppler driving scheme
 * and since these can be multiplexed in a single acquisition, suchs lists are put in a dictionary.
 * */
typedef IufPatternListDict *iupald_t;
#define  IUPALD_INVALID (iupald_t) NULL

/** \brief Create an empty dictionary for #IufPatternList objects
 * \return Returns the created dictionary or #IUPALD_INVALID in case of an error.
 */
iupald_t iufPatternListDictCreate
(
	void
);

/** \brief Delete the dictionary without deleting its #IufPatternList objects.
 * \return Returns #IUF_E_OK in case of success or #IUF_ERR_VALUE in case of an invalid argument.
 */
int iufPatternListDictDelete
(
	iupald_t dict  ///< The dictionary to be deleted
);

/** \brief Delete the dictionary and its #IufPatternList objects.
 * \return Returns #IUF_E_OK in case of success or #IUF_ERR_VALUE in case of an error.
 */
int iufPatternListDictDeepDelete
(
	iupald_t dict ///< The dictionary to be deleted
);

/** \brief Compare two dictionaries of #IufPatternList objects.
 * \return Returns #IUF_TRUE in case the dictionaries are equal and #IUF_FALSE otherwise.
 */
int iufPatternListDictCompare
(
	iupald_t reference, ///< The dictionary to compare to
	iupald_t actual     ///< The dictionary to compare with
);

/** \brief Get the number of #IufPatternList objects in the dictionary.
 * \return Returns the number of #IufPatternList objects or -1 in case of an invalid dictionary.
 */
size_t iufPatternListDictGetSize
(
	iupald_t dict ///< The dictionary of interest
);

/** \brief Get the #IufPatternList object with label \p key from the dictionary.
 * \return Returns #IufPatternList \p key or #IUPALD_INVALID if it is not found.
 */
iupal_t iufPatternListDictGet
(
	iupald_t dict, ///< The dictionary of interest
	char * key     ///< The label of the #IufPatternList
);

/** \brief Get the #IufPatternList object with label \p key from the dictionary.
 * \return Returns #IufPatternList \p key or #IUPALD_INVALID if it is not found.
 */
char **iufPatternListDictGetKeys
(
    iupald_t dict   ///< The dictionary of interest
);

/** \brief Add the #IufPatternList object with label \p key to the dictionary.
 * \return Returns #IUF_E_OK when successful  or #IUF_ERR_VALUE in case of an error.
 */
int iufPatternListDictSet
(
	iupald_t dict,  ///< The dictionary of interest
	char * key,     ///< The label of the #IufPatternList
	iupal_t member  ///< The #IufPatternList
);

/** \brief Remove a #IufPatternList from the dictionary that has the label \p key
 * \return Returns IUF_ERR_VALUE in case the key was noet found or IUF_E_OK when successful
*/
int iufPatternListDictRemove
(
    iupald_t dict,   ///< The dictionary of interest
    char * key       ///< The label of the #IufPatternList that is to be removed
);
#endif //IUFLIBRARY_IUFHLPULSEDICT_H
