//
// Created by Ruijzendaal on 28/03/2018.
//
#ifndef IUSLIBRARY_IUSHLPOSITION_H
#define IUSLIBRARY_IUSHLPOSITION_H

#include <iusTypes.h>

// ADT
struct   Ius3DPosition;
typedef  struct Ius3DPosition Ius3DPosition;
typedef  Ius3DPosition    * iu3dp_t;
#define  IU3DP_INVALID (iu3dp_t) NULL

typedef  struct Ius2DPosition Ius2DPosition;
typedef  Ius2DPosition    * iu2dp_t;
#define  IU2DP_INVALID (iu2dp_t) NULL


iu2dp_t ius2DPositionCreate
(
    float x,
    float z
);


iu3dp_t ius3DPositionCreate
(
    float x,
    float y,
    float z
);

void ius3DPositionDelete
(
    iu3dp_t iusPos
);

void ius2DPositionDelete
(
    iu2dp_t iusPos
);


IUS_BOOL ius2DPositionCompare
(
    iu2dp_t reference,
    iu2dp_t actual
);

IUS_BOOL ius3DPositionCompare
(
    iu3dp_t reference,
    iu3dp_t actual
);

#endif //IUSLIBRARY_IUSHLPOSITION_H
