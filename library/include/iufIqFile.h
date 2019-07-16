#ifndef IUFLIBRARY_IUFHLIQFILE_H
#define IUFLIBRARY_IUFHLIQFILE_H

#include <iufAcquisition.h>
#include <iufIqPatternListDict.h>
#include <iufPulseDict.h>
#include <iufReceiveChannelMapDict.h>
#include <iufTransmitApodizationDict.h>
#include <iufFrameList.h>
#include "iufSourceDict.h"
#include "iufDemodulationDict.h"
#include "iufTransducer.h"
#include "iufData.h"
#include "iufOffset.h"

// ADT
typedef struct IufHistoryNode IufIqFile;
/** The iq file is the instance for the "raw" file format with demodulation settings instead of sampling. It contains the 
 * unprocessed ultrasound aquisition. The instance contains a #IufFrameList, a #IufTransducer definition, #IufAcquisition
 * information and one or more data blocks referering to a patternList. In addition, node history can be added.
 */
typedef IufIqFile *iuiqf_t;

#define IUIQF_INVALID (iuiqf_t) NULL
#define IUF_DEFAULT_NUM_FRAMES  1

/** \brief Create an empty file instance with a filename.
 * \return Returns the empty file instance when succesful or #IUIF_INVALID in case of an error.
 */
iuiqf_t iufIqFileCreate
(
    const char *filename  ///< The filename for the file instance.
);

/** \brief Creates a datablock for the I or Q Component of a frame for the acquisition with the name \p label. Typically called on both components.
 *  \return Returns an allocated memory block of a "label"-frame or IUD_INVALID in case of an error.
 */
iud_t iufIqFileFrameCreate
(
    iuiqf_t iqFile,    ///< The #IufIqFile of interest
    char *label             ///< The acquisition type
);


/** \brief Creates a datablock for a response (numSamplesPerLine * numChannels) for the acquisition with
 * the name \p label. Typically called on both components.
 * \return Returns an allocated memory block of a "label"-response or IUD_INVALID in case of an error.
 */
iud_t iufIqFileResponseCreate
(
    iuiqf_t iqFile,    ///< The #IufIqFile of interest
    char *label             ///< The acquisition type
);

/** \brief Creates a datablock for an single I channel (i.e. numSamplesPerLine floats) for the acquisition with
 * the name \p label. Typically called on both components.
 * \return Returns an allocated memory block of a "label"-channel or IUD_INVALID in case of an error.
 */
iud_t iufIqFileChannelCreate
(
    iuiqf_t iqFile,    ///< The #IufIqFile of interest
    char *label             ///< The acquisition type
);

/** \brief Creates a datablock for a single channel (i.e. numSamplesPerLine floats) for the acquisition with
 * the name \p label.
 * \return Returns an allocated memory block of a "label"-channel or IUD_INVALID in case of an error.
 */
int iufIqFileDelete
(
    iuiqf_t iqFile     ///< The #IufIqFile of interest
);

/** \brief Load an iqFile instance from a file.
 * \return Returns an iq file instance or IUIF_INVALID the file could not be read.
 */
iuiqf_t iufIqFileNodeLoad
(
	const char *pFilename     ///< The filename of the iqFile
);

/** \brief Save (serialize) the iqFile instance to file.
 * \return Returns #IUF_E_OK when succesful or #IUF_ERR_VALUE in case of an error
 */
int iufIqFileNodeSave
(
	iuiqf_t iqFile       ///< The #IufIqFile of interest
);

/** \brief Closes access to the iqFile
 * \return Returns #IUF_E_OK when succesful or #IUF_ERR_VALUE in case of an error
 */
int iufIqFileClose
(
    iuiqf_t iqFile        ///< The #IufIqFile of interest
);

/** \brief Compares two iqFile instances with each other.
 * \return Returns #IUF_TRUE when the iqFiles are equal and #IUF_FALSE otherwise.
 */
int iufIqFileCompare
(
    iuiqf_t reference,        ///< The #IufIqFile to compare to
    iuiqf_t actual            ///< The #IufIqFile to compare with
);

/** \brief Gets the #IufFrameList object from an iqFile.
 * \return Returns the requested #IufFrameList or #IUFL_INVALID in case of an error.
 */
iufl_t iufIqFileGetFrameList
(
	iuiqf_t iqFile       ///< The #IufIqFile of interest
);

/** \brief Gets the #IufPatternListDict object from an iqFile.
 * \return Returns the requested #IufPatternListDict or NULL in case of an error.
 */
iuiqpald_t iufIqFileGetIqPatternListDict
(
	iuiqf_t iqFile       ///< The #IufIqFile of interest
);

