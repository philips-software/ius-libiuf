//
// Created by Ruijzendaal on 30/03/2018.
//
#ifndef IUSLIBRARY_IUSHLTRANSMITAPODIZATION_H
#define IUSLIBRARY_IUSHLTRANSMITAPODIZATION_H

#include <iusTypes.h>

// ADT
struct IusTransmitApodization;
typedef  struct IusTransmitApodization IusTransmitApodization;
/** a transmit apodization describes an attenuation function for the amplitudes of the transducer's transmit 
 * elements. Amplitude gains should be a floating point value in the range [0.0, 1.0].
 * The size of the apodization function is equal to the number of elements that the corresponding transducer has.  
 * */
typedef  IusTransmitApodization    * iuta_t;
#define  IUTA_INVALID (iuta_t) NULL

/** \brief Construct an #IusTransmitApodization of size \p numElements.
 *
 *   The attenuation values are checked that they are within the allowed range [0.0,1.0]. The \p apodization
 *    should contain \p numElements float values.
*/
iuta_t  iusTransmitApodizationCreate
(
    float *apodization,           ///< The transmit apodization values (numElements items)
	int numElements              /// The number of ekements that the transducer has.
);

/** \brief Delete a #IusTransmitApodization of size numElements
 * \return #IUS_E_OK in case of success or #IUS_ERR_VALUE in case the #IusTransmitApodization is invalid
*/
int iusTransmitApodizationDelete
(
	iuta_t transmitApodization /// The \p IusTransmitApodization that is to be deleted.
);

/** \brief Compare two \p IusTransmitApodization objects with each other
 * \return Returns #IUS_TRUE in case the #IusTransmitApodization objects are equal and #IUS_FALSE otherwise
 */
IUS_BOOL  iusTransmitApodizationCompare
(
	iuta_t reference, ///< The IusTransmitApodization to compare to 
	iuta_t actual     ///< The IusTransmitApodization to compare with
);

/** \brief Get a single value from the transmit apodization function.
 * \return Returns the floating point value of the transmit apodization function at index \p idx
 */ 
float iusTransmitApodizationGetElement
(
	iuta_t transmitApodization, ///< The IusTransmitApodization of interest
	int idx                     ///< the index in the transmit apodization function. Should be smaller than \p numElements
);

/** \brief Get the number of ekements that the transducer has.
 * \return Returns the number of ekements that the transducer has.
 */
int iusTransmitApodizationGetNumElements
(
        iuta_t transmitApodization ///< The IusTransmitApodization of interest
);


/** \brief Set the value of a single elements of the transmit apodization function at index \p idx.
 *   
 *   The attenuation value is checked that it is within the allowed range [0.0,1.0] and the index \p idx is checked 
 *   for its range [0,numElements]. 
 *   
 *   \return Returns #IUS_E_OK in case the element was set, #IUS_ERR_VALUE otherwise.
 */ 
int iusTransmitApodizationSetElement
(
	iuta_t transmitApodization,  ///< The transmit apodization of interest
	int idx,                     ///< The index of the vlaue to be set
	float attenuation            ///< The attenuation value to set.
);

#endif //IUSLIBRARY_IUSHLRECEIVECHANNELMAP_H

