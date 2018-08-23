//
// Created by Ruijzendaal on 27/03/2018.
//
#ifndef IUSLIBRARY_IUSHLDRIVINGSCHEME_H
#define IUSLIBRARY_IUSHLDRIVINGSCHEME_H

#include <stddef.h>

#include <iusTypes.h>
#include <iusPulse.h>
#include <iusPulseDict.h>
#include <iusPosition.h>
#include <iusTransmitPattern.h>
#include <iusTransmitPatternList.h>
#include <iusApodizationList.h>

// ADT
struct IusDrivingScheme;
typedef  struct IusDrivingScheme IusDrivingScheme;
typedef  IusDrivingScheme    * iuds_t;
#define IUDS_INVALID (iuds_t) NULL

iuds_t iusDrivingSchemeCreate
(
    IusDrivingSchemeType type,
    IusShape shape,       // determines whether to use 2D or 3D positions/Angles
    iusl_t transmitSources,
    iupd_t transmitPulses,
    iutpal_t transmitPatterns,
    iual_t apodizations,
    int numElements
);

int iusDrivingSchemeDelete
(
    iuds_t drivingScheme
);

// setters
int iusDrivingSchemeSetTransmitPatternDelay
(
    iuds_t drivingScheme,
    float transmitPatternDelay
);

int iusDrivingSchemeSetTransmitPulse
(
    iuds_t drivingScheme,
    iup_t *transmitPulse
);


int iusDrivingSchemeSetTransmitApodization
(
    iuds_t drivingScheme,
    float Apodization,
    int apodizationIndex,
    int elementIndex
);


// getters
iusl_t iusDrivingSchemeGetSourceLocationList
(
    iuds_t drivingScheme
);

float iusDrivingSchemeGetTransmitPatternDelay
(
    iuds_t drivingScheme
);

iutpal_t iusDrivingSchemeGetTransmitPatternList
(
    iuds_t drivingScheme
);

IusShape iusDrivingSchemeGetShape
(
    iuds_t drivingScheme
);

IusDrivingSchemeType iusDrivingSchemeGetType
(
    iuds_t drivingScheme
);

int iusDrivingSchemeGetNumTransmitPulses
(
    iuds_t drivingScheme
);

int iusDrivingSchemeGetNumTransmitSources
(
    iuds_t drivingScheme
);

int iusDrivingSchemeGetNumElements
(
    iuds_t drivingScheme
);


float iusDrivingSchemeGetSourceFNumber
(
iuds_t drivingScheme
);


float iusDrivingSchemeGetTransmitApodization
(
iuds_t drivingScheme,
int pulseIndex,
int elementIndex
);

int iusDrivingSchemeGetNumChannels
(
    iuds_t drivingScheme
);

iu3dp_t iusDrivingSchemeGet3DSourceLocation
(
    iuds_t drivingScheme,
    int elementIndex
);

float iusDrivingSchemeGetSourceStartPhi
(
iuds_t drivingScheme
);

float iusDrivingSchemeGetSourceDeltaPhi
(
iuds_t drivingScheme
);


float iusDrivingSchemeGetSourceStartTheta
(
iuds_t drivingScheme
);

float iusDrivingSchemeGetSourceDeltaTheta
(
iuds_t drivingScheme
);

IUS_BOOL iusDrivingSchemeCompare
(
iuds_t reference,
iuds_t actual
);
//
//
//float *iusCreateTransmitApodization
//(
//    int numTransmitPulses,
//    int numElements
//);
#endif //IUSLIBRARY_IUSHLDRIVINGSCHEME_H
