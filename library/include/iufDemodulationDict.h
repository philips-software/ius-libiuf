// Created by nlv09165 on 15/01/2019.
#ifndef IUFLIBRARY_IUFHLDEMODULATIONDICT_H
#define IUFLIBRARY_IUFHLDEMODULATIONDICT_H

#include <iufHDF5.h>
#include <iufDemodulation.h>

// ADT
typedef struct IufDemodulationDict IufDemodulationDict;
/** A dictionary of #IufDemodulation. Typically, first an empty dictionary is created with #iufDemodulationDictCreate()
* and then #IufDemodulation are added with #iufDemodulationDictSet() or retreived with #iufDemodulationDictGet()
*/
typedef IufDemodulationDict *iudmd_t;
#define  IUDMD_INVALID (iudmd_t) NULL

/** \brief create an empty dictionary
* \return Returns the created dictionary or NULL in case of an error
*/
iudmd_t iufDemodulationDictCreate
(
	void
);

/** \brief Delete the dictionary without deleting its #IufDemodulation
* \return #IUF_E_OK in case of success and #IUF_ERR_VALUE in case of an invalid argument.
*/
int iufDemodulationDictDelete
(
	iudmd_t dict ///< The dictionary of interest
);

/** \brief Delete the dictionary and its #IufDemodulation
* \return #IUF_E_OK in case of success and #IUF_ERR_VALUE in case of an error.
*/
int iufDemodulationDictDeepDelete
(
	iudmd_t dict ///< The dictionary of interest
);

/** \brief Compare two dictionaries with each other
* \return #IUF_TRUE in case the dictionaries are equal and #IUF_FALSE otherwise.
*/
int iufDemodulationDictCompare
(
	iudmd_t reference, ///< The dictionary to compare to
	iudmd_t actual     ///< The dictionary to compare with
);

/** \brief Get the number of entries in the dictionary
* \return Returns the number of #IufDemodulation of the dictionary, or -1 in case of an error.
*/
size_t iufDemodulationDictGetSize
(
	iudmd_t dict       ///< The dictionary of interest
);

/** \brief Get the #IufDemodulation from the dictionary that has the label \p key
* \return Returns the requested #IufDemodulation or IURS_INVALID in case of an error
*/
iudm_t iufDemodulationDictGet
(
	iudmd_t dict,      ///< The dictionary of interest
	char * key         ///< The label of the demodulation to return
);

/** \brief Get the #IufPatternList object with label \p key from the dictionary.
 * \return Returns #IufPatternList \p key or #IUPALD_INVALID if it is not found.
 */
char **iufDemodulationDictGetKeys
(
    iudmd_t dict       ///< The dictionary of interest
);

/** \brief Get the #IufDemodulation from the dictionary that has the label \p key
* \return Returns the requested #IufDemodulation or IURS_INVALID in case of an error
*/
int iufDemodulationDictSet
(
	iudmd_t dict,   ///< The dictionary of interest
	char * key,     ///< The label of the #IufDemodulation that to be added
	iudm_t member   ///< The #IufDemodulation object
);

#endif //IUFLIBRARY_IUFHLDEMODULATIONDICT_H
