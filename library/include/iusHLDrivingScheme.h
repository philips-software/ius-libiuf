//
// Created by Ruijzendaal on 27/03/2018.
//

#ifndef IUSLIBRARY_IUSHLDRIVINGSCHEME_H
#define IUSLIBRARY_IUSHLDRIVINGSCHEME_H


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

// setters
int iusDrivingSchemeSetSourceAngularDelta
(
    iuds_t drivingScheme,
    float angulatDelta
);


int iusDrivingSchemeSetSourceFNumber
(
    iuds_t drivingScheme,
    float FNumber
);

int iusDrivingSchemeSetSourceStartAngle
(
    iuds_t drivingScheme,
    float startAngle
);

int iusDrivingSchemeSetTransmitApodization
(
    iuds_t drivingScheme,
    float Apodization,
    int index
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

float iusDrivingSchemeGetSourceAngularDelta
(
    iuds_t drivingScheme
);

float iusDrivingSchemeGetSourceFNumber
(
    iuds_t drivingScheme
);

float iusDrivingSchemeGetSourceStartAngle
(
    iuds_t drivingScheme
);

float iusDrivingSchemeGetTransmitApodization
(
    iuds_t drivingScheme,
    int elementIndex
);

iu3dp_t iusHLDrivingSchemeGet3DSourceLocation
(
    iuds_t drivingScheme,
    int elementIndex
);

#endif //IUSLIBRARY_IUSHLDRIVINGSCHEME_H
