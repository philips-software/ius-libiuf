// Created by nlv12901 on 20/07/2018.
#ifndef IUSLIBRARY_IUSHLTRANSMITAPODIZATIONDICT_H
#define IUSLIBRARY_IUSHLTRANSMITAPODIZATIONDICT_H

#include <stdio.h>
#include <iusTransmitApodization.h>

// ADT
typedef struct IusTransmitApodizationDict IusTransmitApodizationDict;
/** \brief A transmit apodization is a weighting function of the amplitudes of the transducer elements. 
 * This is a dictionary of these functions.  
 */
typedef IusTransmitApodizationDict *iutad_t;
#define  IUTAD_INVALID (iutad_t) NULL

/** \brief Creates a IusTransmitApodizationDict 
 * \return Returns an allocated empty IusTransmitApodizationDict
 */
iutad_t iusTransmitApodizationDictCreate
(
	void
);

/** \brief Deletes the #IusTransmitApodizationDict object without deleting each #IusTransmitApodization separately.
 * \return returns #IUS_TRUE if the dictionary could be deleted or #IUS_ERR_VALUE if the dictionary was invalid
 * */
int iusTransmitApodizationDictDelete
(
	iutad_t dict ///< The IusTransmitApodizationDict that is to be deleted
);

/** \brief Deletes a IusTransmitApodizationDict object and all its #IusTransmitApodization s
 * \return returns #IUS_TRUE if the dictionary could be deleted or #IUS_ERR_VALUE if the dictionary was invalid
 * */
int iusTransmitApodizationDictDeepDelete
(
	iutad_t dict ///< The IusTransmitApodizationDict that is to be deleted
);

/** \brief Compares if two #IusTransmitApodizationDict objects are equal
 *  \return returns #IUS_TRUE if the objects are equal and #IUS_FALSE if they differ. 
 *  The order of the arguments doesn't matter 
 */
int iusTransmitApodizationDictCompare
(
	iutad_t reference, ///< the #IusTransmitApodizationDict to compare to
	iutad_t actual     ///< the #IusTransmitApodizationDict to compare with
);

/** \brief Get the number of #IusTransmitApodization objects in the dictionary
 *  \return the number of #IusTransmitApodization objects. 
 */
size_t iusTransmitApodizationDictGetSize 
(
	iutad_t dict       ///< the \p IusTransmitApodizationDict of interest
);

/** \brief Get the #IusTransmitApodization with the label \p key from the \p dict
 *  \return requested #IusTransmitApodization or NULL in case of an error. 
 */
iuta_t iusTransmitApodizationDictGet
(
	iutad_t dict, ///< the \p IusTransmitApodizationDict of interest
	char * key    ///< the label of the \p IusTransmitApodization  
);

/** \brief Add a #IusTransmitApodization with the label \p key to the \p dict
 * 
 *  \return #IUS_E_OK if the IusTransmitApodization could be added or #IUS_ERR_VALUE in case of an error.
 */
int iusTransmitApodizationDictSet
(
	iutad_t dict, ///< the \p IusTransmitApodizationDict that receives a new \p IusTransmitApodization
	char * key,   ///< the label of the #IusTransmitApodization
	iuta_t member ///< the \p IusTransmitApodization that is added
);


#endif //IUSLIBRARY_IUSHLTRANSMITAPODIZATIONDICT_H

