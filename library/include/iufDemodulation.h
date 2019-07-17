#ifndef IUFLIBRARY_IUFHLDEMODULATION_H
#define IUFLIBRARY_IUFHLDEMODULATION_H

#include <iufTGC.h>
#include <iufFilter.h>

// ADT
typedef struct IufDemodulation IufDemodulation;
typedef IufDemodulation *iudm_t;
#define  IUDM_INVALID (iudm_t) NULL

//iudm_t iufDemodulationCreateWithoutTGCandFilter
//(
//	IufDemodulationMethod method,		///< The downsampling method
//	float sampleFrequency,				///< The sampling frequency in Hz
//	float centerFrequency,				///< The center frequency in Hz after modulation
//	int numSamplesPerLine				///< The number of samples per acquired line
//);

iudm_t iufDemodulationCreate
(
	IufDemodulationMethod method,       ///< The downsampling method
	float sampleFrequency,				///< The sampling frequency in Hz
	float centerFrequency,				///< The center frequency in Hz after demodulation
	int numSamplesPerLine,				///< The number of samples per acquired line 
	int numTGCentries,					///< The size of the TGC function
	int filterKernelSize				///< The size of the preFilter
);

/** \brief Deletes the #IufDemodulation and also the TGC function that it might contain.
*  \return Returns #IUF_E_OK when successful or #IUF_ERR_VALUE otherwise
*/
int iufDemodulationDelete
(
	iudm_t demodulation     ///< The receive settings of interest.
);

/** \brief Compare two demodulation objects with each other, including their TGC function.
*  \return Returns #IUF_TRUE when the settings are equal and #IUF_FALSE otherwise
*/
int iufDemodulationCompare
(
	iudm_t reference,             ///< the receive settings to compare to
	iudm_t actual                 ///< the receive settings to compare with
);

/** \brief Returns the sampleFrequency setting
*  \return The sample frequency in Hz
*/
float iufDemodulationGetSampleFrequency
(
	iudm_t demodulation     ///< the demodulation of interest
);

/** \brief Returns the centerFrequency (after demodulation)
*  \return The center frequency after demodulation in Hz
*/
float iufDemodulationGetCenterFrequency
(
	iudm_t demodulation     ///< the demodulation of interest
);

/** \brief Returns the number of samples per line
*  \return The number of samples each line has
*/
int iufDemodulationGetNumSamplesPerLine
(
	iudm_t demodulation     ///< the demodulation of interest
);

/** \brief Gets the number of \p [time,gain] paira that the TGC has
*  \return The number of TGC entries
*/
int iufDemodulationGetNumTGCentries
(
	iudm_t demodulation     ///< the demodulation of interest
);

/** \brief Gets the number of filter coefficients of the preFilter
*  \return the number of filter coefficients or -1 in case of an error.
*/
int iufDemodulationGetPreFilterKernelSize
(
	iudm_t demodulation     ///< the demodulation of interest
);

/** \brief Gets the TGC object of the demodulation
*  \return the IufTGC object
*/
iutgc_t iufDemodulationGetTGC
(
	iudm_t demodulation  ///< the demodulation of interest
);

/** \brief Gets the fir filter object of the demodulation
*  \return the #IufFirFilter object
*/
iuff_t iufDemodulationGetPreFilter
(
	iudm_t demodulation  ///< the demodulation of interest
);

/** \brief set the TGC of #IufDemodulation
 * \return Returns IUF_E_OK when successful and IUF_ERR_VALUE otherwise
 */
int iufDemodulationSetTGC
(
   iudm_t demodulation,
   iutgc_t tgc
);

/** \brief set the TGC of #IufDemodulation
 * \return Returns IUF_E_OK when successful and IUF_ERR_VALUE otherwise
 */
int iufDemodulationSetPreFilter
(
   iudm_t demodulation,
   iuff_t preFilter
);

 
#endif //IUFLIBRARY_IUFHLRECEIVESETTINGS_H
