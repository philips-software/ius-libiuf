// Created by nlv09165 on 03/05/2018.
#ifndef IUSLIBRARY_IUSHLSOURCE_H
#define IUSLIBRARY_IUSHLSOURCE_H

// ADT
typedef struct IusSource IusSource;
typedef IusSource *ius_t;
#define  IUS_INVALID (ius_t) NULL

ius_t iusHLSourceCreate
(
    int intParam,
    float floatParam
);

int iusHLSourceDelete
(
    ius_t iusSource
);


// operations
int iusHLSourceCompare
(
    ius_t reference,
    ius_t actual
);

// Getters
int iusHLSourceGetIntParam
(
    ius_t iusSource
);

float iusHLSourceGetFloatParam
(
    ius_t iusSource
);

// Setters
int iusHLSourceSetIntParam
(
    ius_t iusSource,
    int intParam
);

int iusHLSourceSetFloatParam
(
    ius_t iusSource,
    float floatParam
);

#endif //IUSLIBRARY_IUSHLSOURCE_H
