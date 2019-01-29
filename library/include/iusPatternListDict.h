// Created by nlv09165 on 03/05/2018.
#ifndef IUSLIBRARY_IUSPATTERNLISTDICT_H
#define IUSLIBRARY_IUSPATTERNLISTDICT_H

#include <iusHDF5.h>
#include <iusPatternList.h>

// ADT
typedef struct IusPatternListDict IusPatternListDict;
/** A dictionary of #IusPatternList objects. A patternlist typically describes a \p bmode or \p doppler driving scheme 
 * and since these can be multiplexed in a single acquisition, suchs lists are put in a dictionary.
 * */
typedef IusPatternListDict *iupald_t;
#define  IUPALD_INVALID (iupald_t) NULL

/** \brief Create an empty dictionary for #IusPatternList objects
 * \return Returns the created dictionary or #IUPALD_INVALID in case of an error.
 */
iupald_t iusPatternListDictCreate
(
	void
);

/** \brief Delete the dictionary without deleting its #IusPatternList objects.
 * \return Returns #IUS_E_OK in case of success or #IUS_ERR_VALUE in case of an invalid argument.
 */
int iusPatternListDictDelete
(
	iupald_t dict  ///< The dictionary to be deleted
);

/** \brief Delete the dictionary and its #IusPatternList objects.
 * \return Returns #IUS_E_OK in case of success or #IUS_ERR_VALUE in case of an error.
 */
int iusPatternListDictDeepDelete
(
	iupald_t dict ///< The dictionary to be deleted
);

/** \brief Compare two dictionaries of #IusPatternList objects.
 * \return Returns #IUS_TRUE in case the dictionaries are equal and #IUS_FALSE otherwise.
 */
int iusPatternListDictCompare
(
	iupald_t reference, ///< The dictionary to compare to
	iupald_t actual     ///< The dictionary to compare with
);

/** \brief Get the number of #IusPatternList objects in the dictionary.
 * \return Returns the number of #IusPatternList objects or -1 in case of an invalid dictionary.
 */
size_t iusPatternListDictGetSize
(
	iupald_t dict ///< The dictionary of interest
);

/** \brief Get the #IusPatternList object with label \p key from the dictionary.
 * \return Returns #IusPatternList \p key or #IUPALD_INVALID if it is not found.
 */
iupal_t iusPatternListDictGet
(
	iupald_t dict, ///< The dictionary of interest
	char * key     ///< The label of the #IusPatternList
);

/** \brief Get the #IusPatternList object with label \p key from the dictionary.
 * \return Returns #IusPatternList \p key or #IUPALD_INVALID if it is not found.
 */
char **iusPatternListDictGetKeys
(
    iupald_t dict   ///< The dictionary of interest
);

/** \brief Add the #IusPatternList object with label \p key to the dictionary.
 * \return Returns #IUS_E_OK when successful  or #IUS_ERR_VALUE in case of an error.
 */
int iusPatternListDictSet
(
	iupald_t dict,  ///< The dictionary of interest
	char * key,     ///< The label of the #IusPatternList 
	iupal_t member  ///< The #IusPatternList
);

#endif //IUSLIBRARY_IUSHLPULSEDICT_H
