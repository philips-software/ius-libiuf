// Created by nlv09165 on 05/10/2018.
#ifndef IUSLIBRARY_IUSDATA_H
#define IUSLIBRARY_IUSDATA_H


// ADT
typedef struct IusData IusData;
typedef IusData *iud_t;
#define  IUD_INVALID (iud_t) NULL

iud_t iusDataCreate
(
    int size
);

int iusDataDelete
(
    iud_t iusData
);


// operations
int iusDataCompare
(
    iud_t reference,
    iud_t actual
);

int iusDataGetSize
(
    iud_t iusData
);

float *iusDataGetPointer
(
    iud_t iusData
);

#endif //IUSLIBRARY_IUSDATA_H
