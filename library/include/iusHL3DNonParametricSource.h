
// Created by nlv09165 on 25/07/2018.
#ifndef IUSLIBRARY_IUSHL3DNONPARAMETRICSOURCE_H
#define IUSLIBRARY_IUSHL3DNONPARAMETRICSOURCE_H

//#ifndef IUSLIBRARY_IMPLEMENTATION
//typedef struct Ius3DNonParametricSource Ius3DNonParametricSource;
//#endif // IUSLIBRARY_IMPLEMENTATION

// ADT
typedef struct Ius3DNonParametricSource Ius3DNonParametricSource;
typedef Ius3DNonParametricSource *iu3dnps_t;
#define  IU3DNPS_INVALID (iu3dnps_t) NULL

iu3dnps_t iusHL3DNonParametricSourceCreate
(
    int intParam,
    float floatParam
);

int iusHL3DNonParametricSourceDelete
(
    iu3dnps_t ius3DNonParametricSource
);


// operations
int iusHL3DNonParametricSourceCompare
(
    iu3dnps_t reference,
    iu3dnps_t actual
);

// Getters
int iusHL3DNonParametricSourceGetIntParam
(
    iu3dnps_t ius3DNonParametricSource
);

float iusHL3DNonParametricSourceGetFloatParam
(
    iu3dnps_t ius3DNonParametricSource
);

// Setters
int iusHL3DNonParametricSourceSetIntParam
(
    iu3dnps_t ius3DNonParametricSource,
    int intParam
);

int iusHL3DNonParametricSourceSetFloatParam
(
    iu3dnps_t ius3DNonParametricSource,
    float floatParam
);


#endif //IUSLIBRARY_IUSHL3DNONPARAMETRICSOURCE_H
