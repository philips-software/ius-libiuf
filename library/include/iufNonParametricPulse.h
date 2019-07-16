#ifndef IUFLIBRARY_IUFHLNONPARAMETRICPULSE_H
#define IUFLIBRARY_IUFHLNONPARAMETRICPULSE_H

// ADT
typedef  struct IufNonParametricPulse IufNonParametricPulse;
/** A non parametric pulse describes an ultrasound transmit pulse using a piecewise linear approximation 
 * with a number of {time,voltage} pairs.
 */
typedef  IufNonParametricPulse    * iunpp_t;
#define  IUNPP_INVALID (iunpp_t) NULL

/** \brief Create an #IufNonParametricPulse for a wave function with \p numPulseValues {time,voltage} pairs.
 * \return Returns the created #IufNonParametricPulse or #IUNPP_INVALID in case of an error.
 */
iunpp_t iufNonParametricPulseCreate
(
    int numPulseValues                ///< number of points to describe the pulse waveform 
);

/** \brief Delete an #IufNonParametricPulse
 * \return Returns #IUF_E_OK when succesful or #IUF_ERR_VALUE when the argument is invalid.
 */
int iufNonParametricPulseDelete
(
    iunpp_t pulse ///< The #IufNonParametricPulse that is to be deleted.
);

/** \brief Compare two #IufNonParametricPulse with each other
* \return Returns #IUF_TRUE in case the pulses are equal and #IUF_FALSE otherwise
*/
int iufNonParametricPulseCompare
(
    iunpp_t reference, ///< The #IufNonParametricPulse to compare to
    iunpp_t actual     ///< The #IufNonParametricPulse to compare with
);
/** \brief Get the number of {time, voltage} pairs.
 * \return Returns the number of {time, voltage} pairs or -1 in case of an error
 */
int iufNonParametricPulseGetNumValues
(
    iunpp_t pulse           ///< The #IufNonParametricPulse of interest
);

/** \brief Get the voltage value at location \p index.
 * \return Returns the voltage at \p index in Volt or #IUF_ERR_VALUE in case of an error
 */
float iufNonParametricPulseGetValueAmplitude
(
    iunpp_t pulse,          ///< The #IufNonParametricPulse of interest
    int index               ///< The index of the {time, voltage} pairs
);

/** \brief Get the time at location \p index.
 * \return Returns the time at \p index in seconds or #IUF_ERR_VALUE in case of an error
 */
float iufNonParametricPulseGetValueTime
(
    iunpp_t pulse,          ///< The #IufNonParametricPulse of interest
    int index               ///< The index of the {time, voltage} pairs
);

/** \brief Set a {time, voltage} pair at location \p index.
 * \return Returns #IUF_E_OK when succesful and #IUF_ERR_VALUE in case of invalid parameters
 */
int iufNonParametricPulseSetValue
(
    iunpp_t pulse,          ///< The #IufNonParametricPulse of interest
    int index,              ///< The index of the {time, voltage} pairs
    float pulseTime,        ///< the time in seconds
    float pulseAmplitude    ///< the amplitude in Volt
);

#endif //IUFLIBRARY_IUFHLNONPARAMETRICPULSE_H
