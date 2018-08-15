
// Created by nlv09165 on 24/07/2018.
#ifndef IUSLIBRARY_IUSHLSOURCE_H
#define IUSLIBRARY_IUSHLSOURCE_H

//#ifndef IUSLIBRARY_IMPLEMENTATION
//typedef struct IusSource IusSource;
//#endif // IUSLIBRARY_IMPLEMENTATION



typedef enum {
  IUS_INVALID_SOURCE_TYPE = 0,
  IUS_2D_NON_PARAMETRIC_SOURCE,
  IUS_3D_NON_PARAMETRIC_SOURCE,
  IUS_2D_PARAMETRIC_SOURCE,
  IUS_3D_PARAMETRIC_SOURCE
} IusSourceType;


// ADT

typedef struct IusSource IusSource;
typedef IusSource *ius_t;
#define  IUS_INVALID (ius_t) NULL


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
IusSourceType iusHLSourceGetType
(
    ius_t iusSource
);

char * iusHLSourceGetLabel
(
    ius_t iusSource
);


#endif //IUSLIBRARY_IUSHLSOURCE_H
