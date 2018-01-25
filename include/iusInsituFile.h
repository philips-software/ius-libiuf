//=============================================================================
//    COPYRIGHT 2016 PHILIPS RESEARCH
//=============================================================================
//
//  Source Name   : iusInsituFile.h
//                  Insitu file i/o
//  iusVersion    : 255.255.255.255
//
//=============================================================================
/**
* \file iusInsituFile.h
* \brief File IO routines for Insitu files according to the specification doc.
*
* \author Harm
*/

#ifndef IUS_INSITU_FILE_H
#define IUS_INSITU_FILE_H

#ifdef __cplusplus
extern "C" {
#endif

typedef void * InsituFileHandle;

/**
* \brief DNL data structure holding the ultrasound image and metadata
*/
typedef struct
{
    int                numImageRows;
    int                numImageCols;
    unsigned char *    pImageData;        /**< the image pixel gray level values      */
} InsituDNLData;

/**
* \brief Frame data structure holding the RF sensor data samples and metadata
*/
typedef struct
{
	long              frameCount;         /**< frame number (counting starts from 0)    */
	long long         frameTimeStamp;     /**< time in units of 1/sampleFrequency       */
	int               numLines;           /**< the number of RF lines in one frame      */
	int               numSamplesPerLine;  /**< the number of samples per RF line        */
	float             sampleFrequency;    /**< the sample rate in Hz                    */
	short *           pSensorData;        /**< ptr to the sensor RF data sample values  */
	int *             pLMI;               /**< ptr to 1D array of line mode indices     */
	int *             pLPI;               /**< ptr to 1D array of line pattern indices  */
	int *             pSLI;               /**< ptr to 1D array of scam line indices     */
	int *             pTZI;               /**< ptr to 1D array of transmit zone indices */
} InsituFrameData;

/**
* \brief Ground truth data with the Insitu encoder values of the position sensor
*/
typedef struct
{
    int                     encoderXZ;
    int                     encoderY;
} InsituGroundTruthData;

/**
* \brief Insitu file data all together
*/
typedef struct
{
	InsituDNLData           dNLData;
	InsituFrameData         frameData;
    InsituGroundTruthData   groundTruth;
} InsituFileData;

/**
* \brief Open an Insitu file for reading.
*
* File open occurs by full pathname, and after opening the file the read position
* is at the start of the first frame.
*
* \pre         the input file exists
* \post        The Insitu file has been opened for reading
* \param[in]   pInsituFileName    : full path name of the input file
* \return      InsituFileHandle   : handle to the Insitu file
*/
InsituFileHandle iusInsituFileCreate
(
	const char * const pInsituFileName
);

/**
* \brief Read the next frame from the Insitu input file.
*
* \pre         the input file was opened
* \post        the frame data was read and the read position is at the start
*              of the next frame
* \param[in]   insituFileHandle: handle to the Insitu file
* \return      pointer to the current frame data
*/
InsituFileData * iusInsituFileReadFrame
(
	InsituFileHandle insituFileHandle
);

/**
* \brief Closes the Insitu input file
*
* \pre         the input file was opened
* \post        the input file has been closed
* \param[in]   insituFileHandle   : handle to the Insitu file
* \return      void
*/
void iusInsituFileClose
(
    InsituFileHandle insituFileHandle
);

#ifdef __cplusplus
}
#endif

#endif  // IUS_INSITU_FILE_H

