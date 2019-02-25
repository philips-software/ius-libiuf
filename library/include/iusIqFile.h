// Created by nlv09165 on 11/07/2018.
#ifndef IUSLIBRARY_IUSHLIQFILE_H
#define IUSLIBRARY_IUSHLIQFILE_H

#include <iusAcquisition.h>
#include <iusIqPatternListDict.h>
#include <iusPulseDict.h>
#include <iusReceiveChannelMapDict.h>
#include <iusTransmitApodizationDict.h>
#include <iusFrameList.h>
#include "iusSourceDict.h"
#include "iusDemodulationDict.h"
#include "iusTransducer.h"
#include "iusData.h"
#include "iusOffset.h"

// ADT
typedef struct IusHistoryNode IusIqFile;
/** The iq file is the instance for the "raw" file format with demodulation settings instead of sampling. It contains the 
 * unprocessed ultrasound aquisition. The instance contains a #IusFrameList, a #IusTransducer definition, #IusAcquisition 
 * information and one or more data blocks referering to a patternList. In addition, node history can be added.
 */
typedef IusIqFile *iuiqf_t;

#define IUIQF_INVALID (iuiqf_t) NULL
#define IUS_DEFAULT_NUM_FRAMES  1

/** \brief Create an empty file instance with a filename.
 * \return Returns the empty file instance when succesful or #IUIF_INVALID in case of an error.
 */
iuiqf_t iusIqFileCreate
(
    const char *filename  ///< The filename for the file instance.
);

/** \brief Creates a datablock for a frame for the acquisition with the name \p label.
 *  \return Returns an allocated memory block of a "label"-frame or IUD_INVALID in case of an error.
 */
iud_t iusIqFileFrameCreate
(
    iuiqf_t iqFile,    ///< The #IusIqFile of interest
    char *label             ///< The acquisition type
);

/** \brief Creates a datablock for a response (numSamplesPerLine * numChannels) for the acquisition with
 * the name \p label.
 * \return Returns an allocated memory block of a "label"-response or IUD_INVALID in case of an error.
 */
iud_t iusIqFileResponseCreate
(
    iuiqf_t iqFile,    ///< The #IusIqFile of interest
    char *label             ///< The acquisition type
);

/** \brief Creates a datablock for a single channel (i.e. numSamplesPerLine floats) for the acquisition with
 * the name \p label.
 * \return Returns an allocated memory block of a "label"-channel or IUD_INVALID in case of an error.
 */
iud_t iusIqFileChannelCreate
(
    iuiqf_t iqFile,    ///< The #IusIqFile of interest
    char *label             ///< The acquisition type
);

/** \brief Creates a datablock for a single channel (i.e. numSamplesPerLine floats) for the acquisition with
 * the name \p label.
 * \return Returns an allocated memory block of a "label"-channel or IUD_INVALID in case of an error.
 */
int iusIqFileDelete
(
    iuiqf_t iqFile     ///< The #IusIqFile of interest
);

/** \brief Load an iqFile instance from a file.
 * \return Returns an iq file instance or IUIF_INVALID the file could not be read.
 */
iuiqf_t iusIqFileNodeLoad
(
	const char *pFilename     ///< The filename of the iqFile
);

/** \brief Save (serialize) the iqFile instance to file.
 * \return Returns #IUS_E_OK when succesful or #IUS_ERR_VALUE in case of an error
 */
int iusIqFileNodeSave
(
	iuiqf_t iqFile       ///< The #IusIqFile of interest
);

/** \brief Closes access to the iqFile
 * \return Returns #IUS_E_OK when succesful or #IUS_ERR_VALUE in case of an error
 */
int iusIqFileClose
(
    iuiqf_t iqFile        ///< The #IusIqFile of interest
);

/** \brief Compares two iqFile instances with each other.
 * \return Returns #IUS_TRUE when the iqFiles are equal and #IUS_FALSE otherwise.
 */
int iusIqFileCompare
(
    iuiqf_t reference,        ///< The #IusIqFile to compare to
    iuiqf_t actual            ///< The #IusIqFile to compare with
);

/** \brief Gets the #IusFrameList object from an iqFile.
 * \return Returns the requested #IusFrameList or #IUFL_INVALID in case of an error.
 */
iufl_t iusIqFileGetFrameList
(
	iuiqf_t iqFile       ///< The #IusIqFile of interest
);

/** \brief Gets the #IusPatternListDict object from an iqFile.
 * \return Returns the requested #IusPatternListDict or NULL in case of an error.
 */
iuiqpald_t iusIqFileGetPatternListDict
(
	iuiqf_t iqFile       ///< The #IusIqFile of interest
);

