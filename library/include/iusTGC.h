// Created by nlv09165 on 08/08/2018.
#ifndef IUSLIBRARY_IUSHLTGC_H
#define IUSLIBRARY_IUSHLTGC_H

// ADT
typedef struct IusTGC IusTGC;
typedef IusTGC *iutgc_t;
#define  IUTGC_INVALID (iutgc_t) NULL


iutgc_t iusTGCCreate
(int numTGCValues);

int iusTGCDelete
(
    iutgc_t iusTGC
);


// operations
int iusTGCCompare
(
    iutgc_t reference,
    iutgc_t actual
);

// Getters
int iusTGCGetNumValues
(
    iutgc_t iusTGC
);

float iusTGCGetTime
(
    iutgc_t iusTGC,
    int index
);

float iusTGCGetGain
(
    iutgc_t iusTGC,
    int index
);

int iusTGCSet
(
    iutgc_t iusTGC,
    int index,
    float time,
    float gain
);

#endif //IUSLIBRARY_IUSHLTGC_H
