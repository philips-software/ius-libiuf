//
// Created by Ruijzendaal on 20/03/2018.
//
#include <unity.h>

#include <ius.h>
#include <util.h>
#include <iusTransducer.h>
#include <iusPosition.h>
#include <ius3DAngle.h>
#include <ius3DSize.h>

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
    iu3dp_t pos = ius3DPositionCreate(
    x,
    y,
    z
    );
    iu3da_t ang = ius3DAngleCreate(
    theta,
    phi
    );
    iu3ds_t siz = ius3DSizeCreate(
    sx,
    sy,
    sz
    );
    iu3dte_t ele = ius3DTransducerElementCreate(
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
    iu2dp_t pos = ius2DPositionCreate(x, z);
    iu2ds_t siz = ius2DSizeCreate(sx, sz);
    iu2dte_t ele = ius2DTransducerElementCreate(pos, phi, siz);
    return ele;
}

