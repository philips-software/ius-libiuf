// Created by nlv09165 on 15/08/2018.
#ifndef IUSLIBRARY_IUSCOMPOUNDWAVELIST_H
#define IUSLIBRARY_IUSCOMPOUNDWAVELIST_H

#include <iusCompoundWave.h>

// ADT
typedef struct IusCompoundWaveList IusCompoundWaveList;
/** A compound list contains a list of #IusCompoundWave objects. Typically a compoundWaveList is created with a fixed size with
* #iusCompoundWaveListCreate(). And then compoundWaves can be set at an index with #iusCompoundListSet().
*/
typedef IusCompoundWaveList *iucwl_t;
#define  IUCWL_INVALID (iucwl_t) NULL

/* \brief Creates a compoundWaveList of size \p numWaves
* \return Returns the created empty #IusCompoundWaveList of size \p numWaves or IUCWL_INVALID in case of an error.
*/
iucwl_t iusCompoundWaveListCreate
(
	int numWaves                      ///< The number of compoundWaves to allocate.
);

/* \brief Deletes a compoundWaveList without deleting the #IusCompoundWaves.
* \return Returns #IUS_E_OK when successful or #IUS_ERR_VALUE in case of an invalid argument.
*/
int iusCompoundWaveListDelete
(
	iucwl_t compoundWaveList          ///< The #IusCompoundWaveList that is to be deleted 
);

/* \brief Deletes a compoundList including its compounds.
* \return Returns #IUS_E_OK when successful or #IUS_ERR_VALUE in case of an error.
*/
int iusCompoundWaveListDeepDelete
(
	iucwl_t compoundWaveList          ///< The compoundWaveList that is to be deleted 
);

/* \brief Compares two #IusCompoundWaveLists with each other.
* \return Returns #IUS_TRUE when the lists are equal and #IUS_FALSE otherwise.
*/
int iusCompoundWaveListCompare
(
	iucwl_t reference,          ///< The compoundWaveList to compare to
	iucwl_t actual              ///< The compoundWaveList to compare with
);

/* \brief Gets the number of #IusCompound objects in the list.
* \return Returns the number of compounds or -1 in case of an error.
*/
int iusCompoundWaveListGetSize
(
	iucwl_t compoundWaveList         ///< The #IusCompoundWaveList of interest 
);

/* \brief Gets the #IusCompoundWave at \p index in the list.
* \return Returns the #IusCompoundWave or NULL in case of an error.
*/
iucw_t iusCompoundWaveListGet
(
	iucwl_t compoundWaveList,       ///< The compoundList of interest
	int index                       ///< The index in the list
);

/* \brief Sets the #IusCompoundWave at \p index in the list.
* \return Returns the #IUS_E_OK in case of success and #IUS_ERR_VALUE otherwise.
*/
int iusCompoundWaveListSet
(
	iucwl_t compoundWaveList,      ///< The compoundWaveList of interest
	iucw_t member,                 ///< The #IusCompoundWave to set
	int index                      ///< The index in the list
);

#endif //IUSLIBRARY_IUSCOMPOUNDWAVELIST_H
