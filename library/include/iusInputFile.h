// Created by nlv09165 on 11/07/2018.
#ifndef IUSLIBRARY_IUSHLINPUTFILE_H
#define IUSLIBRARY_IUSHLINPUTFILE_H

#include <iusAcquisition.h>
#include <iusPatternListDict.h>
#include <iusPulseDict.h>
#include <iusReceiveChannelMapDict.h>
#include <iusTransmitApodizationDict.h>
#include <iusFrameList.h>
#include "iusSourceDict.h"
#include "iusReceiveSettingsDict.h"
#include "iusTransducer.h"
#include "iusData.h"
#include "iusOffset.h"

// ADT
typedef struct IusHistoryNode IusInputFile;
/** The input file is the instance for the "raw" file format. It contains the unprocessed ultrasound aquisition.
 * The instance contains a #IusFrameList, a #IusTransducer definition, #IusAcquisition information and one or more data blocks
 * referering to a patternList. In addition, node history can be added.
 */
typedef IusInputFile *iuif_t;

#define IUIF_INVALID (iuif_t) NULL
#define IUS_DEFAULT_NUM_FRAMES  1

/** \brief Create an empty file instance with a filename.
 * \return Returns the empty file instance when succesful or #IUIF_INVALID in case of an error.
 */
iuif_t iusInputFileCreate
(
    const char *filename  ///< The filename for the file instance.
);

/** \brief Creates a datablock for a frame for the acquisition with the name \p label.
 *  \return Returns an allocated memory block of a "label"-frame or IUD_INVALID in case of an error.
 */
iud_t iusInputFileFrameCreate
(
    iuif_t inputFile,    ///< The #IusInputFile of interest
    char *label             ///< The acquisition type
);

/** \brief Creates a datablock for a response (numSamplesPerLine * numChannels) for the acquisition with
 * the name \p label.
 * \return Returns an allocated memory block of a "label"-response or IUD_INVALID in case of an error.
 */
iud_t iusInputFileResponseCreate
(
    iuif_t inputFile,    ///< The #IusInputFile of interest
    char *label             ///< The acquisition type
);

/** \brief Creates a datablock for a single channel (i.e. numSamplesPerLine floats) for the acquisition with
 * the name \p label.
 * \return Returns an allocated memory block of a "label"-channel or IUD_INVALID in case of an error.
 */
iud_t iusInputFileChannelCreate
(
    iuif_t inputFile,    ///< The #IusInputFile of interest
    char *label             ///< The acquisition type
);

/** \brief Creates a datablock for a single channel (i.e. numSamplesPerLine floats) for the acquisition with
 * the name \p label.
 * \return Returns an allocated memory block of a "label"-channel or IUD_INVALID in case of an error.
 */
int iusInputFileDelete
(
    iuif_t inputFile     ///< The #IusInputFile of interest
);

/** \brief Load an inputFile instance from a file.
 * \return Returns an input file instance or IUIF_INVALID the file could not be read.
 */
iuif_t iusInputFileNodeLoad
(
	const char *pFilename     ///< The filename of the inputFile
);

/** \brief Save (serialize) the inputFile instance to file.
 * \return Returns #IUS_E_OK when succesful or #IUS_ERR_VALUE in case of an error
 */
int iusInputFileNodeSave
(
	iuif_t inputFile       ///< The #IusInputFile of interest
);

/** \brief Closes access to the inputFile
 * \return Returns #IUS_E_OK when succesful or #IUS_ERR_VALUE in case of an error
 */
int iusInputFileClose
(
    iuif_t inputFile        ///< The #IusInputFile of interest
);

/** \brief Compares two inputFile instances with each other.
 * \return Returns #IUS_TRUE when the inputFiles are equal and #IUS_FALSE otherwise.
 */
int iusInputFileCompare
(
    iuif_t reference,        ///< The #IusInputFile to compare to
    iuif_t actual            ///< The #IusInputFile to compare with
);

/** \brief Gets the #IusFrameList object from an inputFile.
 * \return Returns the requested #IusFrameList or #IUFL_INVALID in case of an error.
 */
iufl_t iusInputFileGetFrameList
(
	iuif_t inputFile       ///< The #IusInputFile of interest
);

/** \brief Gets the #IusPatternListDict object from an inputFile.
 * \return Returns the requested #IusPatternListDict or NULL in case of an error.
 */
iupald_t iusInputFileGetPatternListDict
(
	iuif_t inputFile       ///< The #IusInputFile of interest
);