/** \brief Gets the #IufPulseDict object from an iqFile.
 * \return Returns the requested #IufPulseDict or #IUPD_INVALID in case of an error.
 */
iupd_t iufIqFileGetPulseDict
(
	iuiqf_t iqFile       ///< The #IufIqFile of interest
);

/** \brief Gets the #IufSourceDict object from an iqFile.
 * \return Returns the requested #IufSourceDict or #IUFD_INVALID in case of an error.
 */
iusd_t iufIqFileGetSourceDict
(
	iuiqf_t iqFile       ///< The #IufIqFile of interest
);

/** \brief Gets the #IufReceiveChannelMapDict object from an iqFile.
 * \return Returns the requested #IufReceiveChannelMapDict or #IURCMD_INVALID in case of an error.
 */
iurcmd_t iufIqFileGetReceiveChannelMapDict
(
	iuiqf_t iqFile       ///< The #IufIqFile of interest
);

/** \brief Gets the #IufTransmitApodizationDict object from an iqFile.
 * \return Returns the requested #IufTransmitApodizationDict or NULL in case of an error.
 */
iutad_t  iufIqFileGetTransmitApodizationDict
(
	iuiqf_t iqFile       ///< The #IufIqFile of interest
);

/** \brief Gets the #IufReceiveSettingsDict object from an iqFile.
 * \return Returns the requested #IufReceiveSettingsDict or NULL in case of an error.
 */
iudmd_t iufIqFileGetDemodulationDict
(
    iuiqf_t iqFile        ///< The #IufIqFile of interest
);

/** \brief Gets the #IufAcquisition object from an iqFile.
 * \return Returns the requested #IufAcquisition or NULL in case of an error.
 */
iua_t iufIqFileGetAcquisition
(
	iuiqf_t iqFile       ///< The #IufIqFile of interest
);

/** \brief Gets the #IufTransducer object from an iqFile.
 * \return Returns the requested #IufTransducer or NULL in case of an error.
 */
iut_t iufIqFileGetTransducer
(
	iuiqf_t iqFile       ///< The #IufIqFile of interest
);

/** \brief Returns the number of frames an iqFile has.
 * \return Returns the number of frames or 0 if there are no frames, or -1 in case of an invalid argument.
 */
int iufIqFileGetNumFrames
(
    iuiqf_t iqFile        ///< The #IufIqFile of interest
);

/** \brief Gets the number of responses for the patternList type \p label.
 * \return Returns the number of responses (numPatterns) or -1 in case of an error.
 */
int iufIqFileGetNumResponses
(
	iuiqf_t iqFile,          ///< The #IufIqFile of interest
	char *label                ///< the patternList type
);

/** \brief Gets the number of channels for the patternList type \p label.
 * \return Returns the number of channels or -1 in case of an error.
 */
int iufIqFileGetNumChannels
(
    iuiqf_t iqFile,       ///< The #IufIqFile of interest
    char *label                ///< the patternList type
);


/** \brief Gets the number of samples per line for this \p label.
 * \return Returns the number of channels or -1 in case of an error.
 */
int iufIqFileGetSamplesPerLine
(
	iuiqf_t iqFile,       ///< The #IufIqFile of interest
	char *label             ///< the patternList type
);

// Setters
/** \brief Sets the frameList of an iqFile.
 * \return Returns #IUF_E_OK when succesful or #IUF_ERR_VALUE in case of an error.
 */
int iufIqFileSetFrameList
(
	iuiqf_t iqFile,          ///< The #IufIqFile of interest
	iufl_t frameList           ///< The #IufFrameList to set
);

/** \brief Sets the patternListDict of an iqFile.
 * \return Returns #IUF_E_OK when succesful or #IUF_ERR_VALUE in case of an error.
 */
int iufIqFileSetPatternListDict
(
	iuiqf_t iqFile,          ///< The #IufIqFile of interest
	iuiqpald_t iqPatternListDict   ///< The #IufPatternListDict to set
);

/** \brief Sets the pulseDict of an iqFile.
 * \return Returns #IUF_E_OK when succesful or #IUF_ERR_VALUE in case of an error.
 */
int iufIqFileSetPulseDict
(
	iuiqf_t iqFile,         ///< The #IufIqFile of interest
	iupd_t pulseDict          ///< The #IufPulseDict to set
);

/** \brief Sets the sourceDict of an iqFile.
 * \return Returns #IUF_E_OK when succesful or #IUF_ERR_VALUE in case of an error.
 */
int iufIqFileSetSourceDict
(
    iuiqf_t iqFile,                       ///< The #IufIqFile of interest
    iusd_t sourceDict                       ///< The #IufSourceDict to set
);

/** \brief Sets the receiveChannelMap of an iqFile.
 * \return Returns #IUF_E_OK when succesful or #IUF_ERR_VALUE in case of an error.
 */
