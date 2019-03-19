//
// Created by Ruijzendaal on 14/03/2018.
//
#ifndef IUFLIBRARY_IUFUTIL_H
#define IUFLIBRARY_IUFUTIL_H

#include <iufTypes.h>


#define FLOAT_PRECISION (0.00001f)
//#define IUF_EQUAL_FLOAT(a,b)    equalFloat(FLOAT_PRECISION*a,a,b)
#define IUF_EQUAL_FLOAT(a,b)    (a == b)

IUF_BOOL  equalFloat
(
    const float delta,
    const float expected,
    const float actual
);
#endif //IUFLIBRARY_IUFUTIL_H
