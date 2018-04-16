//
// Created by Ruijzendaal on 16/04/2018.
//

#ifndef IUSLIBRARY_TESTDATAGENERATORS_H
#define IUSLIBRARY_TESTDATAGENERATORS_H

#include <include/iusHLSourceLocationList.h>
#include <include/iusHLNonParametricTransmitPulse.h>
#include <include/iusHLTransmitPulseList.h>
#include <include/iusHLTransmitPatternList.h>

int fill3DSourceLocationList
(
    iusll_t sourceLocationList
);
int nonParametricPulseFillAmplitudeTime
(
    iunptp_t transmitPulse
);

int fillTransmitPulseList
(
    iutpl_t transmitPulses
);
int fillTransmitPatternList
(
    iutpal_t transmitPatterns
);

int fill2DSourceLocationList
(
    iusll_t list
);

iuds_t dgCreateDrivingScheme
(
    IusDrivingSchemeType type,
    IusShape shape,
    IusSourceLocationType locationType,
    int numTransmitPulses,
    int numTransmitSources,
    int numElements
);

#endif //IUSLIBRARY_TESTDATAGENERATORS_H
