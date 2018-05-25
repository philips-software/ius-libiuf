//
// Created by Ruijzendaal on 14/03/2018.
//
#include <math.h>
#include <stdlib.h>

#include <library_config.h>
#include <iusHDF5.h>
#include <iusUtil.h>

int iusHLDiagDisable()
{
    return iusHdf5DisableMessages();
}


int iusGetVersionMajor(void) {
    return IUS_VERSION_MAJOR;
}

IUS_BOOL  equalFloat
    (
        const float delta,
        const float expected,
        const float actual
    )
{
    float diff = actual - expected;
    float pos_delta = delta;

    if (diff < 0.0f)
    {
        diff = 0.0f - diff;
    }
    if (pos_delta < 0.0f)
    {
        pos_delta = 0.0f - pos_delta;
    }

    /* This first part of this condition will catch any NaN or Infinite values */
    if (isnan(diff) || isinf(diff) || (pos_delta < diff))
    {
        return IUS_FALSE;
    }
    return IUS_TRUE;
}