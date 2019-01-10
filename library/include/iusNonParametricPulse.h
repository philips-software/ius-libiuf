//
// Created by Ruijzendaal on 10/04/2018.
//
#ifndef IUSLIBRARY_IUSHLNONPARAMETRICPULSE_H
#define IUSLIBRARY_IUSHLNONPARAMETRICPULSE_H

// ADT
typedef  struct IusNonParametricPulse IusNonParametricPulse;
/** A non parametric pulse describes an ultrasound transmit pulse using a piecewise linear approximation 
 * with a number of {time,voltage} pairs.
 */
typedef  IusNonParametricPulse    * iunpp_t;
#define  IUNPP_INVALID (iunpp_t) NULL

/** \brief Create an #IusNonParametricPulse for a wave function with \p numPulseValues {time,voltage} pairs.
 * \return Returns the created #IusNonParametricPulse or #IUNPP_INVALID in case of an error.
 */
iunpp_t iusNonParametricPulseCreate
(
    int numPulseValues                ///< number of points to describe the pulse waveform 
);

/** \brief Delete an #IusNonParametricPulse
 * \return Returns #IUS_E_OK when succesful or #IUS_ERR_VALUE when the argument is invalid. 
 */
int iusNonParametricPulseDelete
(
    iunpp_t pulse ///< The #IusNonParametricPulse that is to be deleted.
);

/** \brief Compare two #IusNonParametricPulse with each other
* \return Returns #IUS_TRUE in case the pulses are equal and #IUS_FALSE otherwise
*/
int iusNonParametricPulseCompare
(
    iunpp_t reference, ///< The #IusNonParametricPulse to compare to
    iunpp_t actual     ///< The #IusNonParametricPulse to compare with
);
/** \brief Get the number of {time, voltage} pairs.
 * \return Returns the number of {time, voltage} pairs or -1 in case of an error
 */
int iusNonParametricPulseGetNumValues
(
    iunpp_t pulse           ///< The #IusNonParametricPulse of interest
);

/** \brief Get the voltage value at location \p index.
 * \return Returns the voltage at \p index in Volt or #IUS_ERR_VALUE in case of an error
 */
float iusNonParametricPulseGetValueAmplitude
(
    iunpp_t pulse,          ///< The #IusNonParametricPulse of interest
    int index               ///< The index of the {time, voltage} pairs
);

/** \brief Get the time at location \p index.
 * \return Returns the time at \p index in seconds or #IUS_ERR_VALUE in case of an error
 */
float iusNonParametricPulseGetValueTime
(
    iunpp_t pulse,          ///< The #IusNonParametricPulse of interest
    int index               ///< The index of the {time, voltage} pairs
);

/** \brief Set a {time, voltage} pair at location \p index.
 * \return Returns #IUS_E_OK when succesful and #IUS_ERR_VALUE in case of invalid parameters
 */
int iusNonParametricPulseSetValue
(
    iunpp_t pulse,          ///< The #IusNonParametricPulse of interest
    int index,              ///< The index of the {time, voltage} pairs
    float pulseTime,        ///< the time in seconds
    float pulseAmplitude    ///< the amplitude in Volt
);

#endif //IUSLIBRARY_IUSHLNONPARAMETRICPULSE_H
