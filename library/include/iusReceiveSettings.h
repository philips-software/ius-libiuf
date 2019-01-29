// Created by nlv09165 on 25/04/2018.
#ifndef IUSLIBRARY_IUSHLRECEIVESETTINGS_H
#define IUSLIBRARY_IUSHLRECEIVESETTINGS_H

#include <iusTGC.h>

// ADT
typedef struct IusReceiveSettings IusReceiveSettings;
/** The receive settings contain the digitization settings of a transducer's data acquisition. It contains \p the sample 
 *  frequency and the number of samples that are taken each line and the TGC info. Typically, constructing this object 
 *  is part of the setup of an #IusInputFile. Start by creating a iusReceiveSettings object using #iusReceiveSettingsCreate() 
 *  and use the iusTGCSet() function to add the TGC function. Typically the constructed setting is added to an 
 *  #IusReceiveSettingsDict, which is part of the #IusInputFile instance. 
 */  
typedef IusReceiveSettings *iurs_t;
#define  IURS_INVALID (iurs_t) NULL

iurs_t iusReceiveSettingsCreate
(
    float sampleFrequency,        ///< The sampling frequency in Hz
    int numSamplesPerLine,        ///< The number of samples per acquired line 
    int numTGCentries             ///< The size of the TGC function
);

/** \brief Deletes the #IusReceiveSettings and also the TGC function that it might contain. 
 *  \return Returns #IUS_E_OK when successful or #IUS_ERR_VALUE otherwise
 */
int iusReceiveSettingsDelete
(
    iurs_t receiveSettings     ///< The receive settings of interest.
);

/** \brief Compare two receive settings with each other, including their TGC function.
 *  \return Returns #IUS_TRUE when the settings are equal and #IUS_FALSE otherwise
 */
int iusReceiveSettingsCompare
(
    iurs_t reference,             ///< the receive settings to compare to
    iurs_t actual                 ///< the receive settings to compare with
);

/** \brief Returns the sampleFrequency setting
 *  \return The sample frequency in Hz
 */
float iusReceiveSettingsGetSampleFrequency
(
    iurs_t receiveSettings     ///< the receive settings of interest
);

/** \brief Returns the number of samples per line
 *  \return The number of samples each line has
 */
int iusReceiveSettingsGetNumSamplesPerLine
(
    iurs_t receiveSettings     ///< the receive settings of interest
);

/** \brief Gets the number of \p [time,gain] paira that the TGC has
 *  \return The number of TGC entries
 */
int iusReceiveSettingsGetNumTGCentries
(
    iurs_t receiveSettings     ///< the receive settings of interest
);

/** \brief Gets the TGC object of the receive settings
 *  \return the IusTGC object 
 */
iutgc_t iusReceiveSettingsGetTGC
(
    iurs_t receiveSettings  ///< the receive settings of interest
);


#endif //IUSLIBRARY_IUSHLRECEIVESETTINGS_H
