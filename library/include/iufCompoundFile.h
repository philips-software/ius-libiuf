#ifndef IUFLIBRARY_IUFHLCOMPOUNDFILE_H
#define IUFLIBRARY_IUFHLCOMPOUNDFILE_H

#include "iufData.h"
#include "iufOffset.h"

// ADT
typedef struct IufHistoryNode IufCompoundFile;
typedef IufCompoundFile *iucf_t;

#define IUCF_INVALID (iucf_t) NULL
#define IUF_DEFAULT_NUM_FRAMES  1

/** \brief Create an empty file instance with a filename.
* \return Returns the empty file instance when succesful or #IUCF_INVALID in case of an error.
*/
iucf_t iufCompoundFileCreate
(
	const char *filename  ///< The filename for the file instance.
);

/** \brief Creates a datablock for a single channel (i.e. numSamplesPerLine floats) for the acquisition with
* the name \p label.
* \return Returns an allocated memory block of a "label"-channel or IUD_INVALID in case of an error.
*/
int iufCompoundFileDelete
(
	iucf_t inputFile     ///< The #IufCompoundFile of interest
);

/** \brief Load an inputFile instance from a file.
* \return Returns an input file instance or IUIF_INVALID the file could not be read.
*/
iucf_t iufCompoundFileNodeLoad
(
	const char *pFilename     ///< The filename of the inputFile
);

/** \brief Save (serialize) the inputFile instance to file.
* \return Returns #IUF_E_OK when succesful or #IUF_ERR_VALUE in case of an error
*/
int iufCompoundFileNodeSave
(
	iucf_t inputFile       ///< The #IufCompoundFile of interest
);

/** \brief Closes access to the inputFile
* \return Returns #IUF_E_OK when succesful or #IUF_ERR_VALUE in case of an error
*/
int iufCompoundFileClose
(
	iucf_t inputFile        ///< The #IufCompoundFile of interest
);

/** \brief Compares two inputFile instances with each other.
* \return Returns #IUF_TRUE when the inputFiles are equal and #IUF_FALSE otherwise.
*/
int iufCompoundFileCompare
(
	iucf_t reference,        ///< The #IufCompoundFile to compare to
	iucf_t actual            ///< The #IufCompoundFile to compare with
);

#endif //IUFLIBRARY_IUFHLCOMPOUNDFILE_H
