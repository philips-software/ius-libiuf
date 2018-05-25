// Created by nlv09165 on 24/05/2018.
#ifndef IUSLIBRARY_IUSHL2DSIZE_H
#define IUSLIBRARY_IUSHL2DSIZE_H

// ADT
typedef struct Ius2DSize Ius2DSize;
typedef Ius2DSize *iu2ds_t;
#define  IU2DS_INVALID (iu2ds_t) NULL

iu2ds_t iusHL2DSizeCreate
(
  float sx,
  float sz
);

int iusHL2DSizeDelete
(
    iu2ds_t ius2DSize
);


// operations
int iusHL2DSizeCompare
(
    iu2ds_t reference,
    iu2ds_t actual
);

#endif //IUSLIBRARY_IUSHL2DSIZE_H
