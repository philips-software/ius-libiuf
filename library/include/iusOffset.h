
// Created by nlv09165 on 05/10/2018.
#ifndef IUSLIBRARY_IUSOFFSET_H
#define IUSLIBRARY_IUSOFFSET_H

// ADT
typedef struct IusOffset_
{
    int x ;
    int y ;
    int z ;
    int t ;
} IusOffset;

typedef IusOffset *iuo_t;
#define  IUO_INVALID (iuo_t) NULL

iuo_t iusOffsetCreate
(
    void
);

int iusOffsetDelete
(
    iuo_t iusOffset
);


// operations
int iusOffsetCompare
(
    iuo_t reference,
    iuo_t actual
);


#endif //IUSLIBRARY_IUSOFFSET_H
