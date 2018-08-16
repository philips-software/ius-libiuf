//
// Created by Ruijzendaal on 16/04/2018.
//
#ifndef IUSLIBRARY_TESTDATAGENERATORS_H
#define IUSLIBRARY_TESTDATAGENERATORS_H

#include <include/iusHLExperiment.h>
#include <include/iusHLPatternList.h>
#include <include/iusHLPulseDict.h>
#include <include/iusHLReceiveChannelMapDict.h>
#include <include/iusHLTransmitApodizationDict.h>
#include <include/iusHLReceiveSettingsDict.h>
#include <include/iusHLFrameList.h>
#include <include/iusHLSourceDict.h>

iupd_t dgGeneratePulseDict
(
  	void
);

iupal_t dgGeneratePatternList
(
  	void
);

iusd_t dgGenerateSourceDict
(
	void
);

iue_t dgGenerateExperiment
(
	void
);

iufl_t dgGenerateFrameList
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

#endif //IUSLIBRARY_TESTDATAGENERATORS_H
