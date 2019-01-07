// Created by nlv09165 on 03/05/2018.
#ifndef IUSLIBRARY_IUSHLRECEIVECHANNELMAPDICT_H
#define IUSLIBRARY_IUSHLRECEIVECHANNELMAPDICT_H

#include <stdio.h>
#include <iusReceiveChannelMap.h>

// ADT
typedef struct IusReceiveChannelMapDict IusReceiveChannelMapDict;
/** A dictionary of #IusReceiveChannelMap objects. Typically, create an empty dictionary with #iusReceiveChannelMapDictCreate()
 * and add #IusReceiveChannelMap objects with #iusReceiveChannelMapDictSet(). 
 */
typedef IusReceiveChannelMapDict *iurcmd_t;
#define  IURCMD_INVALID (iurcmd_t) NULL

/** \brief Create an empty dictionary 
 * \return returns the created dictionary or #IURCMD_INVALID in case of an error. 
*/
iurcmd_t iusReceiveChannelMapDictCreate
(
	void
);

/** \brief Delete a dictionary without deleting the #IusReceiveChannelMap objects 
 * \return Returns #IUS_E_OK in case of success or #IUS_ERR_VALUE in case of an invalid argument.
 */
int iusReceiveChannelMapDictDelete
(
	iurcmd_t dict ///< the dictionary of interest
);

/** \brief Delete a dictionary and its #IusReceiveChannelMap objects 
 * \return Returns #IUS_E_OK in case of success or #IUS_ERR_VALUE otherwise.
 */
int iusReceiveChannelMapDictDeepDelete
(
	iurcmd_t dict ///< the dictionary of interest
);

/** \brief Compare two #IusReceiveChannelMapDict ojects with each other
 * \return Return #IUS_TRUE in case the dictionaries are equal and #IUS_FALSE otherwise
 */
int iusReceiveChannelMapDictCompare
(
	iurcmd_t reference,  ///< the dictionary to compare to
	iurcmd_t actual      ///< the dictionary to compare with
);

/** \brief Gets the number of #IusReceiveChannelMap objects in the dictionary
 * \return Returns the number of #IusReceiveChannelMap objects
 */
size_t iusReceiveChannelMapDictGetSize
(
	iurcmd_t dict ///< the dictionary of interest
);

/** \brief Gets the #IusReceiveChannelMap with label \p key from the dictionary. 
 * \return Returns the requested #IusReceiveChannelMap or #IURCMD_INVALID in case it could not be found
 */
iurcm_t iusReceiveChannelMapDictGet
(
	iurcmd_t dict,  ///< The dictionary of interest 
	char * key      ///< The label of the #IusReceiveChannelMap
);

/** \brief Add a #IusReceiveChannelMap with label \p key to the dictionary. 
 * \return Returns #IUS_E_OK in case of success or #IUS_ERR_VALUE in case of an invalid argument.
 */
int iusReceiveChannelMapDictSet
(
	iurcmd_t dict,  ///< The dictionary of interest
	char * key,     ///< The label of the #IusReceiveChannelMap that is added 
	iurcm_t member  ///< The #IusReceiveChannelMap 
);

#endif //IUSLIBRARY_IUSHLRECEIVECHANNELMAPDICT_H

