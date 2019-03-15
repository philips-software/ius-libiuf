// Created by nlv09165 on 25/04/2018.
#ifndef IUSLIBRARY_IUSHLDEMODULATION_H
#define IUSLIBRARY_IUSHLDEMODULATION_H

#include <iusTGC.h>
#include <iusFilter.h>

// ADT
typedef struct IusDemodulation IusDemodulation;
typedef IusDemodulation *iudm_t;
#define  IUDM_INVALID (iudm_t) NULL

//iudm_t iusDemodulationCreateWithoutTGCandFilter
//(
//	IusDemodulationMethod method,		///< The downsampling method
//	float sampleFrequency,				///< The sampling frequency in Hz
//	float centerFrequency,				///< The center frequency in Hz after modulation
//	int numSamplesPerLine				///< The number of samples per acquired line
//);

iudm_t iusDemodulationCreate
(
	IusDemodulationMethod method,       ///< The downsampling method
	float sampleFrequency,				///< The sampling frequency in Hz
	float centerFrequency,				///< The center frequency in Hz after demodulation
	int numSamplesPerLine,				///< The number of samples per acquired line 
	int numTGCentries,					///< The size of the TGC function
	int filterKernelSize				///< The size of the preFilter
);

/** \brief Deletes the #IusDemodulation and also the TGC function that it might contain.
*  \return Returns #IUS_E_OK when successful or #IUS_ERR_VALUE otherwise
*/
int iusDemodulationDelete
(
	iudm_t demodulation     ///< The receive settings of interest.
);

/** \brief Compare two demodulation objects with each other, including their TGC function.
*  \return Returns #IUS_TRUE when the settings are equal and #IUS_FALSE otherwise
*/
int iusDemodulationCompare
(
	iudm_t reference,             ///< the receive settings to compare to
	iudm_t actual                 ///< the receive settings to compare with
);

/** \brief Returns the sampleFrequency setting
*  \return The sample frequency in Hz
*/
float iusDemodulationGetSampleFrequency
(
	iudm_t demodulation     ///< the demodulation of interest
);

/** \brief Returns the centerFrequency (after demodulation)
*  \return The center frequency after demodulation in Hz
*/
float iusDemodulationGetCenterFrequency
(
	iudm_t demodulation     ///< the demodulation of interest
);

/** \brief Returns the number of samples per line
*  \return The number of samples each line has
*/
int iusDemodulationGetNumSamplesPerLine
(
	iudm_t demodulation     ///< the demodulation of interest
);

/** \brief Gets the number of \p [time,gain] paira that the TGC has
*  \return The number of TGC entries
*/
int iusDemodulationGetNumTGCentries
(
	iudm_t demodulation     ///< the demodulation of interest
);

/** \brief Gets the number of filter coefficients of the preFilter
*  \return the number of filter coefficients or -1 in case of an error.
*/
int iusDemodulationGetPreFilterKernelSize
(
	iudm_t demodulation     ///< the demodulation of interest
);

/** \brief Gets the TGC object of the demodulation
*  \return the IusTGC object
*/
iutgc_t iusDemodulationGetTGC
(
	iudm_t demodulation  ///< the demodulation of interest
);

/** \brief Gets the fir filter object of the demodulation
*  \return the #IusFirFilter object
*/
iuff_t iusDemodulationGetPreFilter
(
	iudm_t demodulation  ///< the demodulation of interest
);

/** \brief set the TGC of #IusDemodulation
 * \return Returns IUS_E_OK when successful and IUS_ERR_VALUE otherwise
 */
int iusDemodulationSetTGC
(
   iudm_t demodulation,
   iutgc_t tgc
);

/** \brief set the preFilter of #IusDemodulation
 * \return Returns IUS_E_OK when successful and IUS_ERR_VALUE otherwise
 */
int iusDemodulationSetPreFilter
(
   iudm_t demodulation,
   iuff_t preFilter
);

 
#endif //IUSLIBRARY_IUSHLRECEIVESETTINGS_H
