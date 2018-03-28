//
// Created by Ruijzendaal on 28/03/2018.
//

#include <iusHLPosition.h>
#include <stdlib.h>
#include <include/iusUtil.h>


IUS_BOOL iusCompare3DPosition(Ius3DPosition *reference,Ius3DPosition *actual)
{
    if (IUS_EQUAL_FLOAT(reference->x, actual->x) &&
        IUS_EQUAL_FLOAT(reference->y, actual->y) &&
        IUS_EQUAL_FLOAT(reference->z, actual->z) )
        return IUS_TRUE;
    else
        return IUS_FALSE;
}


IUS_BOOL iusCompare2DPosition(Ius2DPosition *reference,Ius2DPosition *actual)
{
    if (IUS_EQUAL_FLOAT(reference->x, actual->x) &&
        IUS_EQUAL_FLOAT(reference->z, actual->z) )
        return IUS_TRUE;
    else
        return IUS_FALSE;
}

Ius3DPosition *iusHLCreate3DPosition
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


Ius2DPosition *iusHLCreate2DPosition
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

void iusHLDelete3DPosition(Ius3DPosition *iusPos)
{
    free(iusPos);
}

void iusHLDelete2DPosition(Ius2DPosition *iusPos)
{
    free(iusPos);
}