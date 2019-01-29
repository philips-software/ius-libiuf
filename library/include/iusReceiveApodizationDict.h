// Created by nlv12901 on 28/01/2019.
#ifndef IUSLIBRARY_IUSRECEIVEAPODIZATIONDICT_H
#define IUSLIBRARY_IUSRECEIVEAPODIZATIONDICT_H

#include <stdio.h>
#include <iusReceiveApodization.h>

// ADT
typedef struct IusReceiveApodizationDict IusReceiveApodizationDict;
/** \brief A receive apodization is a weighting function of the amplitudes of the transducer elements. 
 * This is a dictionary of these functions.  
 */
typedef IusReceiveApodizationDict *iurad_t;
#define  IURAD_INVALID (iurad_t) NULL

/** \brief Creates a IusReceiveApodizationDict 
 * \return Returns an allocated empty IusReceiveApodizationDict
 */
iurad_t iusReceiveApodizationDictCreate
(
	void
);

/** \brief Deletes the #IusReceiveApodizationDict object without deleting each #IusReceiveApodization separately.
 * \return returns #IUS_TRUE if the dictionary could be deleted or #IUS_ERR_VALUE if the dictionary was invalid
 * */
int iusReceiveApodizationDictDelete
(
	iurad_t dict ///< The IusReceiveApodizationDict that is to be deleted
);

/** \brief Deletes a IusReceiveApodizationDict object and all its #IusReceiveApodization s
 * \return returns #IUS_TRUE if the dictionary could be deleted or #IUS_ERR_VALUE if the dictionary was invalid
 * */
int iusReceiveApodizationDictDeepDelete
(
	iurad_t dict ///< The IusReceiveApodizationDict that is to be deleted
);

/** \brief Compares if two #IusReceiveApodizationDict objects are equal
 *  \return returns #IUS_TRUE if the objects are equal and #IUS_FALSE if they differ. 
 *  The order of the arguments doesn't matter 
 */
int iusReceiveApodizationDictCompare
(
	iurad_t reference, ///< the #IusReceiveApodizationDict to compare to
	iurad_t actual     ///< the #IusReceiveApodizationDict to compare with
);

/** \brief Get the number of #IusReceiveApodization objects in the dictionary
 *  \return the number of #IusReceiveApodization objects. 
 */
size_t iusReceiveApodizationDictGetSize 
(
	iurad_t dict       ///< the \p IusReceiveApodizationDict of interest
);

/** \brief Get the #IusReceiveApodization with the label \p key from the \p dict
 *  \return requested #IusReceiveApodization or NULL in case of an error. 
 */
iura_t iusReceiveApodizationDictGet
(
	iurad_t dict, ///< the \p IusReceiveApodizationDict of interest
	char * key    ///< the label of the \p IusReceiveApodization  
);

/** \brief Add a #IusReceiveApodization with the label \p key to the \p dict
 * 
 *  \return #IUS_E_OK if the IusReceiveApodization could be added or #IUS_ERR_VALUE in case of an error.
 */
int iusReceiveApodizationDictSet
(
	iurad_t dict, ///< the \p IusReceiveApodizationDict that receives a new \p IusReceiveApodization
	char * key,   ///< the label of the #IusReceiveApodization
	iura_t member ///< the \p IusReceiveApodization that is added
);


#endif //IUSLIBRARY_IUSRECEIVEAPODIZATIONDICT_H

