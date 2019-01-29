//
// Created by Ruijzendaal on 30/03/2018.
//
#ifndef IUSLIBRARY_IUSRECEIVEAPODIZATION_H
#define IUSLIBRARY_IUSRECEIVEAPODIZATION_H

#include <iusTypes.h>

// ADT
struct IusReceiveApodization;
typedef  struct IusReceiveApodization IusReceiveApodization;
/** a receive apodization describes an attenuation function for the amplitudes of the transducer's receive 
 * elements. Amplitude gains typically are a floating point value in the range [0.0, 1.0], and must be positive.
 * The size of the apodization function is equal to the number of elements that the corresponding transducer has.  
 * */
typedef  IusReceiveApodization    * iura_t;
#define  IURA_INVALID (iura_t) NULL

/** \brief Construct an #IusReceiveApodization of size \p numElements.
*/
iura_t  iusReceiveApodizationCreate
(
	int numElements /// The number of ekements that the transducer has. 
);

/** \brief Delete a #IusReceiveApodization of size numElements
 * \return #IUS_E_OK in case of success or #IUS_ERR_VALUE in case the #IusReceiveApodization is invalid
*/
int iusReceiveApodizationDelete
(
	iura_t receiveApodization /// The \p IusReceiveApodization that is to be deleted.
);

/** \brief Compare two \p IusReceiveApodization objects with each other
 * \return Returns #IUS_TRUE in case the #IusReceiveApodization objects are equal and #IUS_FALSE otherwise
 */
IUS_BOOL  iusReceiveApodizationCompare
(
	iura_t reference, ///< The IusReceiveApodization to compare to 
	iura_t actual     ///< The IusReceiveApodization to compare with
);

/** \brief Get a single value from the receive apodization function.
 * \return Returns the floating point value of the receive apodization function at index \p idx
 */ 
float iusReceiveApodizationGetElement
(
	iura_t receiveApodization, ///< The IusReceiveApodization of interest
	int idx                     ///< the index in the receive apodization function. Should be smaller than \p numElements
);
/** \brief Set the value of a single elements of the receive apodization function at index \p idx.
 *   
 *   The attenuation value is checked that it is within the allowed range [0.0,1.0] and the index \p idx is checked 
 *   for its range [0,numElements]. 
 *   
 *   \return Returns #IUS_E_OK in case the element was set, #IUS_ERR_VALUE otherwise.
 */ 
int iusReceiveApodizationSetElement
(
	iura_t receiveApodization,  ///< The receive apodization of interest
	int idx,                     ///< The index of the vlaue to be set
	float attenuation            ///< The attenuation value to set.
);


/** \brief Set the receive apodization function.
 *   
 *   The attenuation values are checked that they are within the allowed range [0.0,1.0]. The \p apodization  
 *    should contain \p numElements float values. 
 *   
 *   \return Returns #IUS_E_OK in case the elements were set, #IUS_ERR_VALUE otherwise.
 */ 
int iusReceiveApodizationSetApodization
(
	iura_t receiveApodization,   ///< The receive apodization of interest
	float *apodization            ///< The receive apodization values (numElements items)
);

#endif //IUSLIBRARY_IUSRECEIVEAPODIZATION_H

