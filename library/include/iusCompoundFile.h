// Created by nlv09165 on 11/07/2018.
#ifndef IUSLIBRARY_IUSHLCOMPOUNDFILE_H
#define IUSLIBRARY_IUSHLCOMPOUNDFILE_H

#include "iusData.h"
#include "iusOffset.h"
#include <iusCompoundWaveListDict.h>
#include <iusPulseDict.h>
#include <iusReceiveChannelMapDict.h>
#include <iusReceiveApodizationDict.h>
#include <iusCompoundFrameList.h>
#include "iusSourceDict.h"

// ADT
typedef struct IusHistoryNode IusCompoundFile;
typedef IusCompoundFile *iucf_t;

#define IUCF_INVALID (iucf_t) NULL
#define IUS_DEFAULT_NUM_COMPOUND_FRAMES  1

/** \brief Create an empty file instance with a filename.
* \return Returns the empty file instance when succesful or #IUCF_INVALID in case of an error.
*/
iucf_t iusCompoundFileCreate
(
	const char *filename  ///< The filename for the file instance.
);

/** \brief Creates a datablock for a single channel (i.e. numSamplesPerLine floats) for the acquisition with
* the name \p label.
* \return Returns an allocated memory block of a "label"-channel or IUD_INVALID in case of an error.
*/
int iusCompoundFileDelete
(
	iucf_t inputFile     ///< The #IusCompoundFile of interest
);

/** \brief Load an inputFile instance from a file.
* \return Returns an input file instance or IUIF_INVALID the file could not be read.
*/
iucf_t iusCompoundFileNodeLoad
(
	const char *pFilename     ///< The filename of the inputFile
);

/** \brief Save (serialize) the inputFile instance to file.
* \return Returns #IUS_E_OK when succesful or #IUS_ERR_VALUE in case of an error
*/
int iusCompoundFileNodeSave
(
	iucf_t inputFile       ///< The #IusCompoundFile of interest
);

/** \brief Closes access to the inputFile
* \return Returns #IUS_E_OK when succesful or #IUS_ERR_VALUE in case of an error
*/
int iusCompoundFileClose
(
	iucf_t inputFile        ///< The #IusCompoundFile of interest
);

/** \brief Compares two inputFile instances with each other.
* \return Returns #IUS_TRUE when the inputFiles are equal and #IUS_FALSE otherwise.
*/
int iusCompoundFileCompare
(
	iucf_t reference,        ///< The #IusCompoundFile to compare to
	iucf_t actual            ///< The #IusCompoundFile to compare with
);

/** \brief Gets the #IusCompoundFrameList object from a #IusCompoundFile.
 * \return Returns the requested #IusCompoundFrameList or #IUCFRL_INVALID in case of an error.
 */
iucfrl_t iusCompoundFileGetFrameList
(
	iucf_t compoundFile       ///< The #IusCompoundFile of interest
);

/** \brief Gets the #IusCompoundWaveListDict object from a compoundFile.
 * \return Returns the requested #IusPatternListDict or NULL in case of an error.
 */
iucwld_t iusCompoundFileGetCompoundWaveListDict
(
	iucf_t compoundFile       ///< The #IusCompoundFile of interest
);

/** \brief Gets the #IusPulseDict object from a compoundFile.
 * \return Returns the requested #IusPulseDict or #IUPD_INVALID in case of an error.
 */
iupd_t iusCompoundFileGetPulseDict
(
	iucf_t compoundFile       ///< The #IusCompoundFile of interest
);

/** \brief Gets the #IusSourceDict object from a compoundFile.
 * \return Returns the requested #IusSourceDict or #IUSD_INVALID in case of an error.
 */
iusd_t iusCompoundFileGetSourceDict
(
	iucf_t compoundFile       ///< The #IusCompoundFile of interest
);

/** \brief Gets the #IusReceiveChannelMapDict object from a compoundFile.
 * \return Returns the requested #IusReceiveChannelMapDict or #IURCMD_INVALID in case of an error.
 */
iurcmd_t iusCompoundFileGetReceiveChannelMapDict
(
	iucf_t compoundFile       ///< The #IusCompoundFile of interest
);

/** \brief Gets the #IusReceiveApodizationDict object from a compoundFile.
 * \return Returns the requested #IusReceiveApodizationDict or NULL in case of an error.
 */
iurad_t  iusCompoundFileGetReceiveApodizationDict
(
	iucf_t compoundFile       ///< The #IusCompoundFile of interest
);

/** \brief Returns the number of frames an compoundFile has.
 * \return Returns the number of frames or 0 if there are no frames, or -1 in case of an invalid argument.
 */
