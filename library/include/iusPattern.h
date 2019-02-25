
// Created by nlv09165 on 13/07/2018.
#ifndef IUSLIBRARY_IUSHLPATTERN_H
#define IUSLIBRARY_IUSHLPATTERN_H

// ADT
typedef struct IusPattern IusPattern;
/** A pattern is the combinding structure for the transmit and receive of an ultrasound pulse at a certain time in a frame. 
 * It references a pulse, source, channelmap, transmit apodization and receive settings using a label to their respective 
 * dictionaries that are typically part of e.g. the #IusInputFile structure. 
 */ 
typedef IusPattern *iupa_t;
#define  IUPA_INVALID (iupa_t) NULL

/** \brief create a pattern at time \p timeInFrame using labels to reference a pulse, source, channelMap, apodization and 
 * receiveSettings respectively
 * \return Returns the created #IusPattern or #IUPA_INVALID in case of an error.
 */
iupa_t iusPatternCreate
(
    float timeInFrame,                 ///< relative time (s) in a frame
    const char *pPulseLabel,           ///< the name of the pulse 
    const char *pSourceLabel,          ///< the name of the source
    const char *pChannelMapLabel,      ///< the name of the channelMap
    const char *pApodizationLabel,     ///< the name of the apodization
    const char *pReceiveSettingsLabel  ///< the name of the receiveSettings
);

/** \brief Delete a pattern
 * \return Returns #IUS_E_OK in case of success and #IUS_ERR_VALUE in case of an error.
 */ 
int iusPatternDelete
(
    iupa_t pattern ///< The #IusPattern of interest
);

/** \brief Compare two patterns with each other
 * \return Returns #IUS_TRUE when the patterns are equal and #IUS_FALSE otherwise.
 */ 
int iusPatternCompare
(
    iupa_t reference,  ///< The #IusPattern to compare to
    iupa_t actual      ///< The #IusPattern to compare with
);

/** \brief Get the pulse label of the #IusPattern
 * \return Returns the label or NULL if the argument is invalid.
 */
const char * iusPatternGetPulseLabel
(
    iupa_t pattern   ///< The #IusPattern of interest
);

/** \brief Get the source label of the #IusPattern
 * \return Returns the label or NULL if the argument is invalid.
 */
const char * iusPatternGetSourceLabel
(
    iupa_t pattern   ///< The #IusPattern of interest
);

/** \brief Get the receiveChannelMap label of the #IusPattern
 * \return Returns the label or NULL if the argument is invalid.
 */
const char * iusPatternGetChannelMapLabel
(
    iupa_t pattern   ///< The #IusPattern of interest
);

/** \brief Get the apodization label of the #IusPattern
 * \return Returns the label or NULL if the argument is invalid.
 */
const char * iusPatternGetApodizationLabel
(
    iupa_t pattern   ///< The #IusPattern of interest
);

/** \brief Get the receiveSettings label of the #IusPattern
 * \return Returns the label or NULL if the argument is invalid.
 */
const char * iusPatternGetReceivesettingsLabel
(
    iupa_t pattern   ///< The #IusPattern of interest
);

/** \brief Get the time of the #IusPattern in the frame
 * \return Returns the timestamp of the pattern in seconds or NAN in case of an error.
 */
float iusPatternGetTimeInFrame
(
    iupa_t pattern   ///< The #IusPattern of interest
);


#endif //IUSLIBRARY_IUSHLPATTERN_H
