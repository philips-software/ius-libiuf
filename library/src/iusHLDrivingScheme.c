//
// Created by Ruijzendaal on 27/03/2018.
//

#define IUSLIBRARY_IMPLEMENTATION

#include <stdlib.h>
#include <include/ius.h>
#include <include/iusInput.h>
#include <include/iusError.h>
#include <math.h>
#include "iusInput.h"
#include "iusHLDrivingScheme.h"


IUS_BOOL isValidSchemeType(IusDrivingSchemeType type)
{
    switch(type)
    {
        case IUS_DIVERGING_WAVES_PARAMETRIZED:
        case IUS_DIVERGING_WAVES:
        case IUS_FOCUSED_WAVES_PARAMETRIZED:
        case IUS_FOCUSED_WAVES:
        case IUS_CUSTOM_WAVES:
        case IUS_SINGLE_ELEMENT:
        case IUS_PLANE_WAVES:
            return IUS_TRUE;
    }
    return IUS_FALSE;
}

IUS_BOOL isValidShape(IusShape shape)
{
    return (shape == IUS_2D_SHAPE || shape == IUS_3D_SHAPE);
}


IusDrivingScheme *iusHLCreateDrivingScheme
(
    IusDrivingSchemeType type,
    IusShape shape,       // determines whether to use 2D or 3D positions/Angles
    int numTransmitPulses,
    int numTransmitSources
)
{
    if( !isValidSchemeType(type) ) return IUDS_INVALID;
    if( !isValidShape(shape) ) return IUDS_INVALID;
    if( numTransmitPulses <= 0 ) return IUDS_INVALID;

    IusDrivingScheme *baseDrivingScheme = NULL;
    if( shape == IUS_2D_SHAPE )
    {
        Ius2DDrivingScheme *_2DDrivingScheme = (Ius2DDrivingScheme *)calloc( 1, sizeof( Ius2DDrivingScheme ) );
        _2DDrivingScheme->pSourceLocations = (Ius2DPosition *)calloc(numTransmitSources, sizeof(Ius2DPosition));
        baseDrivingScheme = (IusDrivingScheme *)_2DDrivingScheme;
    }

    if( shape == IUS_3D_SHAPE )
    {
        Ius3DDrivingScheme *_3DDrivingScheme = (Ius3DDrivingScheme *)calloc( 1, sizeof( Ius3DDrivingScheme ) );
        _3DDrivingScheme->pSourceLocations = (Ius3DPosition *)calloc(numTransmitSources, sizeof(Ius3DPosition));
        baseDrivingScheme = (IusDrivingScheme *)_3DDrivingScheme;
    }

    baseDrivingScheme->numTransmitSources = numTransmitSources;
    baseDrivingScheme->numTransmitPulses = numTransmitPulses;
    baseDrivingScheme->shape = shape;
    baseDrivingScheme->type = type;
    baseDrivingScheme->pTransmitPattern = (IusTransmitPattern *)calloc( numTransmitPulses, sizeof(IusTransmitPattern) );


    return baseDrivingScheme;
}

int iusHLDeleteDrivingScheme
(
    iuds_t drivingScheme
)
{
    if( drivingScheme != NULL )
    {
        free(drivingScheme);
        return IUS_E_OK;
    }
    return IUS_ERR_VALUE;
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



int iusDrivingSchemeSetSourceAngularDelta
(
    iuds_t drivingScheme,
    float angularDelta
)
{
    if( drivingScheme->shape != IUS_2D_SHAPE )
        return IUS_ERR_VALUE;
    ((Ius2DDrivingScheme *)drivingScheme)->sourceAngularDelta = angularDelta;
    return IUS_E_OK;
}

int iusDrivingSchemeSetSourceFNumber
(
    iuds_t drivingScheme,
    float FNumber
)
{
    if( drivingScheme->shape != IUS_2D_SHAPE )
        return IUS_ERR_VALUE;
    ((Ius2DDrivingScheme *)drivingScheme)->sourceFNumber = FNumber;
    return IUS_E_OK;
}

int iusDrivingSchemeSetSourceStartAngle
(
    iuds_t drivingScheme,
    float startAngle
)
{
    if( drivingScheme->shape != IUS_2D_SHAPE )
        return NAN;
    ((Ius2DDrivingScheme *)drivingScheme)->sourceStartAngle = startAngle;
    return IUS_E_OK;
}


// getters
IusShape iusHLDrivingSchemeGetShape
(
    iuds_t drivingScheme
)
{
    return drivingScheme->shape;
}

IusDrivingSchemeType iusHLDrivingSchemeGetType
(
    iuds_t drivingScheme
)
{
    return drivingScheme->type;
}

int iusHLDrivingSchemeGetNumTransmitPulses
(
    iuds_t drivingScheme
)
{
    return drivingScheme->numTransmitPulses;
}



int iusHLDrivingSchemeGetNumTransmitSources
(
    iuds_t drivingScheme
)
{
    return drivingScheme->numTransmitSources;
}

float iusDrivingSchemeGetSourceAngularDelta
(
    iuds_t drivingScheme
)
{
    if( drivingScheme->shape != IUS_2D_SHAPE )
        return NAN;
    return ((Ius2DDrivingScheme *)drivingScheme)->sourceAngularDelta;
}

float iusDrivingSchemeGetSourceFNumber
(
    iuds_t drivingScheme
)
{
    if( drivingScheme->shape != IUS_2D_SHAPE )
        return NAN;
    return ((Ius2DDrivingScheme *)drivingScheme)->sourceFNumber;
}

float iusDrivingSchemeGetSourceStartAngle
(
    iuds_t drivingScheme
)
{
    if( drivingScheme->shape != IUS_2D_SHAPE )
        return NAN;
    return ((Ius2DDrivingScheme *)drivingScheme)->sourceStartAngle;
}