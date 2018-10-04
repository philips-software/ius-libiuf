
// Created by nlv09165 on 25/07/2018.
#ifndef IUSLIBRARY_IUSHL3DNONPARAMETRICSOURCE_H
#define IUSLIBRARY_IUSHL3DNONPARAMETRICSOURCE_H

#include "iusPosition.h"

// ADT
typedef struct Ius3DNonParametricSource Ius3DNonParametricSource;
typedef Ius3DNonParametricSource *iu3dnps_t;
#define  IU3DNPS_INVALID (iu3dnps_t) NULL

iu3dnps_t ius3DNonParametricSourceCreate
(
    int numLocations
);


int ius3DNonParametricSourceDelete
(
    iu3dnps_t ius3DNonParametricSource
);


// operations
int ius3DNonParametricSourceCompare
(
    iu3dnps_t reference,
    iu3dnps_t actual
);

// Getters
iu3dp_t ius3DNonParametricSourceGetPosition
(
    iu3dnps_t ius3DNonParametricSource,
    int index
);

// Setters
int ius3DNonParametricSourceSetPosition
(
    iu3dnps_t ius3DNonParametricSource,
    iu3dp_t  pos,
    int index
);

#endif //IUSLIBRARY_IUSHL3DNONPARAMETRICSOURCE_H
