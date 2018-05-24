//
// Created by Ruijzendaal on 28/03/2018.
//

#ifndef IUSLIBRARY_IUSHLPOSITION_H
#define IUSLIBRARY_IUSHLPOSITION_H
#include <include/iusTypes.h>


// ADT
typedef Ius3DPosition *iu3dp_t;
typedef Ius2DPosition *iu2dp_t;


iu2dp_t iusHL2DPositionCreate
(
float x,
float z
);


iu3dp_t iusHL3DPositionCreate
(
float x,
float y,
float z
);

void iusHL3DPositionDelete
(
iu3dp_t iusPos
);

void iusHL2DPositionDelete
(
iu2dp_t iusPos
);



IUS_BOOL iusHL2DPositionCompare
(
iu2dp_t reference,
iu2dp_t actual
);

IUS_BOOL iusHL3DPositionCompare
(
iu3dp_t reference,
iu3dp_t actual
);

#endif //IUSLIBRARY_IUSHLPOSITION_H
