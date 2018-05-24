//
// Created by Ruijzendaal on 27/03/2018.
//

#ifndef IUSLIBRARY_IUSHLDRIVINGSCHEME_H
#define IUSLIBRARY_IUSHLDRIVINGSCHEME_H


#include <stddef.h>
#include "iusTypes.h"
#include "iusHLPosition.h"
#include "iusHLSourceListOld.h"
#include "iusHLPulse.h"
#include "iusHLTransmitPattern.h"
#include "iusHLTransmitPatternList.h"
#include "iusHLApodizationList.h"
#include "iusHLPulseList.h"

// ADT
struct IusDrivingScheme;
typedef  struct IusDrivingScheme IusDrivingScheme;
typedef  IusDrivingScheme    * iuds_t;
#define IUDS_INVALID (iuds_t) NULL


iuds_t iusHLDrivingSchemeCreate
(
IusDrivingSchemeType type,
IusShape shape,       // determines whether to use 2D or 3D positions/Angles
iusl_t transmitSources,
iupl_t transmitPulses,
iutpal_t transmitPatterns,
iual_t apodizations,
int numElements
);

int iusHLDrivingSchemeDelete
(
iuds_t drivingScheme
);

// setters
int iusHLDrivingSchemeSetTransmitPatternDelay
(
    iuds_t drivingScheme,
    float transmitPatternDelay
);

int iusHLDrivingSchemeSetTransmitPulse
(
    iuds_t drivingScheme,
    iup_t *transmitPulse
);


int iusHLDrivingSchemeSetTransmitApodization
(
iuds_t drivingScheme,
float Apodization,
int apodizationIndex,
int elementIndex
);


// getters
iusl_t iusHLDrivingSchemeGetSourceLocationList
(
    iuds_t drivingScheme
);

float iusHLDrivingSchemeGetTransmitPatternDelay
(
    iuds_t drivingScheme
);

iutpal_t iusHLDrivingSchemeGetTransmitPatternList
(
    iuds_t drivingScheme
);

IusShape iusHLDrivingSchemeGetShape
(
    iuds_t drivingScheme
);

IusDrivingSchemeType iusHLDrivingSchemeGetType
(
    iuds_t drivingScheme
);

int iusHLDrivingSchemeGetNumTransmitPulses
(
    iuds_t drivingScheme
);

int iusHLDrivingSchemeGetNumTransmitSources
(
    iuds_t drivingScheme
);

int iusHLDrivingSchemeGetNumElements
(
    iuds_t drivingScheme
);


float iusHLDrivingSchemeGetSourceFNumber
(
iuds_t drivingScheme
);


float iusHLDrivingSchemeGetTransmitApodization
(
iuds_t drivingScheme,
int pulseIndex,
int elementIndex
);

int iusHLDrivingSchemeGetNumChannels
(
    iuds_t drivingScheme
);

iu3dp_t iusHLDrivingSchemeGet3DSourceLocation
(
    iuds_t drivingScheme,
    int elementIndex
);

float iusHLDrivingSchemeGetSourceStartPhi
(
iuds_t drivingScheme
);

float iusHLDrivingSchemeGetSourceDeltaPhi
(
iuds_t drivingScheme
);


float iusHLDrivingSchemeGetSourceStartTheta
(
iuds_t drivingScheme
);

float iusHLDrivingSchemeGetSourceDeltaTheta
(
iuds_t drivingScheme
);

IUS_BOOL iusHLDrivingSchemeCompare
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
