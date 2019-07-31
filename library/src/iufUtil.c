#include <math.h>
#include <stdlib.h>

#include <library_config.h>
#include <iufHDF5.h>
#include <iufUtil.h>

char* iufGetVersionMajor(void) {
    return IUF_VERSION_MAJOR;
}

char* iufGetVersionPatch(void) {
    return IUF_VERSION_PATCH;
}

char* iufGetVersionMinor(void) {
    return IUF_VERSION_MINOR;
}

char* iufGetVersion(void) {
    return IUF_VERSION;
}


IUF_BOOL fileExists(const char *fname)
{
    FILE *file = fopen(fname, "r");
    if (file)
    {
        fclose(file);
        return IUF_TRUE;
    }
    return IUF_FALSE;
}

IUF_BOOL  equalFloat
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
        return IUF_FALSE;
    }
    return IUF_TRUE;
}