int iufIqFileSetReceiveChannelMapDict
(
    iuiqf_t iqFile,                       ///< The #IufIqFile of interest
    iurcmd_t receiveChannelMapDict          ///< The #IufReceiveChannelMapDict to set
);

/** \brief Sets the transmitApodizationDict of an iqFile.
 * \return Returns #IUF_E_OK when succesful or #IUF_ERR_VALUE in case of an error.
 */
int iufIqFileSetTransmitApodizationDict
(
	iuiqf_t iqFile,                       ///< The #IufIqFile of interest
	iutad_t transmitApodizationDict         ///< The #IufTransmitApodizationDict to set
);

/** \brief Sets the receiveSettingsDict of an iqFile.
 * \return Returns #IUF_E_OK when succesful or #IUF_ERR_VALUE in case of an error.
 */
int iufIqFileSetDemodulationDict
(
    iuiqf_t iqFile,                       ///< The #IufIqFile of interest
    iudmd_t demodulationDict             ///< The #IufTransmitApodizationDict to set
);

/** \brief Sets the acquisition info of an iqFile.
 * \return Returns #IUF_E_OK when succesful or #IUF_ERR_VALUE in case of an error.
 */
int iufIqFileSetAcquisition
(
	iuiqf_t iqFile,                       ///< The #IufIqFile of interest
	iua_t acquisition                       ///< The #IufAcquisition to set
);

/** \brief Sets the transducer of an iqFile.
 * \return Returns #IUF_E_OK when succesful or #IUF_ERR_VALUE in case of an error.
 */
int iufIqFileSetTransducer
(
	iuiqf_t iqFile,                       ///< The #IufIqFile of interest
	iut_t  transducer                       ///< The #IufTransducer to set
);

/** \brief Saves a channel of data to an iqFile.
 * \return Returns #IUF_E_OK when succesful or <0 in case of an error.
 */
int iufIqFileChannelSave
(
    iuiqf_t iqFile,                       ///< The #IufIqFile of interest
    char *label,                            ///< The label of the patternList
    IufComponent component,                 ///< Is it an I or a Q component?
    iud_t channel,                          ///< The channel data to save
    iuo_t channel_offset                    ///< The offsets in the datablock
);

/** \brief Loads a single channel of data from an iqFile.
 * \return Returns #IUF_E_OK when succesful or <0 in case of an error.
 */
int iufIqFileChannelLoad
(
    iuiqf_t iqFile,                       ///< The #IufIqFile of interest
    char *label,                            ///< The label of the patternList
    IufComponent component,                 ///< Is it an I or a Q component?
    iud_t channel,                          ///< The channel data block to be filled
    iuo_t channel_offset                    ///< The offset in the datablock
);

/** \brief Saves a channel of data to an iqFile.
 * \return Returns #IUF_E_OK when succesful or <0 in case of an error.
 */
int iufIqFileResponseSave
(
	iuiqf_t iqFile,                       ///< The #IufIqFile of interest
	char *label,                            ///< The label of the patternList
    IufComponent component,                 ///< Is it an I or a Q component?
	iud_t response,                         ///< the response data to save
	iuo_t response_offset                   ///< The offset in the datablock
);

/** \brief Loads a single response of data from an iqFile.
 * \return Returns #IUF_E_OK when succesful or <0 in case of an error.
 */
int iufIqFileResponseLoad
(
    iuiqf_t iqFile,                       ///< The #IufIqFile of interest
    char *label,                            ///< The label of the patternList
    IufComponent component,                 ///< Is it an I or a Q component?
    iud_t response,                         ///< The response data to load
    iuo_t response_offset                   ///< The offset in the datablock
);

/** \brief Saves a frame of data to an iqFile.
 * \return Returns #IUF_E_OK when succesful or <0 in case of an error.
 */
int iufIqFileFrameSave
(
	iuiqf_t iqFile,                         ///< The #IufIqFile of interest
	char *label,                            ///< The label of the patternList
    IufComponent component,                 ///< Is it an I or a Q component?
	iud_t frame,                            ///< The frame of data
	iuo_t frame_offset                      ///< The offset in the datablock
);

/** \brief Loads a frame of data to an iqFile.
 * \return Returns #IUF_E_OK when succesful or <0 in case of an error.
 */
int iufIqFileFrameLoad
(
	iuiqf_t iqFile,                       ///< The #IufIqFile of interest
	char *label,                            ///< The label of the patternList
    IufComponent component,                 ///< Is it an I or a Q component?
	iud_t frame,                            ///< The frame of data
	iuo_t frame_offset                      ///< The offset in the datablock
);


#endif //IUFLIBRARY_IUFHLIQFILE_H
