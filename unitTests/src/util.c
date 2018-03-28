//
// Created by Ruijzendaal on 20/03/2018.
//

#include <include/iusHLTransducer.h>
#include <include/iusHLPosition.h>
#include "util.h"

iu3dte_t iusUtilCreate3DElement
    (
        float x,
        float y,
        float z,
        float sx,
        float sy,
        float sz,
        float theta,
        float phi
    )
{
    iu3dp_t pos = iusHLCreate3DPosition(
        x,
        y,
        z
    );
    iu3da_t ang = iusHLCreate3DElementAngle(
        theta,
        phi
    );
    iu3ds_t siz = iusHLCreate3DElementSize(
        sx,
        sy,
        sz
    );
    iu3dte_t ele = iusHLCreate3DElement(
        pos,
        ang,
        siz
    );
    return ele;
}

iu2dte_t iusUtilCreate2DElement
    (
        float x,
        float z,
        float sx,
        float sz,
        float phi
    )
{
    iu2dp_t pos = iusHLCreate2DPosition(x, z);
    iu2ds_t siz = iusHLCreate2DElementSize(sx, sz);
    iu2dte_t ele = iusHLCreate2DElement(pos, phi, siz);
    return ele;
}