#ifndef IUFLIBRARY_IUFHLTRANSMITAPODIZATIONDICT_H
#define IUFLIBRARY_IUFHLTRANSMITAPODIZATIONDICT_H

#include <stdio.h>
#include <iufTransmitApodization.h>

// ADT
typedef struct IufTransmitApodizationDict IufTransmitApodizationDict;
/** \brief A transmit apodization is a weighting function of the amplitudes of the transducer elements. 
 * This is a dictionary of these functions.  
 */
typedef IufTransmitApodizationDict *iutad_t;
#define  IUTAD_INVALID (iutad_t) NULL

/** \brief Creates a IufTransmitApodizationDict
 * \return Returns an allocated empty IufTransmitApodizationDict
 */
iutad_t iufTransmitApodizationDictCreate
(
	void
);

/** \brief Deletes the #IufTransmitApodizationDict object without deleting each #IufTransmitApodization separately.
 * \return returns #IUF_TRUE if the dictionary could be deleted or #IUF_ERR_VALUE if the dictionary was invalid
 * */
int iufTransmitApodizationDictDelete
(
	iutad_t dict ///< The IufTransmitApodizationDict that is to be deleted
);

/** \brief Deletes a IufTransmitApodizationDict object and all its #IufTransmitApodization s
 * \return returns #IUF_TRUE if the dictionary could be deleted or #IUF_ERR_VALUE if the dictionary was invalid
 * */
int iufTransmitApodizationDictDeepDelete
(
	iutad_t dict ///< The IufTransmitApodizationDict that is to be deleted
);

/** \brief Compares if two #IufTransmitApodizationDict objects are equal
 *  \return returns #IUF_TRUE if the objects are equal and #IUF_FALSE if they differ.
 *  The order of the arguments doesn't matter 
 */
int iufTransmitApodizationDictCompare
(
	iutad_t reference, ///< the #IufTransmitApodizationDict to compare to
	iutad_t actual     ///< the #IufTransmitApodizationDict to compare with
);

/** \brief Get the number of #IufTransmitApodization objects in the dictionary
 *  \return the number of #IufTransmitApodization objects.
 */
size_t iufTransmitApodizationDictGetSize
(
	iutad_t dict       ///< the \p IufTransmitApodizationDict of interest
);

/** \brief Get the #IufTransmitApodization with the label \p key from the \p dict
 *  \return requested #IufTransmitApodization or NULL in case of an error.
 */
iuta_t iufTransmitApodizationDictGet
(
	iutad_t dict, ///< the \p IufTransmitApodizationDict of interest
	char * key    ///< the label of the \p IufTransmitApodization
);

/** \brief Get the keys currently available in the dict
 *  \return array (with size #iufTransmitApodizationDictGetSize) of keys.
 */
char **iufTransmitApodizationDictGetKeys
(
        iutad_t dict ///< the \p IufTransmitApodizationDict of interest
);

/** \brief Add a #IufTransmitApodization with the label \p key to the \p dict
 * 
 *  \return #IUF_E_OK if the IufTransmitApodization could be added or #IUF_ERR_VALUE in case of an error.
 */
int iufTransmitApodizationDictSet
(
	iutad_t dict, ///< the \p IufTransmitApodizationDict that receives a new \p IufTransmitApodization
	char * key,   ///< the label of the #IufTransmitApodization
	iuta_t member ///< the \p IufTransmitApodization that is added
);


#endif //IUFLIBRARY_IUFHLTRANSMITAPODIZATIONDICT_H

