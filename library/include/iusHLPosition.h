//
// Created by Ruijzendaal on 28/03/2018.
//

#ifndef IUSLIBRARY_IUSHLPOSITION_H
#define IUSLIBRARY_IUSHLPOSITION_H
#include <include/iusTypes.h>


// ADT
typedef Ius3DPosition *iu3dp_t;
typedef Ius2DPosition *iu2dp_t;


IUS_BOOL iusCompare2DPosition
(
    iu2dp_t reference,
    iu2dp_t actual
);

IUS_BOOL iusCompare3DPosition
(
    iu3dp_t reference,
    iu3dp_t actual
);

iu2dp_t iusHLCreate2DPosition
(
    float x,
    float z
);


iu3dp_t iusHLCreate3DPosition
(
    float x,
    float y,
    float z
);


void iusHLDelete3DPosition
(
    iu3dp_t iusPos
);

void iusHLDelete2DPosition
(
    iu2dp_t iusPos
);


#endif //IUSLIBRARY_IUSHLPOSITION_H
