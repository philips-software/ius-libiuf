//
// Created by Ruijzendaal on 16/04/2018.
//
#ifndef IUSLIBRARY_TESTDATAGENERATORS_H
#define IUSLIBRARY_TESTDATAGENERATORS_H

#include <include/iusExperiment.h>
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

iuhn_t dgGenerateHistoryNode
(

);

iupad_t dgGenerateParameterDict
(
	int numElements
);

iuif_t dgGenerateInputFile
(
	char *ptestFileName,
	char *transducerName
);

iufl_t dgGenerateFrameList
(
	void
);

iupal_t dgGeneratePatternList
(
  	void
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
  void
);

iutad_t dgGenerateTransmitApodizationDict
(
	void
);

iursd_t dgGenerateReceiveSettingsDict
(
	void
);

iue_t dgGenerateExperiment
(
	void
);

iut_t dgGenerateTransducer
(
	char *transducerName
);


#endif //IUSLIBRARY_TESTDATAGENERATORS_H
