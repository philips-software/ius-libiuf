#ifndef IUFLIBRARY_IUFHLPATTERN_H
#define IUFLIBRARY_IUFHLPATTERN_H

// ADT
typedef struct IufPattern IufPattern;
/** A pattern is the combinding structure for the transmit and receive of an ultrasound pulse at a certain time in a frame. 
 * It references a pulse, source, channelmap, transmit apodization and receive settings using a label to their respective 
 * dictionaries that are typically part of e.g. the #IufInputFile structure.
 */ 
typedef IufPattern *iupa_t;
#define  IUPA_INVALID (iupa_t) NULL

/** \brief create a pattern at time \p timeInFrame using labels to reference a pulse, source, channelMap, apodization and 
 * receiveSettings respectively
 * \return Returns the created #IufPattern or #IUPA_INVALID in case of an error.
 */
iupa_t iufPatternCreate
(
    float timeInFrame,                 ///< relative time (s) in a frame
    const char *pPulseLabel,           ///< the name of the pulse 
    const char *pSourceLabel,          ///< the name of the source
    const char *pChannelMapLabel,      ///< the name of the channelMap
    const char *pApodizationLabel,     ///< the name of the apodization
    const char *pReceiveSettingsLabel  ///< the name of the receiveSettings
);

/** \brief Delete a pattern
 * \return Returns #IUF_E_OK in case of success and #IUF_ERR_VALUE in case of an error.
 */ 
int iufPatternDelete
(
    iupa_t pattern ///< The #IufPattern of interest
);

/** \brief Compare two patterns with each other
 * \return Returns #IUF_TRUE when the patterns are equal and #IUF_FALSE otherwise.
 */ 
int iufPatternCompare
(
    iupa_t reference,  ///< The #IufPattern to compare to
    iupa_t actual      ///< The #IufPattern to compare with
);

/** \brief Get the pulse label of the #IufPattern
 * \return Returns the label or NULL if the argument is invalid.
 */
const char * iufPatternGetPulseLabel
(
    iupa_t pattern   ///< The #IufPattern of interest
);

/** \brief Get the source label of the #IufPattern
 * \return Returns the label or NULL if the argument is invalid.
 */
const char * iufPatternGetSourceLabel
(
    iupa_t pattern   ///< The #IufPattern of interest
);

/** \brief Get the receiveChannelMap label of the #IufPattern
 * \return Returns the label or NULL if the argument is invalid.
 */
const char * iufPatternGetChannelMapLabel
(
    iupa_t pattern   ///< The #IufPattern of interest
);

/** \brief Get the apodization label of the #IufPattern
 * \return Returns the label or NULL if the argument is invalid.
 */
const char * iufPatternGetApodizationLabel
(
    iupa_t pattern   ///< The #IufPattern of interest
);

/** \brief Get the receiveSettings label of the #IufPattern
 * \return Returns the label or NULL if the argument is invalid.
 */
const char * iufPatternGetReceivesettingsLabel
(
    iupa_t pattern   ///< The #IufPattern of interest
);

/** \brief Get the time of the #IufPattern in the frame
 * \return Returns the timestamp of the pattern in seconds or NAN in case of an error.
 */
float iufPatternGetTimeInFrame
(
    iupa_t pattern   ///< The #IufPattern of interest
);


#endif //IUFLIBRARY_IUFHLPATTERN_H
