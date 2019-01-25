// Created by nlv09165 on 15/08/2018.
#ifndef IUSLIBRARY_IUSCOMPOUNDFRAME_H
#define IUSLIBRARY_IUSCOMPOUNDFRAME_H


// ADT
typedef struct IusCompoundFrame IusCompoundFrame;
/** A frame is reference to a patternList, a reference to data, and a timestamp.
*/
typedef IusCompoundFrame *iucfr_t;
#define  IUCFR_INVALID (iucfr_t) NULL

/** \brief Creates an #IusCompoundFrame.
* \return Returns an #IusCompoundFrame or #IUF_INVALID in case of an error.
*/
iucfr_t iusCompoundFrameCreate
(
	char *compoundListLabel,        ///< The reference to the compound Dictionary
	int   dataIndex,				///< The offset in the data (frame number?)
	float time						///< The timestamp in seconds since the start of the acquisition
);

/** \brief Delete an #IusCompoundFrame.
* \return Returns #IUS_E_OK when successful and #IUS_ERR_VALUE in case of an error.
*/
int iusCompoundFrameDelete
(
	iucfr_t frame              ///< The #IusCompoundFrame to be deleted 
);

/** \brief Compare two #IusCompoundFrame object with each other.
* \return Returns #IUS_TRUE when the frames are equal and #IUS_FALSE otherwise.
*/
int iusCompoundFrameCompare
(
	iucfr_t reference,            ///< The #IusCompoundFrame to compare to
	iucfr_t actual                ///< The #IusCompoundFrame to compare with
);

/** \brief Gets the timestamp in seconds of a frame.
* \return Returns the timestamp in seconds or -1 in case of an error.
*/
float iusCompoundFrameGetTime
(
	iucfr_t frame
);

/** \brief Gets the patternListLabel of a frame.
* \return Returns the label or NULL in case of an error.
*/
char *iusCompoundFrameGetCompoundListLabel
(
	iucfr_t frame               ///< The #IusCompoundFrame of interest
);

/** \brief Gets the dataIndex of a frame.
* \return Returns the dataIndex or -1 in case of an error.
*/
int iusCompoundFrameGetDataIndex
(
	iucfr_t frame               ///< The #IusCompoundFrame of interest
);
#endif //IUSLIBRARY_IUSCOMPOUNDFRAME_H
