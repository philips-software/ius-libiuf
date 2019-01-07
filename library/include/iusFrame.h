// Created by nlv09165 on 15/08/2018.
#ifndef IUSLIBRARY_IUSHLFRAME_H
#define IUSLIBRARY_IUSHLFRAME_H


// ADT
typedef struct IusFrame IusFrame;
/** A frame is reference to a patternList, a reference to data, and a timestamp. 
*/
typedef IusFrame *iufr_t;
#define  IUF_INVALID (iufr_t) NULL

/** \brief Creates an #IusFrame. 
 * \return Returns an #IusFrame or #IUF_INVALID in case of an error.
*/
iufr_t iusFrameCreate
(
    char *patternListLabel,      ///< The patternList label this frame refers to, e.g. "bmode"
    int dataIndex,               ///< The offset in the data (frame number?)
    float time                   ///< The timestamp in seconds since the start of the acquisition
);

/** \brief Delete an #IusFrame. 
 * \return Returns #IUS_E_OK when successful and #IUS_ERR_VALUE in case of an error.
*/
int iusFrameDelete
(
    iufr_t frame              ///< The #IusFrame to be deleted 
);

/** \brief Compare two #IusFrame object with each other. 
 * \return Returns #IUS_TRUE when the frames are equal and #IUS_FALSE otherwise.
*/
int iusFrameCompare
(
    iufr_t reference,            ///< The #IusFrame to compare to
    iufr_t actual                ///< The #IusFrame to compare with
);

/** \brief Gets the timestamp in seconds of a frame. 
 * \return Returns the timestamp in seconds or -1 in case of an error.
*/
float iusFrameGetTime
(
    iufr_t frame
);

/** \brief Gets the patternListLabel of a frame. 
 * \return Returns the label or NULL in case of an error.
*/
char *iusFrameGetPatternListLabel
(
    iufr_t frame               ///< The #IusFrame of interest
);

/** \brief Gets the dataIndex of a frame. 
 * \return Returns the dataIndex or -1 in case of an error.
*/
int iusFrameGetDataIndex
(
    iufr_t frame               ///< The #IusFrame of interest
);
#endif //IUSLIBRARY_IUSHLFRAME_H
