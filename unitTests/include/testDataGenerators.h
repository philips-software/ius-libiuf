//
// Created by Ruijzendaal on 16/04/2018.
//
#ifndef IUSLIBRARY_TESTDATAGENERATORS_H
#define IUSLIBRARY_TESTDATAGENERATORS_H

#include <include/iusHLExperiment.h>
#include <include/iusHLPatternList.h>
#include <include/iusHLPulseDict.h>
#include <include/iusHLReceiveChannelMapDict.h>

iupd_t dgGeneratePulseDict
(
  	void
);

iupal_t dgGeneratePatternList
(
  	void
);

iue_t dgGenerateExperiment
(
	void
);

iurcmd_t dgGenerateReceiveChannelMapDict
(
  void
);

#endif //IUSLIBRARY_TESTDATAGENERATORS_H
