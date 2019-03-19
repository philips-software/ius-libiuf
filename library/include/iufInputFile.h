// Created by nlv09165 on 11/07/2018.
#ifndef IUFLIBRARY_IUFHLINPUTFILE_H
#define IUFLIBRARY_IUFHLINPUTFILE_H

#include <iufAcquisition.h>
#include <iufPatternListDict.h>
#include <iufPulseDict.h>
#include <iufReceiveChannelMapDict.h>
#include <iufTransmitApodizationDict.h>
#include <iufFrameList.h>
#include "iufSourceDict.h"
#include "iufReceiveSettingsDict.h"
#include "iufTransducer.h"
#include "iufData.h"
#include "iufOffset.h"

// ADT
typedef struct IufHistoryNode IufInputFile;
/** The input file is the instance for the "raw" file format. It contains the unprocessed ultrasound aquisition.
 * The instance contains a #IufFrameList, a #IufTransducer definition, #IufAcquisition information and one or more data blocks
 * referering to a patternList. In addition, node history can be added.
 */
typedef IufInputFile *iuif_t;

#define IUIF_INVALID (iuif_t) NULL
#define IUF_DEFAULT_NUM_FRAMES  1

/** \brief Create an empty file instance with a filename.
 * \return Returns the empty file instance when succesful or #IUIF_INVALID in case of an error.
 */
iuif_t iufInputFileCreate
(
    const char *filename  ///< The filename for the file instance.
);

/** \brief Creates a datablock for a frame for the acquisition with the name \p label.
 *  \return Returns an allocated memory block of a "label"-frame or IUD_INVALID in case of an error.
 */
iud_t iufInputFileFrameCreate
(
    iuif_t inputFile,    ///< The #IufInputFile of interest
    char *label             ///< The acquisition type
);

/** \brief Creates a datablock for a response (numSamplesPerLine * numChannels) for the acquisition with
 * the name \p label.
 * \return Returns an allocated memory block of a "label"-response or IUD_INVALID in case of an error.
 */
iud_t iufInputFileResponseCreate
(
    iuif_t inputFile,    ///< The #IufInputFile of interest
    char *label             ///< The acquisition type
);

/** \brief Creates a datablock for a single channel (i.e. numSamplesPerLine floats) for the acquisition with
 * the name \p label.
 * \return Returns an allocated memory block of a "label"-channel or IUD_INVALID in case of an error.
 */
iud_t iufInputFileChannelCreate
(
    iuif_t inputFile,    ///< The #IufInputFile of interest
    char *label             ///< The acquisition type
);

/** \brief Creates a datablock for a single channel (i.e. numSamplesPerLine floats) for the acquisition with
 * the name \p label.
 * \return Returns an allocated memory block of a "label"-channel or IUD_INVALID in case of an error.
 */
int iufInputFileDelete
(
    iuif_t inputFile     ///< The #IufInputFile of interest
);

/** \brief Load an inputFile instance from a file.
 * \return Returns an input file instance or IUIF_INVALID the file could not be read.
 */
iuif_t iufInputFileNodeLoad
(
	const char *pFilename     ///< The filename of the inputFile
);

/** \brief Save (serialize) the inputFile instance to file.
 * \return Returns #IUF_E_OK when succesful or #IUF_ERR_VALUE in case of an error
 */
int iufInputFileNodeSave
(
	iuif_t inputFile       ///< The #IufInputFile of interest
);

/** \brief Closes access to the inputFile
 * \return Returns #IUF_E_OK when succesful or #IUF_ERR_VALUE in case of an error
 */
int iufInputFileClose
(
    iuif_t inputFile        ///< The #IufInputFile of interest
);

/** \brief Compares two inputFile instances with each other.
 * \return Returns #IUF_TRUE when the inputFiles are equal and #IUF_FALSE otherwise.
 */
int iufInputFileCompare
(
    iuif_t reference,        ///< The #IufInputFile to compare to
    iuif_t actual            ///< The #IufInputFile to compare with
);

/** \brief Gets the #IufFrameList object from an inputFile.
 * \return Returns the requested #IufFrameList or #IUFL_INVALID in case of an error.
 */
iufl_t iufInputFileGetFrameList
(
	iuif_t inputFile       ///< The #IufInputFile of interest
);

/** \brief Gets the #IufPatternListDict object from an inputFile.
 * \return Returns the requested #IufPatternListDict or NULL in case of an error.
 */
iupald_t iufInputFileGetPatternListDict
(
	iuif_t inputFile       ///< The #IufInputFile of interest
);

