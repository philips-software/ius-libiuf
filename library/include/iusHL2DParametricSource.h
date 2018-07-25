
// Created by nlv09165 on 25/07/2018.
#ifndef IUSLIBRARY_IUSHL2DPARAMETRICSOURCE_H
#define IUSLIBRARY_IUSHL2DPARAMETRICSOURCE_H

//#ifndef IUSLIBRARY_IMPLEMENTATION
//typedef struct Ius2DParametricSource Ius2DParametricSource;
//#endif // IUSLIBRARY_IMPLEMENTATION

// ADT
typedef struct Ius2DParametricSource Ius2DParametricSource;
typedef Ius2DParametricSource *iu2dps_t;
#define  IU2DPS_INVALID (iu2dps_t) NULL

iu2dps_t iusHL2DParametricSourceCreate
(
    int intParam,
    float floatParam
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
int iusHL2DParametricSourceGetIntParam
(
    iu2dps_t ius2DParametricSource
);

float iusHL2DParametricSourceGetFloatParam
(
    iu2dps_t ius2DParametricSource
);

// Setters
int iusHL2DParametricSourceSetIntParam
(
    iu2dps_t ius2DParametricSource,
    int intParam
);

int iusHL2DParametricSourceSetFloatParam
(
    iu2dps_t ius2DParametricSource,
    float floatParam
);


#endif //IUSLIBRARY_IUSHL2DPARAMETRICSOURCE_H
