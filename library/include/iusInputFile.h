// Created by nlv09165 on 11/07/2018.
#ifndef IUSLIBRARY_IUSHLINPUTFILE_H
#define IUSLIBRARY_IUSHLINPUTFILE_H

#include <iusExperiment.h>
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
typedef IusInputFile *iuif_t;

#define IUIF_INVALID (iuif_t) NULL
#define IUS_DEFAULT_NUM_FRAMES  1

iuif_t iusInputFileCreate
(
    const char *filename,
    int numFrames
);

iud_t iusInputFileFrameCreate
(
    iuif_t iusInputFile,
    char *label
);

iud_t iusInputFileResponseCreate
(
    iuif_t iusInputFile,
    char *label
);

iud_t iusInputFileChannelCreate
(
    iuif_t iusInputFile,
    char *label
);

int iusInputFileDelete
(
    iuif_t iusInputFile
);

// operations
iuif_t iusInputFileNodeLoad
(
	const char *filename
);

int iusInputFileNodeSave
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

iupald_t iusInputFileGetPatternListDict
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

int iusInputFileGetNumFrames
(
    iuif_t inputFile
);

int iusInputFileGetNumResponses
(
	iuif_t inputFile,
	char *label
);

int iusInputFileGetNumChannels
(
    iuif_t iusInputFile,
    char *label
);

// Setters
int iusInputFileSetFrameList
(
	iuif_t inputFile,
	iufl_t frameList
);

int iusInputFileSetPatternListDict
(
	iuif_t inputFile,
	iupald_t patternListDict
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


int iusInputFileSetNumFrames
(
    iuif_t inputFile,
    int  numFrames
);

int iusInputFileChannelSave
(
    iuif_t inputFile,
    char *label,
    iud_t channel,
    iuo_t channel_offset
);

int iusInputFileChannelLoad
(
    iuif_t inputFile,
    char *label,
    iud_t channel,
    iuo_t channel_offset
);

int iusInputFileResponseSave
(
	iuif_t inputFile,
	char *label,
	iud_t response,
	iuo_t response_offset
);

int iusInputFileResponseLoad
(
    iuif_t inputFile,
    char *label,
    iud_t response,
    iuo_t response_offset
);

int iusInputFileFrameSave
(
	iuif_t inputFile,
	char *label,
	iud_t frame,
	iuo_t offset
);

int iusInputFileFrameLoad
(
	iuif_t inputFile,
	char *label,
	iud_t frame,
	iuo_t frame_offset
);



#endif //IUSLIBRARY_IUSHLINPUTFILE_H