/** \brief Gets the #IufPulseDict object from an inputFile.
 * \return Returns the requested #IufPulseDict or #IUPD_INVALID in case of an error.
 */
iupd_t iufInputFileGetPulseDict
(
	iuif_t inputFile       ///< The #IufInputFile of interest
);

/** \brief Gets the #IufSourceDict object from an inputFile.
 * \return Returns the requested #IufSourceDict or #IUFD_INVALID in case of an error.
 */
iusd_t iufInputFileGetSourceDict
(
	iuif_t inputFile       ///< The #IufInputFile of interest
);

/** \brief Gets the #IufReceiveChannelMapDict object from an inputFile.
 * \return Returns the requested #IufReceiveChannelMapDict or #IURCMD_INVALID in case of an error.
 */
iurcmd_t iufInputFileGetReceiveChannelMapDict
(
	iuif_t inputFile       ///< The #IufInputFile of interest
);

/** \brief Gets the #IufTransmitApodizationDict object from an inputFile.
 * \return Returns the requested #IufTransmitApodizationDict or NULL in case of an error.
 */
iutad_t  iufInputFileGetTransmitApodizationDict
(
	iuif_t inputFile       ///< The #IufInputFile of interest
);

/** \brief Gets the #IufReceiveSettingsDict object from an inputFile.
 * \return Returns the requested #IufReceiveSettingsDict or NULL in case of an error.
 */
iursd_t iufInputFileGetReceiveSettingsDict
(
    iuif_t inputFile        ///< The #IufInputFile of interest
);

/** \brief Gets the #IufAcquisition object from an inputFile.
 * \return Returns the requested #IufAcquisition or NULL in case of an error.
 */
iua_t iufInputFileGetAcquisition
(
	iuif_t inputFile       ///< The #IufInputFile of interest
);

/** \brief Gets the #IufTransducer object from an inputFile.
 * \return Returns the requested #IufTransducer or NULL in case of an error.
 */
iut_t iufInputFileGetTransducer
(
	iuif_t inputFile       ///< The #IufInputFile of interest
);

/** \brief Returns the number of frames an inputFile has.
 * \return Returns the number of frames or 0 if there are no frames, or -1 in case of an invalid argument.
 */
int iufInputFileGetNumFrames
(
    iuif_t inputFile        ///< The #IufInputFile of interest
);

/** \brief Gets the number of responses for the patternList type \p label.
 * \return Returns the number of responses (numPatterns) or -1 in case of an error.
 */
int iufInputFileGetNumResponses
(
	iuif_t inputFile,          ///< The #IufInputFile of interest
	char *label                ///< the patternList type
);

/** \brief Gets the number of channels for the patternList type \p label.
 * \return Returns the number of channels or -1 in case of an error.
 */
int iufInputFileGetNumChannels
(
    iuif_t inputFile,       ///< The #IufInputFile of interest
    char *label                ///< the patternList type
);


/** \brief Gets the number of samples per line for this \p label.
 * \return Returns the number of channels or -1 in case of an error.
 */
int iufInputFileGetSamplesPerLine
(
	iuif_t inputFile,       ///< The #IufInputFile of interest
	char *label             ///< the patternList type
);

// Setters
/** \brief Sets the frameList of an inputFile.
 * \return Returns #IUF_E_OK when succesful or #IUF_ERR_VALUE in case of an error.
 */
int iufInputFileSetFrameList
(
	iuif_t inputFile,          ///< The #IufInputFile of interest
	iufl_t frameList           ///< The #IufFrameList to set
);

/** \brief Sets the patternListDict of an inputFile.
 * \return Returns #IUF_E_OK when succesful or #IUF_ERR_VALUE in case of an error.
 */
int iufInputFileSetPatternListDict
(
	iuif_t inputFile,          ///< The #IufInputFile of interest
	iupald_t patternListDict   ///< The #IufPatternListDict to set
);

/** \brief Sets the pulseDict of an inputFile.
 * \return Returns #IUF_E_OK when succesful or #IUF_ERR_VALUE in case of an error.
 */
int iufInputFileSetPulseDict
(
	iuif_t inputFile,         ///< The #IufInputFile of interest
	iupd_t pulseDict          ///< The #IufPulseDict to set
);

/** \brief Sets the sourceDict of an inputFile.
 * \return Returns #IUF_E_OK when succesful or #IUF_ERR_VALUE in case of an error.
 */
int iufInputFileSetSourceDict
(
    iuif_t inputFile,                       ///< The #IufInputFile of interest
    iusd_t sourceDict                       ///< The #IufSourceDict to set
);

