// Created by nlv09165 on 24/05/2018.
#ifndef IUSLIBRARY_IUSHL3DSIZE_H
#define IUSLIBRARY_IUSHL3DSIZE_H

// ADT
typedef struct Ius3DSize
{
	float sx;
	float sy;
	float sz;
} Ius3DSize;

typedef Ius3DSize *iu3ds_t;
#define  IU3DS_INVALID (iu3ds_t) NULL

iu3ds_t ius3DSizeCreate
(
  float sx,
  float sy,
  float sz
);

int ius3DSizeDelete
(
    iu3ds_t ius3DSize
);


// operations
int ius3DSizeCompare
(
    iu3ds_t reference,
    iu3ds_t actual
);
#endif //IUSLIBRARY_IUSHL3DSIZE_H
