//
// Created by Ruijzendaal on 27/03/2018.
//

#ifndef IUSLIBRARY_IUSHLDRIVINGSCHEME_H
#define IUSLIBRARY_IUSHLDRIVINGSCHEME_H


#include "iusHLTransmitPulse.h"
#include "iusHLTransmitPattern.h"

#ifndef IUSLIBRARY_IMPLEMENTATION
#include <stddef.h>
#include "iusTypes.h"
#include "iusHLPosition.h"

struct IusDrivingScheme;
typedef  struct IusDrivingScheme IusDrivingScheme;
#endif // IUSLIBRARY_IMPLEMENTATION

// ADT
typedef  IusDrivingScheme    * iuds_t;
#define IUDS_INVALID (iuds_t) NULL

iuds_t iusHLCreateDrivingScheme
(
    IusDrivingSchemeType param,
    IusShape shape, // determines whether to use 2D or 3D positions/Angles
    int numTransmitPulses,
    int numTransmitSources,
    int numElements
);


int iusHLDeleteDrivingScheme
(
    iuds_t drivingScheme
);

iuds_t iusCreate3DDrivingScheme
(
    int numTransmitSources
);


iuds_t iusCreate2DDrivingScheme
(
    int numTransmitSources
);

// setters

int iusHLDrivingSchemeSetTransmitPulse
(
    iuds_t drivingScheme,
    iutp_t transmitPulse
);

int iusDrivingSchemeSetSourceFNumber
(
    iuds_t drivingScheme,
    float FNumber
);


int iusDrivingSchemeSetSourceStartPhi
(
    iuds_t drivingScheme,
    float startPhi
);

int iusDrivingSchemeSetSourceDeltaPhi
(
    iuds_t drivingScheme,
    float deltaPhi
);


int iusDrivingSchemeSetSourceStartTheta
(
    iuds_t drivingScheme,
    float startTheta
);

int iusDrivingSchemeSetSourceDeltaTheta
(
    iuds_t drivingScheme,
    float deltaTheta
);

int iusDrivingSchemeSetTransmitApodization
(
    iuds_t drivingScheme,
    float Apodization,
    int pulseIndex,
    int elementIndex
);

int iusHLDrivingSchemeSet3DSourceLocation
(
    IusDrivingScheme * drivingScheme,
    iu3dp_t position,
    int index
);

int iusHLDrivingSchemeSet2DSourceLocation
(
    IusDrivingScheme * drivingScheme,
    iu2dp_t position,
    int index
);

// getters
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

iu3dp_t iusHLDrivingSchemeGet3DSourceLocation
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

IUS_BOOL iusHLCompareDrivingScheme
(
    iuds_t reference,
    iuds_t actual
);


float *iusCreateTransmitApodization
(
    int numTransmitPulses,
    int numElements
);
#endif //IUSLIBRARY_IUSHLDRIVINGSCHEME_H