/** \brief Sets the receiveChannelMap of an inputFile.
 * \return Returns #IUF_E_OK when succesful or #IUF_ERR_VALUE in case of an error.
 */
int iufInputFileSetReceiveChannelMapDict
(
    iuif_t inputFile,                       ///< The #IufInputFile of interest
    iurcmd_t receiveChannelMapDict          ///< The #IufReceiveChannelMapDict to set
);

/** \brief Sets the transmitApodizationDict of an inputFile.
 * \return Returns #IUF_E_OK when succesful or #IUF_ERR_VALUE in case of an error.
 */
int iufInputFileSetTransmitApodizationDict
(
	iuif_t inputFile,                       ///< The #IufInputFile of interest
	iutad_t transmitApodizationDict         ///< The #IufTransmitApodizationDict to set
);

/** \brief Sets the receiveSettingsDict of an inputFile.
 * \return Returns #IUF_E_OK when succesful or #IUF_ERR_VALUE in case of an error.
 */
int iufInputFileSetReceiveSettingsDict
(
    iuif_t inputFile,                       ///< The #IufInputFile of interest
    iursd_t receiveSettingsDict             ///< The #IufTransmitApodizationDict to set
);

/** \brief Sets the acquisition info of an inputFile.
 * \return Returns #IUF_E_OK when succesful or #IUF_ERR_VALUE in case of an error.
 */
int iufInputFileSetAcquisition
(
	iuif_t inputFile,                       ///< The #IufInputFile of interest
	iua_t acquisition                       ///< The #IufAcquisition to set
);

/** \brief Sets the transducer of an inputFile.
 * \return Returns #IUF_E_OK when succesful or #IUF_ERR_VALUE in case of an error.
 */
int iufInputFileSetTransducer
(
	iuif_t inputFile,                       ///< The #IufInputFile of interest
	iut_t  transducer                       ///< The #IufTransducer to set
);

/** \brief Saves a channel of data to an inputFile.
 * \return Returns #IUF_E_OK when succesful or <0 in case of an error.
 */
int iufInputFileChannelSave
(
    iuif_t inputFile,                       ///< The #IufInputFile of interest
    char *label,                            ///< The label of the patternList
    iud_t channel,                          ///< The channel data to save
    iuo_t channel_offset                    ///< The offsets in the datablock
);

/** \brief Loads a single channel of data from an inputFile.
 * \return Returns #IUF_E_OK when succesful or <0 in case of an error.
 */
int iufInputFileChannelLoad
(
    iuif_t inputFile,                       ///< The #IufInputFile of interest
    char *label,                            ///< The label of the patternList
    iud_t channel,                          ///< The channel data block to be filled
    iuo_t channel_offset                    ///< The offset in the datablock
);

/** \brief Saves a channel of data to an inputFile.
 * \return Returns #IUF_E_OK when succesful or <0 in case of an error.
 */
int iufInputFileResponseSave
(
	iuif_t inputFile,                       ///< The #IufInputFile of interest
	char *label,                            ///< The label of the patternList
	iud_t response,                         ///< the response data to save
	iuo_t response_offset                   ///< The offset in the datablock
);

/** \brief Loads a single response of data from an inputFile.
 * \return Returns #IUF_E_OK when succesful or <0 in case of an error.
 */
int iufInputFileResponseLoad
(
    iuif_t inputFile,                       ///< The #IufInputFile of interest
    char *label,                            ///< The label of the patternList
    iud_t response,                         ///< The response data to load
    iuo_t response_offset                   ///< The offset in the datablock
);

/** \brief Saves a frame of data to an inputFile.
 * \return Returns #IUF_E_OK when succesful or <0 in case of an error.
 */
int iufInputFileFrameSave
(
	iuif_t inputFile,                       ///< The #IufInputFile of interest
	char *label,                            ///< The label of the patternList
	iud_t frame,                            ///< The frame of data
	iuo_t frame_offset                            ///< The offset in the datablock
);

/** \brief Loads a frame of data to an inputFile.
 * \return Returns #IUF_E_OK when succesful or <0 in case of an error.
 */
int iufInputFileFrameLoad
(
	iuif_t inputFile,                       ///< The #IufInputFile of interest
	char *label,                            ///< The label of the patternList
	iud_t frame,                            ///< The frame of data
	iuo_t frame_offset                      ///< The offset in the datablock
);


#endif //IUFLIBRARY_IUFHLINPUTFILE_H
