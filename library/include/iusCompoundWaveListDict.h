// Created by nlv09165 on 03/05/2018.
#ifndef IUSLIBRARY_IUSCOMPOUNDWAVELISTDICT_H
#define IUSLIBRARY_IUSCOMPOUNDWAVELISTDICT_H

#include <iusHDF5.h>
#include <iusCompoundWaveList.h>

// ADT
typedef struct IusCompoundWaveListDict IusCompoundWaveListDict;
/** A dictionary of #IusCompoundWaveList objects. A CompoundWaveList typically describes the input ultrasound pattern
* and since this pattern can differ per frame a suchs lists are put in a dictionary.
* */
typedef IusCompoundWaveListDict *iucwld_t;
#define  IUCWLD_INVALID (iucwld_t) NULL

/** \brief Create an empty dictionary for #IusCompoundWaveList objects
* \return Returns the created dictionary or #IUCWLD_INVALID in case of an error.
*/
iucwld_t iusCompoundWaveListDictCreate
(
	void
);

/** \brief Delete the dictionary without deleting its #IusCompoundWaveList objects.
* \return Returns #IUS_E_OK in case of success or #IUS_ERR_VALUE in case of an invalid argument.
*/
int iusCompoundWaveListDictDelete
(
	iucwld_t dict  ///< The dictionary to be deleted
);

/** \brief Delete the dictionary and its #IusIqPatternList objects.
* \return Returns #IUS_E_OK in case of success or #IUS_ERR_VALUE in case of an error.
*/
int iusCompoundWaveListDictDeepDelete
(
	iucwld_t dict ///< The dictionary to be deleted
);

/** \brief Compare two dictionaries of #IusCompoundWaveList objects.
* \return Returns #IUS_TRUE in case the dictionaries are equal and #IUS_FALSE otherwise.
*/
int iusCompoundWaveListDictCompare
(
	iucwld_t reference, ///< The dictionary to compare to
	iucwld_t actual     ///< The dictionary to compare with
);

/** \brief Get the number of #IusCompoundWaveList objects in the dictionary.
* \return Returns the number of #IusCompoundWaveList objects or -1 in case of an invalid dictionary.
*/
int iusIusCompoundWaveListDictGetSize
(
	iucwld_t dict ///< The dictionary of interest
);

/** \brief Get the #IusIqPatternList object with label \p key from the dictionary.
* \return Returns #IusIqPatternList \p key or #IUIQPALD_INVALID if it is not found.
*/
iucwl_t iusCompoundWaveListDictGet
(
	iucwld_t dict, ///< The dictionary of interest
	char * key     ///< The label of the #IusCompoundWaveList
);

/** \brief Add the #IusCompoundWaveList object with label \p key to the dictionary.
* \return Returns #IUS_E_OK when successful  or #IUS_ERR_VALUE in case of an error.
*/
int iusCompoundWaveListDictSet
(
	iucwld_t dict,  ///< The dictionary of interest
	char * key,     ///< The label of the #IusCompoundWaveList 
	iucwl_t member  ///< The #IusCompoundWaveList
);

#endif //IUSLIBRARY_IUSCOMPOUNDWAVELISTDICT_H
