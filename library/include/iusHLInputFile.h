// Created by nlv09165 on 11/07/2018.
#ifndef IUSLIBRARY_IUSHLINPUTFILE_H
#define IUSLIBRARY_IUSHLINPUTFILE_H

#include <iusHLExperiment.h>
#include <iusHLPatternList.h>
#include <iusHLPulseDict.h>
#include <iusHLReceiveChannelMapDict.h>
#include <iusHLTransmitApodizationDict.h>
#include <iusHLFrameList.h>
#include "iusHLSourceDict.h"
#include "iusHLReceiveSettingsDict.h"
#include "iusHLTransducer.h"

// ADT
typedef struct IusInputFile IusInputFile;
typedef IusInputFile *iuif_t;

#define IUIF_INVALID (iuif_t) NULL
#define IUS_DEFAULT_NUM_FRAMES  1

iuif_t iusHLInputFileCreate
(
    const char *filename
);

int iusHLInputFileDelete
(
    iuif_t iusInputFile
);

// operations
iuif_t iusHLInputFileLoad
(
    const char *filename
);

int iusHLInputFileSave
(
    iuif_t fileHandle
);

int iusHLInputFileClose
(
    iuif_t fileHandle
);

int iusHLInputFileCompare
(
    iuif_t reference,
    iuif_t actual
);

// Getters
iufl_t iusHLInputFileGetFrameList
(
	iuif_t fileHandle
);

iupal_t iusHLInputFileGetPatternList
(
	iuif_t fileHandle
);

iupd_t iusHLInputFileGetPulseDict
(
	iuif_t fileHandle
);

iusd_t iusHLInputFileGetSourceDict
(
	iuif_t fileHandle
);

iurcmd_t iusHLInputFileGetReceiveChannelMapDict
(
	iuif_t fileHandle
);

iutad_t  iusHLInputFileGetTransmitApodizationDict
(
	iuif_t fileHandle
);

iursd_t iusHLInputFileGetReceiveSettingsDict
(
    iuif_t inputFile
);

iue_t iusHLInputFileGetExperiment
(
	iuif_t fileHandle
);

iut_t iusHLInputFileGetTransducer
(
	iuif_t fileHandle
);

// Setters
int iusHLInputFileSetFrameList
(
	iuif_t inputFile,
	iufl_t frameList
);

int iusHLInputFileSetPatternList
(
	iuif_t inputFile,
	iupal_t patternList
);

int iusHLInputFileSetPulseDict
(
	iuif_t inputFile,
	iupd_t pulseDict
);

int iusHLInputFileSetSourceDict
(
    iuif_t inputFile,
    iusd_t sourceDict
);

int iusHLInputFileSetReceiveChannelMapDict
(
    iuif_t inputFile,
    iurcmd_t receiveChannelMapDict
);

int iusHLInputFileSetTransmitApodizationDict
(
	iuif_t inputFile,
	iutad_t transmitApodizationDict
);

int iusHLInputFileSetReceiveSettingsDict
(
    iuif_t inputFile,
    iursd_t transmitApreceiveSettingsDict
);

int iusHLInputFileSetExperiment
(
	iuif_t inputFile,
	iue_t  experiment
);

int iusHLInputFileSetTransducer
(
	iuif_t inputFile,
	iut_t  transducer
);
#endif //IUSLIBRARY_IUSHLINPUTFILE_H
