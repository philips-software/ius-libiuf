//
// Created by Ruijzendaal on 16/04/2018.
//
#ifndef IUSLIBRARY_TESTDATAGENERATORS_H
#define IUSLIBRARY_TESTDATAGENERATORS_H

#include <include/iusAcquisition.h>
#include <include/iusPatternList.h>
#include <include/iusPulseDict.h>
#include <include/iusReceiveChannelMapDict.h>
#include <include/iusTransmitApodizationDict.h>
#include <include/iusReceiveSettingsDict.h>
#include <include/iusFrameList.h>
#include <include/iusSourceDict.h>
#include <include/iusTransducer.h>
#include <include/iusInputFile.h>
#include <include/iusParameterDict.h>
#include <include/iusHistoryNode.h>


void dgFillData
(
iud_t data,
float value
);

iuhn_t dgGenerateHistoryNode
(

);

iupad_t dgGenerateParameterDict
(
	int numElements
);


int dgInputFileAddGeneratedData
(
	iuif_t inputFile,
	char *label,
	int numSamplesPerLine,
	int numChannels
);

iuif_t dgGenerateInputFile
(
    char *ptestFileName,
    char *transducerName,
    char *label,
    int numFrames,
    int numSamplesPerLine,
    int numChannels
);

int dgDeleteInputFile
(
	iuif_t iusInputFile
);

iufl_t dgGenerateFrameList
(
	int numFrames
);

iupal_t dgGeneratePatternList
(
	int numPatterns,
	float timeInterval,
	iursd_t receiveSettingsDict,
	iurcmd_t receiveChannelMapDict
);

iupald_t dgGeneratePatternListDict
(
	char *label,
	iursd_t receiveSettingsDict,
	iurcmd_t receiveChannelMapDict
);

iupd_t dgGeneratePulseDict
(
    void
);

iusd_t dgGenerateSourceDict
(
    void
);

iurcmd_t dgGenerateReceiveChannelMapDict
(
	char *label,
	int numChannels
);

iurcm_t dgGenerateReceiveChannelMap
(
	int numChannels
);

iutad_t dgGenerateTransmitApodizationDict
(
	char *label
);

iurs_t dgGenerateReceiveSettings
(
	int numSamplesPerLine
);

iursd_t dgGenerateReceiveSettingsDict
(
	char *label,
	int numSamplesPerLine
);

iua_t dgGenerateAcquisition
(
	void
);

iut_t dgGenerateTransducer
(
	char *transducerName
);


#endif //IUSLIBRARY_TESTDATAGENERATORS_H
