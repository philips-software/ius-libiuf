
// Created by nlv09165 on 25/07/2018.
#ifndef IUSLIBRARY_IUSHL2DPARAMETRICSOURCE_H
#define IUSLIBRARY_IUSHL2DPARAMETRICSOURCE_H

#include "iusPosition.h"

// ADT
typedef struct Ius2DParametricSource Ius2DParametricSource;
typedef Ius2DParametricSource *iu2dps_t;
#define  IU2DPS_INVALID (iu2dps_t) NULL

iu2dps_t ius2DParametricSourceCreate
(
    int numLocations,
    float fNumber,
    float deltaTheta,
    float startTheta
);


int ius2DParametricSourceDelete
(
    iu2dps_t ius2DParametricSource
);


// operations
int ius2DParametricSourceCompare
(
    iu2dps_t reference,
    iu2dps_t actual
);

// Getters
float ius2DParametricSourceGetFNumber
(
    iu2dps_t ius2DParametricSource
);

float ius2DParametricSourceGetAngularDelta
(
    iu2dps_t ius2DParametricSource
);

float ius2DParametricSourceGetStartAngle
(
    iu2dps_t ius2DParametricSource
);


// Setters
int ius2DParametricSourceSetFNumber
(
    iu2dps_t ius2DParametricSource,
    float FNumber
);

int ius2DParametricSourceSetAngularDelta
(
    iu2dps_t ius2DParametricSource,
    float angularDelta
);

int ius2DParametricSourceSetStartAngle
(
    iu2dps_t ius2DParametricSource,
    float startAngle
);

#endif //IUSLIBRARY_IUSHL2DPARAMETRICSOURCE_H
