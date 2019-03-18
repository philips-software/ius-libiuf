//
// Created by Ruijzendaal on 20/03/2018.
//
#include <iuf.h>
#include <string.h>


IUF_BOOL  aInB
(
    char *a,
    char **b
)
{
    int i=0;
    char *current = b[i];
    while (current != NULL)
    {
        if (strcmp(a,b[i])==0)
        {
            return IUF_TRUE;
        }
        i++;
    }
    return IUF_FALSE;
}

iu3dte_t iufUtilCreate3DElement
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
    iu3dp_t pos = iuf3DPositionCreate(
    x,
    y,
    z
    );
    iu3da_t ang = iuf3DAngleCreate(
    theta,
    phi
    );
    iu3ds_t siz = iuf3DSizeCreate(
    sx,
    sy,
    sz
    );
    iu3dte_t ele = iuf3DTransducerElementCreate(
    pos,
    ang,
    siz
    );
    return ele;
}

iu2dte_t iufUtilCreate2DElement
    (
        float x,
        float z,
        float sx,
        float sz,
        float phi
    )
{
    iu2dp_t pos = iuf2DPositionCreate(x, z);
    iu2ds_t siz = iuf2DSizeCreate(sx, sz);
    iu2dte_t ele = iuf2DTransducerElementCreate(pos, phi, siz);
    return ele;
}


