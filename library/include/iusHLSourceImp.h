//
// Created by Ruijzendaal on 26/07/2018.
//

#ifndef IUSLIBRARY_IUSHLSOURCEIMP_H
#define IUSLIBRARY_IUSHLSOURCEIMP_H

#include <iusTypes.h>
#include <iusHLSource.h>
#include <hdf5.h>

struct IusSource
{
    IusSourceType type;
    char *label;
    int locationCount; /**< number of locations */
} ;


IUS_BOOL iusHLBaseSourceCompare
(
    ius_t reference,
    ius_t actual
);

#endif //IUSLIBRARY_IUSHLSOURCEIMP_H
