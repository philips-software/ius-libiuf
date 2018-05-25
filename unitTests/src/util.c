//
// Created by Ruijzendaal on 20/03/2018.
//
#include <unity.h>

#include <ius.h>
#include <util.h>
#include <iusHLTransducer.h>
#include <iusHLPosition.h>
#include <iusHL3DAngle.h>
#include <iusHL3DSize.h>

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
    iu3dp_t pos = iusHL3DPositionCreate(
    x,
    y,
    z
    );
    iu3da_t ang = iusHL3DAngleCreate(
    theta,
    phi
    );
    iu3ds_t siz = iusHL3DSizeCreate(
    sx,
    sy,
    sz
    );
    iu3dte_t ele = iusHL3DTransducerElementCreate(
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
    iu2dp_t pos = iusHL2DPositionCreate(x, z);
    iu2ds_t siz = iusHL2DSizeCreate(sx, sz);
    iu2dte_t ele = iusHL2DTransducerElementCreate(pos, phi, siz);
    return ele;
}

