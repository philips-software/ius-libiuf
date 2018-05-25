//
// Created by Ruijzendaal on 16/04/2018.
//
#ifndef IUSLIBRARY_TESTDATAGENERATORS_H
#define IUSLIBRARY_TESTDATAGENERATORS_H

#include <iusHLNonParametricPulse.h>
#include <iusHLPulseList.h>
#include <iusHLTransmitPatternList.h>
#include <iusHLDrivingScheme.h>


// TODO: replace with dict!
typedef int iu3dsl_t;
typedef int iu2dsl_t;

int fill3DSourceLocationList
(
    iu3dsl_t sourceLocationList
);

int nonParametricPulseFillAmplitudeTime
(
    iunpp_t transmitPulse
);

int fillTransmitPulseList
(
    iupl_t transmitPulses
);

int fillTransmitPatternList
(
    iutpal_t transmitPatterns
);

int fill2DSourceLocationList
(
    iu2dsl_t list
);

int nonParametricPulseFillAmplitudeTime2
(
    iunpp_t transmitPulse,
    float offset
);

iuds_t dgCreateDrivingScheme
(
    IusDrivingSchemeType type,
    IusShape shape,
    IusSourceType locationType,
    int numTransmitPulses,
    int numTransmitSources,
    int numChannels,
    int numApodizations,
    int numTGCs,
    int numElements
);

int dgDeleteDrivingScheme
(
    iuds_t drivingScheme
);

#endif //IUSLIBRARY_TESTDATAGENERATORS_H
