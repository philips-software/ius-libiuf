#ifndef IUFLIBRARY_TESTDATAGENERATORS_H
#define IUFLIBRARY_TESTDATAGENERATORS_H

#include <iufAcquisition.h>
#include <iufPatternList.h>
#include <iufPulseDict.h>
#include <iufReceiveChannelMapDict.h>
#include <iufTransmitApodizationDict.h>
#include <iufReceiveSettingsDict.h>
#include <iufFrameList.h>
#include <iufSourceDict.h>
#include <iufTransducer.h>
#include <iufInputFile.h>
#include <iufIqFile.h>
#include <iufParameterDict.h>
#include <iufHistoryNode.h>


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

int dgIqFileAddGeneratedData
(
	iuiqf_t iqFile,
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

iuif_t dgGenerateInputFileVerasonics
(
        char *ptestFileName,
        char *transducerName,
        char *label,
        int numFrames,
        int numSamplesPerLine,
        int numChannels
);

iuiqf_t dgGenerateIqFile
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
	iuif_t iufInputFile
);

int dgDeleteIqFile
(
	iuiqf_t iufIqFile
);

iufl_t dgGenerateFrameList
(
	int numFrames
);

iufl_t dgGenerateFrameListVerasonics
(
    int numFrames,
    char *label
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

iuiqpal_t dgGenerateIqPatternList
(
	int numPatterns,
	float timeInterval,
	iudmd_t demodulationDict,
	iurcmd_t receiveChannelMapDict
);

iuiqpald_t dgGenerateIqPatternListDict
(
	char *label,
	iudmd_t demodulationDict,
	iurcmd_t receiveChannelMapDict
);

iupd_t dgGeneratePulseDict
(
    void
);

iupd_t dgGeneratePulseDictVerasonics
(
    void
);

iusd_t dgGenerateSourceDict
(
    void
);

iusd_t dgGenerateSourceDictVerasonics(
        char *label
);

iurcmd_t dgGenerateReceiveChannelMapDict
(
	char *label,
	int numChannels
);

iurcmd_t dgGenerateReceiveChannelMapDictVerasonics
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

iutad_t dgGenerateTransmitApodizationDictVerasonics
(
        char *label,
        int numElements
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

iudm_t dgGenerateDemodulation
(
	int numSamplesPerLine
);

iudmd_t dgGenerateDemodulationDict
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


#endif //IUFLIBRARY_TESTDATAGENERATORS_H
