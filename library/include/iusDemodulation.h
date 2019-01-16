// Created by nlv09165 on 25/04/2018.
#ifndef IUSLIBRARY_IUSHLDEMODULATION_H
#define IUSLIBRARY_IUSHLDEMODULATION_H

#include <iusTGC.h>

// ADT
typedef struct IusDemodulation IusDemodulation;
typedef IusDemodulation *iudm_t;
#define  IUDM_INVALID (iudm_t) NULL

iudm_t iusDemodulationCreate
(
	float sampleFrequency,        ///< The sampling frequency in Hz
	int numSamplesPerLine,        ///< The number of samples per acquired line 
	int numTGCentries             ///< The size of the TGC function
);

/** \brief Deletes the #IusDemodulation and also the TGC function that it might contain.
*  \return Returns #IUS_E_OK when successful or #IUS_ERR_VALUE otherwise
*/
int iusDemodulationDelete
(
	iudm_t iusDemodulation     ///< The receive settings of interest.
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
	iudm_t iusDemodulation     ///< the receive settings of interest 
);

/** \brief Returns the number of samples per line
*  \return The number of samples each line has
*/
int iusDemodulationGetNumSamplesPerLine
(
	iudm_t iusDemodulation     ///< the receive settings of interest
);

/** \brief Gets the number of \p [time,gain] paira that the TGC has
*  \return The number of TGC entries
*/
int iusDemodulationGetNumTGCentries
(
	iudm_t iusDemodulation     ///< the receive settings of interest
);

/** \brief Gets the TGC object of the receive settings
*  \return the IusTGC object
*/
iutgc_t iusDemodulationGetTGC
(
	iudm_t iusDemodulation  ///< the receive settings of interest
);


#endif //IUSLIBRARY_IUSHLRECEIVESETTINGS_H