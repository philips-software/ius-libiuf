// Created by nlv09165 on 15/08/2018.
#ifndef IUFLIBRARY_IUFHLFRAME_H
#define IUFLIBRARY_IUFHLFRAME_H


// ADT
typedef struct IufFrame IufFrame;
/** A frame is reference to a patternList, a reference to data, and a timestamp. 
*/
typedef IufFrame *iufr_t;
#define  IUF_INVALID (iufr_t) NULL

/** \brief Creates an #IufFrame.
 * \return Returns an #IufFrame or #IUF_INVALID in case of an error.
*/
iufr_t iufFrameCreate
(
    char *label,      ///< The patternList label this frame refers to, e.g. "bmode"
    int dataIndex,               ///< The offset in the data (frame number?)
    float time                   ///< The timestamp in seconds since the start of the acquisition
);

/** \brief Delete an #IufFrame.
 * \return Returns #IUF_E_OK when successful and #IUF_ERR_VALUE in case of an error.
*/
int iufFrameDelete
(
    iufr_t frame              ///< The #IufFrame to be deleted
);

/** \brief Compare two #IufFrame object with each other.
 * \return Returns #IUF_TRUE when the frames are equal and #IUF_FALSE otherwise.
*/
int iufFrameCompare
(
    iufr_t reference,            ///< The #IufFrame to compare to
    iufr_t actual                ///< The #IufFrame to compare with
);

/** \brief Gets the timestamp in seconds of a frame. 
 * \return Returns the timestamp in seconds or -1 in case of an error.
*/
float iufFrameGetTime
(
    iufr_t frame
);

/** \brief Gets the patternListLabel of a frame. 
 * \return Returns the label or NULL in case of an error.
*/
char *iufFrameGetPatternListLabel
(
    iufr_t frame               ///< The #IufFrame of interest
);

/** \brief Gets the dataIndex of a frame. 
 * \return Returns the dataIndex or -1 in case of an error.
*/
int iufFrameGetDataIndex
(
    iufr_t frame               ///< The #IufFrame of interest
);
#endif //IUFLIBRARY_IUFHLFRAME_H
