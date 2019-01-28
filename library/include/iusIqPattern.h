
// Created by nlv09165 on 13/07/2018.
#ifndef IUSLIBRARY_IUSHLIQPATTERN_H
#define IUSLIBRARY_IUSHLIQPATTERN_H

// ADT
typedef struct IusIqPattern IusIqPattern;
/** A pattern is the combinding structure for the transmit and receive of an ultrasound pulse at a certain time in a frame. 
 * It references a pulse, source, channelmap, transmit apodization and receive settings using a label to their respective 
 * dictionaries that are typically part of e.g. the #IusInputFile structure. 
 */ 
typedef IusIqPattern *iuiqpa_t;
#define  IUIQPA_INVALID (iuiqpa_t) NULL

/** \brief create a pattern at time \p timeInFrame using labels to reference a pulse, source, channelMap, apodization and 
 * receiveSettings respectively
 * \return Returns the created #IusPattern or #IUPA_INVALID in case of an error.
 */
iuiqpa_t iusIqPatternCreate
(
    float timeInFrame,                 ///< relative time (s) in a frame
    const char *pPulseLabel,           ///< the name of the pulse 
    const char *pSourceLabel,          ///< the name of the source
    const char *pChannelMapLabel,      ///< the name of the channelMap
    const char *pApodizationLabel,     ///< the name of the apodization
    const char *pDemodulationLabel     ///< the name of the receiveSettings
);

/** \brief Delete a pattern
 * \return Returns #IUS_E_OK in case of success and #IUS_ERR_VALUE in case of an error.
 */ 
int iusIqPatternDelete
(
    iuiqpa_t iusIqPattern ///< The #IusPattern of interest
);

/** \brief Compare two patterns with each other
 * \return Returns #IUS_TRUE when the patterns are equal and #IUS_FALSE otherwise.
 */ 
int iusIqPatternCompare
(
    iuiqpa_t reference,  ///< The #IusPattern to compare to
    iuiqpa_t actual      ///< The #IusPattern to compare with
);

/** \brief Get the pulse label of the #IusPattern
 * \return Returns the label or NULL if the argument is invalid.
 */
const char * iusIqPatternGetPulseLabel
(
    iuiqpa_t iusIqPattern   ///< The #IusPattern of interest
);

/** \brief Get the source label of the #IusPattern
 * \return Returns the label or NULL if the argument is invalid.
 */
const char * iusIqPatternGetSourceLabel
(
    iuiqpa_t iusIqPattern   ///< The #IusPattern of interest
);

/** \brief Get the receiveChannelMap label of the #IusPattern
 * \return Returns the label or NULL if the argument is invalid.
 */
const char * iusIqPatternGetChannelMapLabel
(
    iuiqpa_t iusIqPattern   ///< The #IusPattern of interest
);

/** \brief Get the apodization label of the #IusPattern
 * \return Returns the label or NULL if the argument is invalid.
 */
const char * iusIqPatternGetApodizationLabel
(
    iuiqpa_t iusIqPattern   ///< The #IusPattern of interest
);

/** \brief Get the receiveSettings label of the #IusPattern
 * \return Returns the label or NULL if the argument is invalid.
 */
const char * iusIqPatternGetDemodulationLabel
(
    iuiqpa_t iusIqPattern   ///< The #IusPattern of interest
);

/** \brief Get the time of the #IusPattern in the frame
 * \return Returns the timestamp of the pattern in seconds or NAN in case of an error.
 */
float iusIqPatternGetTimeInFrame
(
    iuiqpa_t iusIqPattern   ///< The #IusPattern of interest
);


#endif //IUSLIBRARY_IUSHLIQPATTERN_H
