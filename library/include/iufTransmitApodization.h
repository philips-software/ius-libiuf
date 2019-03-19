//
// Created by Ruijzendaal on 30/03/2018.
//
#ifndef IUFLIBRARY_IUFHLTRANSMITAPODIZATION_H
#define IUFLIBRARY_IUFHLTRANSMITAPODIZATION_H

#include <iufTypes.h>

// ADT
struct IufTransmitApodization;
typedef  struct IufTransmitApodization IufTransmitApodization;
/** a transmit apodization describes an attenuation function for the amplitudes of the transducer's transmit 
 * elements. Amplitude gains should be a floating point value in the range [0.0, 1.0].
 * The size of the apodization function is equal to the number of elements that the corresponding transducer has.  
 * */
typedef  IufTransmitApodization    * iuta_t;
#define  IUTA_INVALID (iuta_t) NULL

/** \brief Construct an #IufTransmitApodization of size \p numElements.
 *
 *   The attenuation values are checked that they are within the allowed range [0.0,1.0]. The \p apodization
 *    should contain \p numElements float values.
*/
iuta_t  iufTransmitApodizationCreate
(
    float *apodization,           ///< The transmit apodization values (numElements items)
	int numElements              /// The number of ekements that the transducer has.
);

/** \brief Delete a #IufTransmitApodization of size numElements
 * \return #IUF_E_OK in case of success or #IUF_ERR_VALUE in case the #IufTransmitApodization is invalid
*/
int iufTransmitApodizationDelete
(
	iuta_t transmitApodization /// The \p IufTransmitApodization that is to be deleted.
);

/** \brief Compare two \p IufTransmitApodization objects with each other
 * \return Returns #IUF_TRUE in case the #IufTransmitApodization objects are equal and #IUF_FALSE otherwise
 */
IUF_BOOL  iufTransmitApodizationCompare
(
	iuta_t reference, ///< The IufTransmitApodization to compare to
	iuta_t actual     ///< The IufTransmitApodization to compare with
);

/** \brief Get a single value from the transmit apodization function.
 * \return Returns the floating point value of the transmit apodization function at index \p idx
 */ 
float iufTransmitApodizationGetElement
(
	iuta_t transmitApodization, ///< The IufTransmitApodization of interest
	int idx                     ///< the index in the transmit apodization function. Should be smaller than \p numElements
);

/** \brief Get the number of ekements that the transducer has.
 * \return Returns the number of ekements that the transducer has.
 */
int iufTransmitApodizationGetNumElements
(
        iuta_t transmitApodization ///< The IufTransmitApodization of interest
);


/** \brief Set the value of a single elements of the transmit apodization function at index \p idx.
 *   
 *   The attenuation value is checked that it is within the allowed range [0.0,1.0] and the index \p idx is checked 
 *   for its range [0,numElements]. 
 *   
 *   \return Returns #IUF_E_OK in case the element was set, #IUF_ERR_VALUE otherwise.
 */ 
int iufTransmitApodizationSetElement
(
	iuta_t transmitApodization,  ///< The transmit apodization of interest
	int idx,                     ///< The index of the vlaue to be set
	float attenuation            ///< The attenuation value to set.
);

#endif //IUFLIBRARY_IUFHLRECEIVECHANNELMAP_H

