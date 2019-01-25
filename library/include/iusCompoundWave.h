// Created by nlv09165 on 15/08/2018.
#ifndef IUSLIBRARY_IUSCOMPOUNDWAVE_H
#define IUSLIBRARY_IUSCOMPOUNDWAVE_H

// ADT
typedef struct IusCompoundWave IusCompoundWave;
/** A compoundWave is decribed by three references to resp. a source, a receiveApodization and a pulse and a timestamp.
 * I.e. a pulse from a source "through" a window at a certain moment.
*/
typedef IusCompoundWave *iucw_t;
#define  IUCW_INVALID (iucw_t) NULL

/** \brief Creates an #IusCompoundWave.
* \return Returns an #IusCompoundWave or #IUCW_INVALID in case of an error.
*/
iucw_t iusCompoundWaveCreate
(
	char *sourceLabel,               ///< The Source that is used for this compound. Label of dictionary key.
	char *receiveApodizationLabel,   ///< The ReceiveApodization that is used for this compound. Label of dictionary key.
	char *receiveChannelMapLabel,    ///< The ReceiveChannelMap that is usedd for this compound. Label of dictionary key.
	char *pulseLabel,                ///< The pulse that is used for this compound. Label of dictionary key.
	float timeStamp	                 ///< The time the pulse was fired.
);

/** \brief Delete an #IusCompoundWave.
* \return Returns #IUS_E_OK when successful and #IUS_ERR_VALUE in case of an error.
*/
int iusCompoundWaveDelete
(
	iucw_t compound              ///< The #IusCompoundWave to be deleted 
);

/** \brief Compare two #IusCompoundWave object with each other.
* \return Returns #IUS_TRUE when the compounds are equal and #IUS_FALSE otherwise.
*/
int iusCompoundWaveCompare
(
	iucw_t reference,            ///< The #IusCompoundWave to compare to
	iucw_t actual                ///< The #IusCompoundWave to compare with
);

/** \brief Gets the source label of a #IusCompoundWave.
* \return Returns the source label or NULL in case of an error.
*/
char *iusCompoundWaveGetSourceLabel
(
	iucw_t compoundWave           ///< The #IusCompoundWave of interest
);

/** \brief Gets the receive apodization of a #IusCompoundWave.
* \return Returns the label or NULL in case of an error.
*/
int iusCompoundGetReceiveApodizationLabel
(
	iucw_t compoundWave           ///< The #IusCompoundWave of interest
);

/** \brief Gets the pulse label of a #IusCompoundWave.
* \return Returns the label or NULL in case of an error.
*/
int iusCompoundWaveGetPulseLabel
(
	iucw_t compoundWave           ///< The #IusCompoundWave of interest
);
#endif //IUSLIBRARY_IUSCOMPOUNDWAVE_H
