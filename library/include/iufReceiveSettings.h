// Created by nlv09165 on 25/04/2018.
#ifndef IUFLIBRARY_IUFHLRECEIVESETTINGS_H
#define IUFLIBRARY_IUFHLRECEIVESETTINGS_H

#include <iufTGC.h>

// ADT
typedef struct IufReceiveSettings IufReceiveSettings;
/** The receive settings contain the digitization settings of a transducer's data acquisition. It contains \p the sample 
 *  frequency and the number of samples that are taken each line and the TGC info. Typically, constructing this object 
 *  is part of the setup of an #IufInputFile. Start by creating a iufReceiveSettings object using #iufReceiveSettingsCreate()
 *  and use the iufTGCSet() function to add the TGC function. Typically the constructed setting is added to an
 *  #IufReceiveSettingsDict, which is part of the #IufInputFile instance.
 */  
typedef IufReceiveSettings *iurs_t;
#define  IURS_INVALID (iurs_t) NULL

iurs_t iufReceiveSettingsCreate
(
    float sampleFrequency,        ///< The sampling frequency in Hz
    int numSamplesPerLine,        ///< The number of samples per acquired line 
    int numTGCentries             ///< The size of the TGC function
);

/** \brief Deletes the #IufReceiveSettings and also the TGC function that it might contain.
 *  \return Returns #IUF_E_OK when successful or #IUF_ERR_VALUE otherwise
 */
int iufReceiveSettingsDelete
(
    iurs_t receiveSettings     ///< The receive settings of interest.
);

/** \brief Compare two receive settings with each other, including their TGC function.
 *  \return Returns #IUF_TRUE when the settings are equal and #IUF_FALSE otherwise
 */
int iufReceiveSettingsCompare
(
    iurs_t reference,             ///< the receive settings to compare to
    iurs_t actual                 ///< the receive settings to compare with
);

/** \brief Returns the sampleFrequency setting
 *  \return The sample frequency in Hz
 */
float iufReceiveSettingsGetSampleFrequency
(
    iurs_t receiveSettings     ///< the receive settings of interest
);

/** \brief Returns the number of samples per line
 *  \return The number of samples each line has
 */
int iufReceiveSettingsGetNumSamplesPerLine
(
    iurs_t receiveSettings     ///< the receive settings of interest
);

/** \brief Gets the number of \p [time,gain] paira that the TGC has
 *  \return The number of TGC entries
 */
int iufReceiveSettingsGetNumTGCentries
(
    iurs_t receiveSettings     ///< the receive settings of interest
);

/** \brief Gets the TGC object of the receive settings
 *  \return the IufTGC object
 */
iutgc_t iufReceiveSettingsGetTGC
(
    iurs_t receiveSettings  ///< the receive settings of interest
);


#endif //IUFLIBRARY_IUFHLRECEIVESETTINGS_H
