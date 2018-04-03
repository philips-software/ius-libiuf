//
// Created by Ruijzendaal on 20/03/2018.
//

#ifndef IUSUNITTESTS_UTIL_H
#define IUSUNITTESTS_UTIL_H
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
    );


iu2dte_t iusUtilCreate2DElement
    (
        float x,
        float z,
        float sx,
        float sz,
        float phi
    );

#endif //IUSUNITTESTS_UTIL_H