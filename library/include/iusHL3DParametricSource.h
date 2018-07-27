
// Created by nlv09165 on 25/07/2018.
#ifndef IUSLIBRARY_IUSHL3DPARAMETRICSOURCE_H
#define IUSLIBRARY_IUSHL3DPARAMETRICSOURCE_H

#include "iusHLPosition.h"

// ADT
typedef struct Ius3DParametricSource Ius3DParametricSource;
typedef Ius3DParametricSource *iu3dps_t;
#define  IU3DPS_INVALID (iu3dps_t) NULL

iu3dps_t iusHL3DParametricSourceCreate
(
    char *pLabel,
    int numLocations,
    float fNumber,
    float deltaTheta,
    float startTheta,
    float deltaPhi,
    float startPhi
);


int iusHL3DParametricSourceDelete
(
    iu3dps_t ius3DParametricSource
);


// operations
int iusHL3DParametricSourceCompare
(
    iu3dps_t reference,
    iu3dps_t actual
);

// Getters
float iusHL3DParametricSourceGetFNumber
(
    iu3dps_t ius3DParametricSource
);

float iusHL3DParametricSourceGetAngularDelta
(
    iu3dps_t ius3DParametricSource
);

float iusHL3DParametricSourceGetStartAngle
(
    iu3dps_t ius3DParametricSource
);

float iusHL3DParametricSourceGetDeltaPhi
(
    iu3dps_t ius3DParametricSource
);

float iusHL3DParametricSourceGetStartPhi
(
    iu3dps_t ius3DParametricSource
);

iu3dp_t iusHL3DParametricSourceGetPosition
(
    iu3dps_t ius3DParametricSource,
    int index
);

// Setters
int iusHL3DParametricSourceSetFNumber
(
    iu3dps_t ius3DParametricSource,
    float FNumber
);

int iusHL3DParametricSourceSetAngularDelta
(
    iu3dps_t ius3DParametricSource,
    float angularDelta
);

int iusHL3DParametricSourceSetStartAngle
(
    iu3dps_t ius3DParametricSource,
    float startAngle
);

int iusHL3DParametricSourceSetDeltaPhi
(
    iu3dps_t ius3DParametricSource,
    float deltaPhi
);

int iusHL3DParametricSourceSetStartPhi
(
    iu3dps_t ius3DParametricSource,
    float startPhi
);

int iusHL3DParametricSourceSetPosition
(
    iu3dps_t ius3DParametricSource,
    iu3dp_t  pos,
    int index
);

#endif //IUSLIBRARY_IUSHL3DPARAMETRICSOURCE_H
