// Created by nlv09165 on 15/01/2019.
#ifndef IUSLIBRARY_IUSHLDEMODULATIONDICT_H
#define IUSLIBRARY_IUSHLDEMODULATIONDICT_H

#include <iusHDF5.h>
#include <iusDemodulation.h>

// ADT
typedef struct IusDemodulationDict IusDemodulationDict;
/** A dictionary of #IusDemodulation. Typically, first an empty dictionary is created with #iusDemodulationDictCreate()
* and then #IusDemodulation are added with #iusDemodulationDictSet() or retreived with #iusDemodulationDictGet()
*/
typedef IusDemodulationDict *iudmd_t;
#define  IUDMD_INVALID (iudmd_t) NULL

/** \brief create an empty dictionary
* \return Returns the created dictionary or NULL in case of an error
*/
iudmd_t iusDemodulationDictCreate
(
	void
);

/** \brief Delete the dictionary without deleting its #IusDemodulation
* \return #IUS_E_OK in case of success and #IUS_ERR_VALUE in case of an invalid argument.
*/
int iusDemodulationDictDelete
(
	iudmd_t dict ///< The dictionary of interest
);

/** \brief Delete the dictionary and its #IusDemodulation
* \return #IUS_E_OK in case of success and #IUS_ERR_VALUE in case of an error.
*/
int iusDemodulationDictDeepDelete
(
	iudmd_t dict ///< The dictionary of interest
);

/** \brief Compare two dictionaries with each other
* \return #IUS_TRUE in case the dictionaries are equal and #IUS_FALSE otherwise.
*/
int iusDemodulationDictCompare
(
	iudmd_t reference, ///< The dictionary to compare to
	iudmd_t actual     ///< The dictionary to compare with
);

/** \brief Get the number of entries in the dictionary
* \return Returns the number of #IusDemodulation of the dictionary, or -1 in case of an error.
*/
int iusDemodulationDictGetSize
(
	iudmd_t dict       ///< The dictionary of interest
);

/** \brief Get the #IusDemodulation from the dictionary that has the label \p key
* \return Returns the requested #IusDemodulation or IURS_INVALID in case of an error
*/
iudm_t iusDemodulationDictGet
(
	iudmd_t dict,      ///< The dictionary of interest
	char * key         ///< The label of the demodulation to return
);

/** \brief Get the #IusDemodulation from the dictionary that has the label \p key
* \return Returns the requested #IusDemodulation or IURS_INVALID in case of an error
*/
int iusDemodulationDictSet
(
	iudmd_t dict,   ///< The dictionary of interest
	char * key,     ///< The label of the #IusDemodulation that to be added
	iudm_t member   ///< The #IusDemodulation object
);

#endif //IUSLIBRARY_IUSHLDEMODULATIONDICT_H
