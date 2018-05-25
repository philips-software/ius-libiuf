//
// Created by Ruijzendaal on 28/03/2018.
//
#include <stdlib.h>

#include <iusHLPosition.h>
#include <iusUtil.h>


IUS_BOOL iusHL3DPositionCompare
(
    Ius3DPosition *reference,
    Ius3DPosition *actual
)
{
    if (IUS_EQUAL_FLOAT(reference->x, actual->x) &&
        IUS_EQUAL_FLOAT(reference->y, actual->y) &&
        IUS_EQUAL_FLOAT(reference->z, actual->z) )
        return IUS_TRUE;
    else
        return IUS_FALSE;
}


IUS_BOOL iusHL2DPositionCompare
(
    Ius2DPosition *reference,
    Ius2DPosition *actual
)
{
    if (IUS_EQUAL_FLOAT(reference->x, actual->x) &&
        IUS_EQUAL_FLOAT(reference->z, actual->z) )
        return IUS_TRUE;
    else
        return IUS_FALSE;
}

Ius3DPosition *iusHL3DPositionCreate
(
    float x,
    float y,
    float z
)
{
    Ius3DPosition *iusPos = calloc(1, sizeof(Ius3DPosition));
    iusPos->x=x;
    iusPos->y=y;
    iusPos->z=z;
    return iusPos;
}


Ius2DPosition *iusHL2DPositionCreate
(
    float x,
    float z
)
{
    Ius2DPosition *iusPos = calloc(1, sizeof(Ius2DPosition));
    iusPos->x=x;
    iusPos->z=z;
    return iusPos;
}

void iusHL3DPositionDelete
(
    Ius3DPosition *iusPos
)
{
    free(iusPos);
}

void iusHL2DPositionDelete
(
    Ius2DPosition *iusPos
)
{
    free(iusPos);
}