/** \brief Gets the #IusPulseDict object from an inputFile.
 * \return Returns the requested #IusPulseDict or #IUPD_INVALID in case of an error.
 */
iupd_t iusInputFileGetPulseDict
(
	iuif_t inputFile       ///< The #IusInputFile of interest
);

/** \brief Gets the #IusSourceDict object from an inputFile.
 * \return Returns the requested #IusSourceDict or #IUSD_INVALID in case of an error.
 */
iusd_t iusInputFileGetSourceDict
(
	iuif_t inputFile       ///< The #IusInputFile of interest
);

/** \brief Gets the #IusReceiveChannelMapDict object from an inputFile.
 * \return Returns the requested #IusReceiveChannelMapDict or #IURCMD_INVALID in case of an error.
 */
iurcmd_t iusInputFileGetReceiveChannelMapDict
(
	iuif_t inputFile       ///< The #IusInputFile of interest
);

/** \brief Gets the #IusTransmitApodizationDict object from an inputFile.
 * \return Returns the requested #IusTransmitApodizationDict or NULL in case of an error.
 */
iutad_t  iusInputFileGetTransmitApodizationDict
(
	iuif_t inputFile       ///< The #IusInputFile of interest
);

/** \brief Gets the #IusReceiveSettingsDict object from an inputFile.
 * \return Returns the requested #IusReceiveSettingsDict or NULL in case of an error.
 */
iursd_t iusInputFileGetReceiveSettingsDict
(
    iuif_t inputFile        ///< The #IusInputFile of interest
);

/** \brief Gets the #IusAcquisition object from an inputFile.
 * \return Returns the requested #IusAcquisition or NULL in case of an error.
 */
iua_t iusInputFileGetAcquisition
(
	iuif_t inputFile       ///< The #IusInputFile of interest
);

/** \brief Gets the #IusTransducer object from an inputFile.
 * \return Returns the requested #IusTransducer or NULL in case of an error.
 */
iut_t iusInputFileGetTransducer
(
	iuif_t inputFile       ///< The #IusInputFile of interest
);

/** \brief Returns the number of frames an inputFile has.
 * \return Returns the number of frames or 0 if there are no frames, or -1 in case of an invalid argument.
 */
int iusInputFileGetNumFrames
(
    iuif_t inputFile        ///< The #IusInputFile of interest
);

/** \brief Gets the number of responses for the patternList type \p label.
 * \return Returns the number of responses (numPatterns) or -1 in case of an error.
 */
int iusInputFileGetNumResponses
(
	iuif_t inputFile,          ///< The #IusInputFile of interest
	char *label                ///< the patternList type
);

/** \brief Gets the number of channels for the patternList type \p label.
 * \return Returns the number of channels or -1 in case of an error.
 */
int iusInputFileGetNumChannels
(
    iuif_t inputFile,       ///< The #IusInputFile of interest
    char *label                ///< the patternList type
);


/** \brief Gets the number of samples per line for this \p label.
 * \return Returns the number of channels or -1 in case of an error.
 */
int iusInputFileGetSamplesPerLine
(
	iuif_t inputFile,       ///< The #IusInputFile of interest
	char *label             ///< the patternList type
);

// Setters
/** \brief Sets the frameList of an inputFile.
 * \return Returns #IUS_E_OK when succesful or #IUS_ERR_VALUE in case of an error.
 */
int iusInputFileSetFrameList
(
	iuif_t inputFile,          ///< The #IusInputFile of interest
	iufl_t frameList           ///< The #IusFrameList to set
);

/** \brief Sets the patternListDict of an inputFile.
 * \return Returns #IUS_E_OK when succesful or #IUS_ERR_VALUE in case of an error.
 */
int iusInputFileSetPatternListDict
(
	iuif_t inputFile,          ///< The #IusInputFile of interest
	iupald_t patternListDict   ///< The #IusPatternListDict to set
);

/** \brief Sets the pulseDict of an inputFile.
 * \return Returns #IUS_E_OK when succesful or #IUS_ERR_VALUE in case of an error.
 */
int iusInputFileSetPulseDict
(
	iuif_t inputFile,         ///< The #IusInputFile of interest
	iupd_t pulseDict          ///< The #IusPulseDict to set
);

/** \brief Sets the sourceDict of an inputFile.
 * \return Returns #IUS_E_OK when succesful or #IUS_ERR_VALUE in case of an error.
 */
int iusInputFileSetSourceDict
(
    iuif_t inputFile,                       ///< The #IusInputFile of interest
    iusd_t sourceDict                       ///< The #IusSourceDict to set
);

