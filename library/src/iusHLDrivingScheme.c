//
// Created by Ruijzendaal on 27/03/2018.
//

#define IUSLIBRARY_IMPLEMENTATION

#include <stdlib.h>
#include <include/ius.h>
#include <include/iusInput.h>
#include "iusInput.h"
#include "iusHLDrivingScheme.h"


IusDrivingScheme *iusHLCreateDrivingScheme
(
    IusDrivingSchemeType scheme,
    int numElements,
    IusTransducerShape shape, // determines whether to use 2D or 3D positions/Angles
    int numTransmitPulses
)
{
    IusDrivingScheme *baseDrivingScheme = NULL;
    baseDrivingScheme = (IusDrivingScheme *)calloc( 1, sizeof( IusDrivingScheme ) );
    //

    return baseDrivingScheme;
}

int iusHLDeleteDrivingScheme
(
    iuds_t drivingScheme
)
{
    return 0;
}

// Setters
int iusDrivingSchemeSetNumSamplesPerLine
(
    IusDrivingScheme * drivingScheme,
    int numSamplesPerLine
)
{
    drivingScheme->numSamplesPerLine = numSamplesPerLine;
    return IUS_E_OK;
}