/** \brief Gets the #IusPulseDict object from an iqFile.
 * \return Returns the requested #IusPulseDict or #IUPD_INVALID in case of an error.
 */
iupd_t iusIqFileGetPulseDict
(
	iuiqf_t iqFile       ///< The #IusIqFile of interest
);

/** \brief Gets the #IusSourceDict object from an iqFile.
 * \return Returns the requested #IusSourceDict or #IUSD_INVALID in case of an error.
 */
iusd_t iusIqFileGetSourceDict
(
	iuiqf_t iqFile       ///< The #IusIqFile of interest
);

/** \brief Gets the #IusReceiveChannelMapDict object from an iqFile.
 * \return Returns the requested #IusReceiveChannelMapDict or #IURCMD_INVALID in case of an error.
 */
iurcmd_t iusIqFileGetReceiveChannelMapDict
(
	iuiqf_t iqFile       ///< The #IusIqFile of interest
);

/** \brief Gets the #IusTransmitApodizationDict object from an iqFile.
 * \return Returns the requested #IusTransmitApodizationDict or NULL in case of an error.
 */
iutad_t  iusIqFileGetTransmitApodizationDict
(
	iuiqf_t iqFile       ///< The #IusIqFile of interest
);

/** \brief Gets the #IusReceiveSettingsDict object from an iqFile.
 * \return Returns the requested #IusReceiveSettingsDict or NULL in case of an error.
 */
iudmd_t iusIqFileGetDemodulationDict
(
    iuiqf_t iqFile        ///< The #IusIqFile of interest
);

/** \brief Gets the #IusAcquisition object from an iqFile.
 * \return Returns the requested #IusAcquisition or NULL in case of an error.
 */
iua_t iusIqFileGetAcquisition
(
	iuiqf_t iqFile       ///< The #IusIqFile of interest
);

/** \brief Gets the #IusTransducer object from an iqFile.
 * \return Returns the requested #IusTransducer or NULL in case of an error.
 */
iut_t iusIqFileGetTransducer
(
	iuiqf_t iqFile       ///< The #IusIqFile of interest
);

/** \brief Returns the number of frames an iqFile has.
 * \return Returns the number of frames or 0 if there are no frames, or -1 in case of an invalid argument.
 */
int iusIqFileGetNumFrames
(
    iuiqf_t iqFile        ///< The #IusIqFile of interest
);

/** \brief Gets the number of responses for the patternList type \p label.
 * \return Returns the number of responses (numPatterns) or -1 in case of an error.
 */
int iusIqFileGetNumResponses
(
	iuiqf_t iqFile,          ///< The #IusIqFile of interest
	char *label                ///< the patternList type
);

/** \brief Gets the number of channels for the patternList type \p label.
 * \return Returns the number of channels or -1 in case of an error.
 */
int iusIqFileGetNumChannels
(
    iuiqf_t iqFile,       ///< The #IusIqFile of interest
    char *label                ///< the patternList type
);


/** \brief Gets the number of samples per line for this \p label.
 * \return Returns the number of channels or -1 in case of an error.
 */
int iusIqFileGetSamplesPerLine
(
	iuiqf_t iqFile,       ///< The #IusIqFile of interest
	char *label             ///< the patternList type
);

// Setters
/** \brief Sets the frameList of an iqFile.
 * \return Returns #IUS_E_OK when succesful or #IUS_ERR_VALUE in case of an error.
 */
int iusIqFileSetFrameList
(
	iuiqf_t iqFile,          ///< The #IusIqFile of interest
	iufl_t frameList           ///< The #IusFrameList to set
);

/** \brief Sets the patternListDict of an iqFile.
 * \return Returns #IUS_E_OK when succesful or #IUS_ERR_VALUE in case of an error.
 */
int iusIqFileSetPatternListDict
(
	iuiqf_t iqFile,          ///< The #IusIqFile of interest
	iuiqpald_t iqPatternListDict   ///< The #IusPatternListDict to set
);

/** \brief Sets the pulseDict of an iqFile.
 * \return Returns #IUS_E_OK when succesful or #IUS_ERR_VALUE in case of an error.
 */
int iusIqFileSetPulseDict
(
	iuiqf_t iqFile,         ///< The #IusIqFile of interest
	iupd_t pulseDict          ///< The #IusPulseDict to set
);

/** \brief Sets the sourceDict of an iqFile.
 * \return Returns #IUS_E_OK when succesful or #IUS_ERR_VALUE in case of an error.
 */
