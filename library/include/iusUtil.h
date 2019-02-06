//
// Created by Ruijzendaal on 14/03/2018.
//
#ifndef IUSLIBRARY_IUSUTIL_H
#define IUSLIBRARY_IUSUTIL_H

#include <iusTypes.h>


#define FLOAT_PRECISION (0.00001f)
//#define IUS_EQUAL_FLOAT(a,b)    equalFloat(FLOAT_PRECISION*a,a,b)
#define IUS_EQUAL_FLOAT(a,b)    (a == b)

IUS_BOOL  equalFloat
(
    const float delta,
    const float expected,
    const float actual
);
#endif //IUSLIBRARY_IUSUTIL_H
