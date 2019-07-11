#ifndef IUFUNITTESTS_UTIL_H
#define IUFUNITTESTS_UTIL_H
#include <iuf2DTransducerElement.h>
#include <iuf3DTransducerElement.h>

IUF_BOOL  aInB
(
    char *a,
    char **b
);

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
);


iu2dte_t iufUtilCreate2DElement
(
    float x,
    float z,
    float sx,
    float sz,
    float phi
);

#endif //IUFUNITTESTS_UTIL_H
