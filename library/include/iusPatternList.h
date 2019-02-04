

// Created by nlv09165 on 18/07/2018.
#ifndef IUSLIBRARY_IUSHLPATTERNLIST_H
#define IUSLIBRARY_IUSHLPATTERNLIST_H

#include <iusPattern.h>
#include <iusReceiveSettingsDict.h>
#include <iusReceiveChannelMapDict.h>

// ADT
typedef struct IusPatternList IusPatternList;
/** The pattern list describes the transmit-receive structure of an acquisition. It contains an array of patterns 
 * where a pattern is a reference to a {pulse, source, ReceiveChannelMap, apodization, receiveSettings} tuple, 
 * defined at a certain timestamp.
 * Since we restrict ourselves to a rectangular memory alignment, not all combinations of arbitrary settings are 
 * possible, i.e. during an acquistion of an #IusPatternList sequence the numSamplesPerLine, numChannels and numPulses
 * need to remain constant over the frames. 
 * To guarantee that, the receiveSettings and receiveChannelMap (dictionaries) need to be known during the creation of 
 * a #IusPatternList.
 */
typedef IusPatternList *iupal_t;
#define  IUPAL_INVALID (iupal_t) NULL

/** \brief Create a patternList of size \p numPattern. Using the #IusReceiveSettingsDict and #IusReceiveChannelMapDict
 * to set the data dimensions 
 * \return Returns an empty list for \p numPattern patterns, or #IUPAL_INVALID in case of an error.
 */ 
iupal_t iusPatternListCreate
(
    int numPatterns,                   ///< The number of patterns that will be in the list
    iursd_t receiveSettingsDict,      ///< The receiveSettings dictionary  
    iurcmd_t receiveChannelMapDict    ///< The receiveChannelMap dictionary
);

/** \brief Delete the patternList without deleting the individual patterns.
 * \return Returns #IUS_E_OK when successful or #IUS_ERR_VALUE in case of an invalid list.  
 */
int iusPatternListDelete
(
    iupal_t list          ///< The #IusPatternList of interest.
);

/** \brief Delete the patternList and the individual patterns.
 * \return Returns #IUS_E_OK when successful or #IUS_ERR_VALUE otherwise.  
 */
int iusPatternListDeepDelete
(
    iupal_t list          ///< The #IusPatternList of interest.
);

/** \brief Compare two patternLists with each other.
 * \return Returns #IUS_TRUE when the patterns lists are equal and #IUS_FALSE otherwise.  
 */
int iusPatternListCompare
(
    iupal_t reference,    ///< The #IusPatternList to compare to
    iupal_t actual        ///< The #IusPatternList to compare with
);

/** \brief Get the number of patterns in the list
 * \return Returns the \p numPattern or -1 in case of an invalid argument.  
 */
int iusPatternListGetSize
(
    iupal_t list          ///< The #IusPatternList
);

/** \brief Get the pattern at \p index from the #IusPatternList 
 * \return Returns the #IusPattern in case of success or IUPAL_INVALID in case of an error.
 */
iupa_t iusPatternListGet
(
    iupal_t list,         ///< The #IusPatternList of interest
    int index             ///< the pattern index
);

/** \brief Set a pattern at \p index in the #IusPatternList 
 * \return Returns #IUS_E_OK in case of success or #IUS_ERR_VALUE in case of an error.
 */
int iusPatternListSet
(
    iupal_t list,         ///< The #IusPatternList of interest
    iupa_t member,        ///< the #IusPattern
    int index             ///< THe loction of the #IusPattern
);

#endif //IUSLIBRARY_IUSHLPATTERNLIST_H
