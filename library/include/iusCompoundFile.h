// Created by nlv09165 on 11/07/2018.
#ifndef IUSLIBRARY_IUSHLCOMPOUNDFILE_H
#define IUSLIBRARY_IUSHLCOMPOUNDFILE_H

#include "iusData.h"
#include "iusOffset.h"

// ADT
typedef struct IusHistoryNode IusCompoundFile;
typedef IusCompoundFile *iucf_t;

#define IUCF_INVALID (iucf_t) NULL
#define IUS_DEFAULT_NUM_FRAMES  1

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

#endif //IUSLIBRARY_IUSHLCOMPOUNDFILE_H
