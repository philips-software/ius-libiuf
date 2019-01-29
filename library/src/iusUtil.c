//
// Created by Ruijzendaal on 14/03/2018.
//
#include <math.h>
#include <stdlib.h>

#include <library_config.h>
#include <iusHDF5.h>
#include <iusUtil.h>

char* iusGetVersionMajor(void) {
    return IUS_VERSION_MAJOR;
}

char* iusGetVersionPatch(void) {
    return IUS_VERSION_PATCH;
}

char* iusGetVersionMinor(void) {
    return IUS_VERSION_MINOR;
}

IUS_BOOL fileExists(const char *fname)
{
    FILE *file = fopen(fname, "r");
    if (file)
    {
        fclose(file);
        return IUS_TRUE;
    }
    return IUS_FALSE;
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
