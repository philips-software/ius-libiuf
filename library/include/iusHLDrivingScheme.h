//
// Created by Ruijzendaal on 27/03/2018.
//

#ifndef IUSLIBRARY_IUSHLDRIVINGSCHEME_H
#define IUSLIBRARY_IUSHLDRIVINGSCHEME_H


#ifndef IUSLIBRARY_IMPLEMENTATION
#include <stddef.h>
#include "iusTypes.h"

struct IusDrivingScheme;
typedef  struct IusDrivingScheme IusDrivingScheme;
#endif // IUSLIBRARY_IMPLEMENTATION

// ADT
typedef  IusDrivingScheme    * iuds_t;
#define IUDS_INVALID (iuds_t) NULL

iuds_t iusHLCreateDrivingScheme
(
    IusDrivingSchemeType param,
    int numElements,
    IusTransducerShape shape, // determines whether to use 2D or 3D positions/Angles
    int numTransmitPulses
);

int iusHLDeleteDrivingScheme
    (
        iuds_t drivingScheme
    );

#endif //IUSLIBRARY_IUSHLDRIVINGSCHEME_H
