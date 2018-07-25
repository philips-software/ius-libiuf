
// Created by nlv09165 on 25/07/2018.
#ifndef IUSLIBRARY_IUSHL2DNONPARAMETRICSOURCE_H
#define IUSLIBRARY_IUSHL2DNONPARAMETRICSOURCE_H

//#ifndef IUSLIBRARY_IMPLEMENTATION
//typedef struct Ius2DNonParametricSource Ius2DNonParametricSource;
//#endif // IUSLIBRARY_IMPLEMENTATION

// ADT
typedef struct Ius2DNonParametricSource Ius2DNonParametricSource;
typedef Ius2DNonParametricSource *iu2dnps_t;
#define  IU2DNPS_INVALID (iu2dnps_t) NULL

iu2dnps_t iusHL2DNonParametricSourceCreate
(
    int intParam,
    float floatParam
);

int iusHL2DNonParametricSourceDelete
(
    iu2dnps_t ius2DNonParametricSource
);


// operations
int iusHL2DNonParametricSourceCompare
(
    iu2dnps_t reference,
    iu2dnps_t actual
);

// Getters
int iusHL2DNonParametricSourceGetIntParam
(
    iu2dnps_t ius2DNonParametricSource
);

float iusHL2DNonParametricSourceGetFloatParam
(
    iu2dnps_t ius2DNonParametricSource
);

// Setters
int iusHL2DNonParametricSourceSetIntParam
(
    iu2dnps_t ius2DNonParametricSource,
    int intParam
);

int iusHL2DNonParametricSourceSetFloatParam
(
    iu2dnps_t ius2DNonParametricSource,
    float floatParam
);


#endif //IUSLIBRARY_IUSHL2DNONPARAMETRICSOURCE_H
