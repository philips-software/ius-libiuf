// Created by nlv09165 on 31/07/2018.
#ifndef IUSLIBRARY_IUSHL2DNONPARAMETRICSOURCE_H
#define IUSLIBRARY_IUSHL2DNONPARAMETRICSOURCE_H

#include "iusPosition.h"

// ADT
typedef struct Ius2DNonParametricSource Ius2DNonParametricSource;
typedef Ius2DNonParametricSource *iu2dnps_t;
#define  IU2DNPS_INVALID (iu2dnps_t) NULL

iu2dnps_t ius2DNonParametricSourceCreate
(
    int numLocations
);


int ius2DNonParametricSourceDelete
(
    iu2dnps_t ius2DNonParametricSource
);


// operations
int ius2DNonParametricSourceCompare
(
    iu2dnps_t reference,
    iu2dnps_t actual
);

// Getters
iu2dp_t ius2DNonParametricSourceGetPosition
(
    iu2dnps_t ius2DNonParametricSource,
    int index
);

// Setters
int ius2DNonParametricSourceSetPosition
(
    iu2dnps_t ius2DNonParametricSource,
    iu2dp_t  pos,
    int index
);

#endif //IUSLIBRARY_IUSHL2DNONPARAMETRICSOURCE_H

