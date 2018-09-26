// Created by nlv09165 on 11/07/2018.
#ifndef IUSLIBRARY_IUSHLINPUTFILE_H
#define IUSLIBRARY_IUSHLINPUTFILE_H

#include <iusExperiment.h>
#include <iusPatternList.h>
#include <iusPulseDict.h>
#include <iusReceiveChannelMapDict.h>
#include <iusTransmitApodizationDict.h>
#include <iusFrameList.h>
#include "iusSourceDict.h"
#include "iusReceiveSettingsDict.h"
#include "iusTransducer.h"

// ADT
typedef struct IusHistoryNode IusInputFile;
typedef IusInputFile *iuif_t;

#define IUIF_INVALID (iuif_t) NULL
#define IUS_DEFAULT_NUM_FRAMES  1

iuif_t iusInputFileCreate
(
    const char *filename
);

int iusInputFileDelete
(
    iuif_t iusInputFile
);

// operations
iuif_t iusInputFileLoad
(
    const char *filename
);

int iusInputFileSave
(
    iuif_t fileHandle
);

int iusInputFileClose
(
    iuif_t fileHandle
);

int iusInputFileCompare
(
    iuif_t reference,
    iuif_t actual
);

// Getters
iufl_t iusInputFileGetFrameList
(
	iuif_t fileHandle
);

iupal_t iusInputFileGetPatternList
(
	iuif_t fileHandle
);

iupd_t iusInputFileGetPulseDict
(
	iuif_t fileHandle
);

iusd_t iusInputFileGetSourceDict
(
	iuif_t fileHandle
);

iurcmd_t iusInputFileGetReceiveChannelMapDict
(
	iuif_t fileHandle
);

iutad_t  iusInputFileGetTransmitApodizationDict
(
	iuif_t fileHandle
);

iursd_t iusInputFileGetReceiveSettingsDict
(
    iuif_t inputFile
);

iue_t iusInputFileGetExperiment
(
	iuif_t fileHandle
);

iut_t iusInputFileGetTransducer
(
	iuif_t fileHandle
);

// Setters
int iusInputFileSetFrameList
(
	iuif_t inputFile,
	iufl_t frameList
);

int iusInputFileSetPatternList
(
	iuif_t inputFile,
	iupal_t patternList
);

int iusInputFileSetPulseDict
(
	iuif_t inputFile,
	iupd_t pulseDict
);

int iusInputFileSetSourceDict
(
    iuif_t inputFile,
    iusd_t sourceDict
);

int iusInputFileSetReceiveChannelMapDict
(
    iuif_t inputFile,
    iurcmd_t receiveChannelMapDict
);

int iusInputFileSetTransmitApodizationDict
(
	iuif_t inputFile,
	iutad_t transmitApodizationDict
);

int iusInputFileSetReceiveSettingsDict
(
    iuif_t inputFile,
    iursd_t transmitApreceiveSettingsDict
);

int iusInputFileSetExperiment
(
	iuif_t inputFile,
	iue_t  experiment
);

int iusInputFileSetTransducer
(
	iuif_t inputFile,
	iut_t  transducer
);
#endif //IUSLIBRARY_IUSHLINPUTFILE_H
