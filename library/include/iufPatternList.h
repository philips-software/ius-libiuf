

// Created by nlv09165 on 18/07/2018.
#ifndef IUFLIBRARY_IUFHLPATTERNLIST_H
#define IUFLIBRARY_IUFHLPATTERNLIST_H

#include <iufPattern.h>
#include <iufReceiveSettingsDict.h>
#include <iufReceiveChannelMapDict.h>

// ADT
typedef struct IufPatternList IufPatternList;
/** The pattern list describes the transmit-receive structure of an acquisition. It contains an array of patterns 
 * where a pattern is a reference to a {pulse, source, ReceiveChannelMap, apodization, receiveSettings} tuple, 
 * defined at a certain timestamp.
 * Since we restrict ourselves to a rectangular memory alignment, not all combinations of arbitrary settings are 
 * possible, i.e. during an acquistion of an #IufPatternList sequence the numSamplesPerLine, numChannels and numPulses
 * need to remain constant over the frames. 
 * To guarantee that, the receiveSettings and receiveChannelMap (dictionaries) need to be known during the creation of 
 * a #IufPatternList.
 */
typedef IufPatternList *iupal_t;
#define  IUPAL_INVALID (iupal_t) NULL

/** \brief Create a patternList of size \p numPattern. Using the #IufReceiveSettingsDict and #IufReceiveChannelMapDict
 * to set the data dimensions 
 * \return Returns an empty list for \p numPattern patterns, or #IUPAL_INVALID in case of an error.
 */ 
iupal_t iufPatternListCreate
(
    int numPatterns,                   ///< The number of patterns that will be in the list
    iursd_t receiveSettingsDict,      ///< The receiveSettings dictionary  
    iurcmd_t receiveChannelMapDict    ///< The receiveChannelMap dictionary
);

/** \brief Delete the patternList without deleting the individual patterns.
 * \return Returns #IUF_E_OK when successful or #IUF_ERR_VALUE in case of an invalid list.
 */
int iufPatternListDelete
(
    iupal_t list          ///< The #IufPatternList of interest.
);

/** \brief Delete the patternList and the individual patterns.
 * \return Returns #IUF_E_OK when successful or #IUF_ERR_VALUE otherwise.
 */
int iufPatternListDeepDelete
(
    iupal_t list          ///< The #IufPatternList of interest.
);

/** \brief Compare two patternLists with each other.
 * \return Returns #IUF_TRUE when the patterns lists are equal and #IUF_FALSE otherwise.
 */
int iufPatternListCompare
(
    iupal_t reference,    ///< The #IufPatternList to compare to
    iupal_t actual        ///< The #IufPatternList to compare with
);

/** \brief Get the number of patterns in the list
 * \return Returns the \p numPattern or -1 in case of an invalid argument.  
 */
int iufPatternListGetSize
(
    iupal_t list          ///< The #IufPatternList
);

/** \brief Get the pattern at \p index from the #IufPatternList
 * \return Returns the #IufPattern in case of success or IUPAL_INVALID in case of an error.
 */
iupa_t iufPatternListGet
(
    iupal_t list,         ///< The #IufPatternList of interest
    int index             ///< the pattern index
);

/** \brief Set a pattern at \p index in the #IufPatternList
 * \return Returns #IUF_E_OK in case of success or #IUF_ERR_VALUE in case of an error.
 */
int iufPatternListSet
(
    iupal_t list,         ///< The #IufPatternList of interest
    iupa_t member,        ///< the #IufPattern
    int index             ///< THe loction of the #IufPattern
);

#endif //IUFLIBRARY_IUFHLPATTERNLIST_H