/** \brief Sets the receiveChannelMap of an inputFile.
 * \return Returns #IUS_E_OK when succesful or #IUS_ERR_VALUE in case of an error.
 */
int iusInputFileSetReceiveChannelMapDict
(
    iuif_t inputFile,                       ///< The #IusInputFile of interest
    iurcmd_t receiveChannelMapDict          ///< The #IusReceiveChannelMapDict to set
);

/** \brief Sets the transmitApodizationDict of an inputFile.
 * \return Returns #IUS_E_OK when succesful or #IUS_ERR_VALUE in case of an error.
 */
int iusInputFileSetTransmitApodizationDict
(
	iuif_t inputFile,                       ///< The #IusInputFile of interest
	iutad_t transmitApodizationDict         ///< The #IusTransmitApodizationDict to set
);

/** \brief Sets the receiveSettingsDict of an inputFile.
 * \return Returns #IUS_E_OK when succesful or #IUS_ERR_VALUE in case of an error.
 */
int iusInputFileSetReceiveSettingsDict
(
    iuif_t inputFile,                       ///< The #IusInputFile of interest
    iursd_t receiveSettingsDict             ///< The #IusTransmitApodizationDict to set
);

/** \brief Sets the acquisition info of an inputFile.
 * \return Returns #IUS_E_OK when succesful or #IUS_ERR_VALUE in case of an error.
 */
int iusInputFileSetAcquisition
(
	iuif_t inputFile,                       ///< The #IusInputFile of interest
	iua_t acquisition                       ///< The #IusAcquisition to set
);

/** \brief Sets the transducer of an inputFile.
 * \return Returns #IUS_E_OK when succesful or #IUS_ERR_VALUE in case of an error.
 */
int iusInputFileSetTransducer
(
	iuif_t inputFile,                       ///< The #IusInputFile of interest
	iut_t  transducer                       ///< The #IusTransducer to set
);

/** \brief Saves a channel of data to an inputFile.
 * \return Returns #IUS_E_OK when succesful or <0 in case of an error.
 */
int iusInputFileChannelSave
(
    iuif_t inputFile,                       ///< The #IusInputFile of interest
    char *label,                            ///< The label of the patternList
    iud_t channel,                          ///< The channel data to save
    iuo_t channel_offset                    ///< The offsets in the datablock
);

/** \brief Loads a single channel of data from an inputFile.
 * \return Returns #IUS_E_OK when succesful or <0 in case of an error.
 */
int iusInputFileChannelLoad
(
    iuif_t inputFile,                       ///< The #IusInputFile of interest
    char *label,                            ///< The label of the patternList
    iud_t channel,                          ///< The channel data block to be filled
    iuo_t channel_offset                    ///< The offset in the datablock
);

/** \brief Saves a channel of data to an inputFile.
 * \return Returns #IUS_E_OK when succesful or <0 in case of an error.
 */
int iusInputFileResponseSave
(
	iuif_t inputFile,                       ///< The #IusInputFile of interest
	char *label,                            ///< The label of the patternList
	iud_t response,                         ///< the response data to save
	iuo_t response_offset                   ///< The offset in the datablock
);

/** \brief Loads a single response of data from an inputFile.
 * \return Returns #IUS_E_OK when succesful or <0 in case of an error.
 */
int iusInputFileResponseLoad
(
    iuif_t inputFile,                       ///< The #IusInputFile of interest
    char *label,                            ///< The label of the patternList
    iud_t response,                         ///< The response data to load
    iuo_t response_offset                   ///< The offset in the datablock
);

/** \brief Saves a frame of data to an inputFile.
 * \return Returns #IUS_E_OK when succesful or <0 in case of an error.
 */
int iusInputFileFrameSave
(
	iuif_t inputFile,                       ///< The #IusInputFile of interest
	char *label,                            ///< The label of the patternList
	iud_t frame,                            ///< The frame of data
	iuo_t frame_offset                            ///< The offset in the datablock
);

/** \brief Loads a frame of data to an inputFile.
 * \return Returns #IUS_E_OK when succesful or <0 in case of an error.
 */
int iusInputFileFrameLoad
(
	iuif_t inputFile,                       ///< The #IusInputFile of interest
	char *label,                            ///< The label of the patternList
	iud_t frame,                            ///< The frame of data
	iuo_t frame_offset                      ///< The offset in the datablock
);


#endif //IUSLIBRARY_IUSHLINPUTFILE_H
