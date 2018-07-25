//
// Created by Ruijzendaal on 16/04/2018.
//
#ifndef IUSLIBRARY_TESTDATAGENERATORS_H
#define IUSLIBRARY_TESTDATAGENERATORS_H

#include <include/iusHLPulseDict.h>
#include <include/iusHLReceiveChannelMapDict.h>
#include <include/iusHLTransmitApodizationDict.h>

iupd_t dgGeneratePulseDict
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
#endif //IUSLIBRARY_TESTDATAGENERATORS_H
