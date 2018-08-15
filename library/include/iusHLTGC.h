// Created by nlv09165 on 08/08/2018.
#ifndef IUSLIBRARY_IUSHLTGC_H
#define IUSLIBRARY_IUSHLTGC_H

// ADT
typedef struct IusTGC IusTGC;
typedef IusTGC *iutgc_t;
#define  IUTGC_INVALID (iutgc_t) NULL


iutgc_t iusHLTGCCreate
(int numTGCValues);

int iusHLTGCDelete
(
    iutgc_t iusTGC
);


// operations
int iusHLTGCCompare
(
    iutgc_t reference,
    iutgc_t actual
);

// Getters
int iusHLTGCGetNumValues
(
    iutgc_t iusTGC
);

float iusHLTGCGetTime
(
    iutgc_t iusTGC,
    int index
);

float iusHLTGCGetGain
(
    iutgc_t iusTGC,
    int index
);

int iusHLTGCSet
(
    iutgc_t iusTGC,
    int index,
    float time,
    float gain
);

#endif //IUSLIBRARY_IUSHLTGC_H
