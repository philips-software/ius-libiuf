
// Created by nlv09165 on 25/07/2018.
#ifndef IUSLIBRARY_IUSHL2DPARAMETRICSOURCE_H
#define IUSLIBRARY_IUSHL2DPARAMETRICSOURCE_H

#include "iusHLPosition.h"

// ADT
typedef struct Ius2DParametricSource Ius2DParametricSource;
typedef Ius2DParametricSource *iu2dps_t;
#define  IU2DPS_INVALID (iu2dps_t) NULL

iu2dps_t iusHL2DParametricSourceCreate
(
    char *pLabel,
    int numLocations,
    float fNumber,
    float deltaTheta,
    float startTheta
);


int iusHL2DParametricSourceDelete
(
    iu2dps_t ius2DParametricSource
);


// operations
int iusHL2DParametricSourceCompare
(
    iu2dps_t reference,
    iu2dps_t actual
);

// Getters
float iusHL2DParametricSourceGetFNumber
(
    iu2dps_t ius2DParametricSource
);

float iusHL2DParametricSourceGetAngularDelta
(
    iu2dps_t ius2DParametricSource
);

float iusHL2DParametricSourceGetStartAngle
(
    iu2dps_t ius2DParametricSource
);

iu2dp_t iusHL2DParametricSourceGetPosition
(
    iu2dps_t ius2DParametricSource,
    int index
);

// Setters
int iusHL2DParametricSourceSetFNumber
(
    iu2dps_t ius2DParametricSource,
    float FNumber
);

int iusHL2DParametricSourceSetAngularDelta
(
    iu2dps_t ius2DParametricSource,
    float angularDelta
);

int iusHL2DParametricSourceSetStartAngle
(
    iu2dps_t ius2DParametricSource,
    float startAngle
);

int iusHL2DParametricSourceSetPosition
(
    iu2dps_t ius2DParametricSource,
    iu2dp_t  pos,
    int index
);
#endif //IUSLIBRARY_IUSHL2DPARAMETRICSOURCE_H