int iusCompoundFileGetNumFrames
(
    iucf_t compoundFile        ///< The #IusCompoundFile of interest
);

/** \brief Gets the number of waves of the #IusCompoundWaveList with label \p label.
 * \return Returns the number of compound waves or -1 in case of an error.
 */
int iusCompoundFileGetNumCompoundWaves
(
	iucf_t compoundFile,       ///< The #IusCompoundFile of interest
	char *label                ///< the #IusCompoundWaveList form the dictionary 
);

/** \brief Gets the number of channels for the compoundWave type \p label.
 * \return Returns the number of channels or -1 in case of an error.
 */
int iusCompoundFileGetNumChannels
(
    iucf_t compoundFile,       ///< The #IusCompoundFile of interest
    char *label                ///< the ReceiveChannelMap label
);

/** \brief Gets the number of samples per line of a compoundWave of type \p label.
 * \return Returns the number of samples or -1 in case of an error.
 */
int iusCompoundFileGetSamplesPerLine
(
	iucf_t compoundFile,       ///< The #IusCompoundFile of interest
	char *label                ///< the compoundWave type
);

// Setters
/** \brief Sets the #IusCompoundFrameList of a compoundFile.
 * \return Returns #IUS_E_OK when succesful or #IUS_ERR_VALUE in case of an error.
 */
int iusCompoundFileSetCompoundFrameList
(
	iucf_t   compoundFile,        ///< The #IusCompoundFile of interest
	iucfrl_t compoundFrameList    ///< The #IusCompoundFrameList to set
);

/** \brief Sets the #IusCompoundWaveListDict of a compoundFile.
 * \return Returns #IUS_E_OK when succesful or #IUS_ERR_VALUE in case of an error.
 */
int iusCompoundFileSetCompoundWaveListDict
(
	iucf_t   compoundFile,           ///< The #IusCompoundFile of interest
	iucwld_t compoundWaveListDict    ///< The #IusCompoundWaveListDict to set
);

/** \brief Sets the pulseDict of a compoundFile.
 * \return Returns #IUS_E_OK when succesful or #IUS_ERR_VALUE in case of an error.
 */
int iusCompoundFileSetPulseDict
(
	iucf_t compoundFile,            ///< The #IusCompoundFile of interest
	iupd_t pulseDict                ///< The #IusPulseDict to set
);

/** \brief Sets the sourceDict of a compoundFile.
 * \return Returns #IUS_E_OK when succesful or #IUS_ERR_VALUE in case of an error.
 */
int iusCompoundFileSetSourceDict
(
    iucf_t compoundFile,              ///< The #IusCompoundFile of interest
    iusd_t  sourceDict                ///< The #IusSourceDict to set
);

/** \brief Sets the receiveChannelMap of a compoundFile.
 * \return Returns #IUS_E_OK when succesful or #IUS_ERR_VALUE in case of an error.
 */
int iusCompoundFileSetReceiveChannelMapDict
(
    iucf_t   compoundFile,            ///< The #IusCompoundFile of interest
    iurcmd_t receiveChannelMapDict    ///< The #IusReceiveChannelMapDict to set
);

/** \brief Sets the receiveApodizationDict of a compoundFile.
 * \return Returns #IUS_E_OK when succesful or #IUS_ERR_VALUE in case of an error.
 */
int iusCompoundFileSetReceiveApodizationDict
(
	iucf_t  compoundFile,             ///< The #IusCompoundFile of interest
	iurad_t receiveApodizationDict    ///< The #IusReceiveApodizationDict to set
);

/** \brief Saves a compoundFrame of data to a compoundFile.
 * \return Returns #IUS_E_OK when succesful or <0 in case of an error.
 */
int iusCompoundFileCompoundFrameSave
(
	iucf_t  compoundFile,              ///< The #IusCompoundFile of interest
	char    *label,                    ///< The type of #IusCompoundWaveList
	iud_t   compoundFrameData,         ///< the compound wave data to save
	iuo_t   compoundFrame_offset       ///< The offset in the datablock
);

/** \brief Loads a single compoundFrame of data from a compoundFile.
 * \return Returns #IUS_E_OK when succesful or <0 in case of an error.
 */
int iusCompoundFileCompoundFrameLoad
(
    iucf_t compoundFile,               ///< The #IusCompoundFile of interest
    char *label,                       ///< The type of #IusCompoundWaveList
    iud_t compoundFrameData,           ///< The data object to fill                 
    iuo_t compoundFrame_offset         ///< The offset in the datablock
);

#endif //IUSLIBRARY_IUSHLCOMPOUNDFILE_H
