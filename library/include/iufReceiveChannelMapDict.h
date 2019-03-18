// Created by nlv09165 on 03/05/2018.
#ifndef IUFLIBRARY_IUFHLRECEIVECHANNELMAPDICT_H
#define IUFLIBRARY_IUFHLRECEIVECHANNELMAPDICT_H

#include <stdio.h>
#include <iufReceiveChannelMap.h>

// ADT
typedef struct IufReceiveChannelMapDict IufReceiveChannelMapDict;
/** A dictionary of #IufReceiveChannelMap objects. Typically, create an empty dictionary with #iufReceiveChannelMapDictCreate()
 * and add #IufReceiveChannelMap objects with #iufReceiveChannelMapDictSet().
 */
typedef IufReceiveChannelMapDict *iurcmd_t;
#define  IURCMD_INVALID (iurcmd_t) NULL

/** \brief Create an empty dictionary 
 * \return returns the created dictionary or #IURCMD_INVALID in case of an error. 
*/
iurcmd_t iufReceiveChannelMapDictCreate
(
	void
);

/** \brief Delete a dictionary without deleting the #IufReceiveChannelMap objects
 * \return Returns #IUF_E_OK in case of success or #IUF_ERR_VALUE in case of an invalid argument.
 */
int iufReceiveChannelMapDictDelete
(
	iurcmd_t dict ///< the dictionary of interest
);

/** \brief Delete a dictionary and its #IufReceiveChannelMap objects
 * \return Returns #IUF_E_OK in case of success or #IUF_ERR_VALUE otherwise.
 */
int iufReceiveChannelMapDictDeepDelete
(
	iurcmd_t dict ///< the dictionary of interest
);

/** \brief Compare two #IufReceiveChannelMapDict ojects with each other
 * \return Return #IUF_TRUE in case the dictionaries are equal and #IUF_FALSE otherwise
 */
int iufReceiveChannelMapDictCompare
(
	iurcmd_t reference,  ///< the dictionary to compare to
	iurcmd_t actual      ///< the dictionary to compare with
);

/** \brief Gets the number of #IufReceiveChannelMap objects in the dictionary
 * \return Returns the number of #IufReceiveChannelMap objects
 */
size_t iufReceiveChannelMapDictGetSize
(
	iurcmd_t dict ///< the dictionary of interest
);

/** \brief Gets the #IufReceiveChannelMap with label \p key from the dictionary.
 * \return Returns the requested #IufReceiveChannelMap or #IURCMD_INVALID in case it could not be found
 */
iurcm_t iufReceiveChannelMapDictGet
(
	iurcmd_t dict,  ///< The dictionary of interest 
	char * key      ///< The label of the #IufReceiveChannelMap
);

/** \brief Get the keys currently available in the dict
 *  \return array (with size #iufTransmitApodizationDictGetSize) of keys.
 */
char **iufReceiveChannelMapDictGetKeys
(
    iurcmd_t dict   ///< The dictionary of interest
);


/** \brief Add a #IufReceiveChannelMap with label \p key to the dictionary.
 * \return Returns #IUF_E_OK in case of success or #IUF_ERR_VALUE in case of an invalid argument.
 */
int iufReceiveChannelMapDictSet
(
	iurcmd_t dict,  ///< The dictionary of interest
	char * key,     ///< The label of the #IufReceiveChannelMap that is added
	iurcm_t member  ///< The #IufReceiveChannelMap
);

#endif //IUFLIBRARY_IUFHLRECEIVECHANNELMAPDICT_H

