
// Created by nlv09165 on 25/07/2018.
#ifndef IUSLIBRARY_IUSHL3DPARAMETRICSOURCE_H
#define IUSLIBRARY_IUSHL3DPARAMETRICSOURCE_H

#include "iusPosition.h"

// ADT
typedef struct Ius3DParametricSource Ius3DParametricSource;
/** The 3D parametric sources describes source locations from which a pulse originates simulatanously with a set of parameters. 
* A 2D array of sources on a spherical 
*/
typedef Ius3DParametricSource *iu3dps_t;
#define  IU3DPS_INVALID (iu3dps_t) NULL

iu3dps_t ius3DParametricSourceCreate
(
    int numLocations,
    float fNumber,
    float deltaTheta,
    float startTheta,
    float deltaPhi,
    float startPhi
);


int ius3DParametricSourceDelete
(
    iu3dps_t ius3DParametricSource
);


// operations
int ius3DParametricSourceCompare
(
    iu3dps_t reference,
    iu3dps_t actual
);

// Getters
float ius3DParametricSourceGetFNumber
(
    iu3dps_t ius3DParametricSource
);

float ius3DParametricSourceGetAngularDelta
(
    iu3dps_t ius3DParametricSource
);

float ius3DParametricSourceGetStartAngle
(
    iu3dps_t ius3DParametricSource
);

float ius3DParametricSourceGetDeltaPhi
(
    iu3dps_t ius3DParametricSource
);

float ius3DParametricSourceGetStartPhi
(
    iu3dps_t ius3DParametricSource
);

// Setters
int ius3DParametricSourceSetFNumber
(
    iu3dps_t ius3DParametricSource,
    float FNumber
);

int ius3DParametricSourceSetAngularDelta
(
    iu3dps_t ius3DParametricSource,
    float angularDelta
);

int ius3DParametricSourceSetStartAngle
(
    iu3dps_t ius3DParametricSource,
    float startAngle
);

int ius3DParametricSourceSetDeltaPhi
(
    iu3dps_t ius3DParametricSource,
    float deltaPhi
);

int ius3DParametricSourceSetStartPhi
(
    iu3dps_t ius3DParametricSource,
    float startPhi
);


#endif //IUSLIBRARY_IUSHL3DPARAMETRICSOURCE_H