int iusIqFileSetSourceDict
(
    iuiqf_t iqFile,                       ///< The #IusIqFile of interest
    iusd_t sourceDict                       ///< The #IusSourceDict to set
);

/** \brief Sets the receiveChannelMap of an iqFile.
 * \return Returns #IUS_E_OK when succesful or #IUS_ERR_VALUE in case of an error.
 */
int iusIqFileSetReceiveChannelMapDict
(
    iuiqf_t iqFile,                       ///< The #IusIqFile of interest
    iurcmd_t receiveChannelMapDict          ///< The #IusReceiveChannelMapDict to set
);

/** \brief Sets the transmitApodizationDict of an iqFile.
 * \return Returns #IUS_E_OK when succesful or #IUS_ERR_VALUE in case of an error.
 */
int iusIqFileSetTransmitApodizationDict
(
	iuiqf_t iqFile,                       ///< The #IusIqFile of interest
	iutad_t transmitApodizationDict         ///< The #IusTransmitApodizationDict to set
);

/** \brief Sets the receiveSettingsDict of an iqFile.
 * \return Returns #IUS_E_OK when succesful or #IUS_ERR_VALUE in case of an error.
 */
int iusIqFileSetDemodulationDict
(
    iuiqf_t iqFile,                       ///< The #IusIqFile of interest
    iudmd_t demodulationDict             ///< The #IusTransmitApodizationDict to set
);

/** \brief Sets the acquisition info of an iqFile.
 * \return Returns #IUS_E_OK when succesful or #IUS_ERR_VALUE in case of an error.
 */
int iusIqFileSetAcquisition
(
	iuiqf_t iqFile,                       ///< The #IusIqFile of interest
	iua_t acquisition                       ///< The #IusAcquisition to set
);

/** \brief Sets the transducer of an iqFile.
 * \return Returns #IUS_E_OK when succesful or #IUS_ERR_VALUE in case of an error.
 */
int iusIqFileSetTransducer
(
	iuiqf_t iqFile,                       ///< The #IusIqFile of interest
	iut_t  transducer                       ///< The #IusTransducer to set
);

/** \brief Saves a channel of data to an iqFile.
 * \return Returns #IUS_E_OK when succesful or <0 in case of an error.
 */
int iusIqFileChannelSave
(
    iuiqf_t iqFile,                       ///< The #IusIqFile of interest
    char *label,                            ///< The label of the patternList
    iud_t channel,                          ///< The channel data to save
    iuo_t channel_offset                    ///< The offsets in the datablock
);

/** \brief Loads a single channel of data from an iqFile.
 * \return Returns #IUS_E_OK when succesful or <0 in case of an error.
 */
int iusIqFileChannelLoad
(
    iuiqf_t iqFile,                       ///< The #IusIqFile of interest
    char *label,                            ///< The label of the patternList
    iud_t channel,                          ///< The channel data block to be filled
    iuo_t channel_offset                    ///< The offset in the datablock
);

/** \brief Saves a channel of data to an iqFile.
 * \return Returns #IUS_E_OK when succesful or <0 in case of an error.
 */
int iusIqFileResponseSave
(
	iuiqf_t iqFile,                       ///< The #IusIqFile of interest
	char *label,                            ///< The label of the patternList
	iud_t response,                         ///< the response data to save
	iuo_t response_offset                   ///< The offset in the datablock
);

/** \brief Loads a single response of data from an iqFile.
 * \return Returns #IUS_E_OK when succesful or <0 in case of an error.
 */
int iusIqFileResponseLoad
(
    iuiqf_t iqFile,                       ///< The #IusIqFile of interest
    char *label,                            ///< The label of the patternList
    iud_t response,                         ///< The response data to load
    iuo_t response_offset                   ///< The offset in the datablock
);

/** \brief Saves a frame of data to an iqFile.
 * \return Returns #IUS_E_OK when succesful or <0 in case of an error.
 */
int iusIqFileFrameSave
(
	iuiqf_t iqFile,                       ///< The #IusIqFile of interest
	char *label,                            ///< The label of the patternList
	iud_t frame,                            ///< The frame of data
	iuo_t frame_offset                            ///< The offset in the datablock
);

/** \brief Loads a frame of data to an iqFile.
 * \return Returns #IUS_E_OK when succesful or <0 in case of an error.
 */
int iusIqFileFrameLoad
(
	iuiqf_t iqFile,                       ///< The #IusIqFile of interest
	char *label,                            ///< The label of the patternList
	iud_t frame,                            ///< The frame of data
	iuo_t frame_offset                      ///< The offset in the datablock
);


#endif //IUSLIBRARY_IUSHLIQFILE_H
