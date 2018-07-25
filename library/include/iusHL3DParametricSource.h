
// Created by nlv09165 on 25/07/2018.
#ifndef IUSLIBRARY_IUSHL3DPARAMETRICSOURCE_H
#define IUSLIBRARY_IUSHL3DPARAMETRICSOURCE_H

// ADT
typedef struct Ius3DParametricSource Ius3DParametricSource;
typedef Ius3DParametricSource *iu3dps_t;
#define  IU3DPS_INVALID (iu3dps_t) NULL

iu3dps_t iusHL3DParametricSourceCreate
(
    int intParam,
    float floatParam
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
int iusHL3DParametricSourceGetIntParam
(
    iu3dps_t ius3DParametricSource
);

float iusHL3DParametricSourceGetFloatParam
(
    iu3dps_t ius3DParametricSource
);

// Setters
int iusHL3DParametricSourceSetIntParam
(
    iu3dps_t ius3DParametricSource,
    int intParam
);

int iusHL3DParametricSourceSetFloatParam
(
    iu3dps_t ius3DParametricSource,
    float floatParam
);


#endif //IUSLIBRARY_IUSHL3DPARAMETRICSOURCE_H
