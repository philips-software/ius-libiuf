//
// Created by Ruijzendaal on 27/03/2018.
//

#define IUSLIBRARY_IMPLEMENTATION

#include <stdlib.h>
#include <include/ius.h>
#include <include/iusInput.h>
#include <include/iusError.h>
#include <math.h>
#include <include/iusHLPosition.h>
#include <assert.h>
#include <include/iusUtil.h>
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

IusTransmitPulse *iusHLCreateParametricPulse
(
    float pulseFrequency,
    float pulseAmplitude,
    int   pulseCount
)
{
    IusParametricTransmitPulse *pulse = (IusParametricTransmitPulse *) calloc (1,sizeof(IusParametricTransmitPulse));
    pulse->pulseAmplitude = pulseAmplitude;
    pulse->pulseCount = pulseCount;
    pulse->pulseFrequency = pulseFrequency;
    pulse->base.type = IUS_PARAMETRIC_PULSETYPE;
    return (IusTransmitPulse *)pulse;
}


IusDrivingScheme * iusCreate2DDrivingScheme
(
    int numTransmitSources
)
{
    Ius2DDrivingScheme *_2DDrivingScheme = (Ius2DDrivingScheme *)calloc( 1, sizeof( Ius2DDrivingScheme ) );
    if( _2DDrivingScheme == NULL ) return NULL;

    _2DDrivingScheme->pSourceLocations = (Ius2DPosition *)calloc(numTransmitSources, sizeof(Ius2DPosition));
    if( _2DDrivingScheme->pSourceLocations == NULL )
    {
        free(_2DDrivingScheme);
        return NULL;
    }
    return (IusDrivingScheme *)_2DDrivingScheme;
}

IusDrivingScheme * iusCreate3DDrivingScheme
(
    int numTransmitSources
)
{
    Ius3DDrivingScheme *_3DDrivingScheme = (Ius3DDrivingScheme *)calloc( 1, sizeof( Ius3DDrivingScheme ) );
    if( _3DDrivingScheme == NULL ) return NULL;

    _3DDrivingScheme->pSourceLocations = (Ius3DPosition *)calloc(numTransmitSources, sizeof(Ius3DPosition));
    if( _3DDrivingScheme->pSourceLocations == NULL )
    {
        free(_3DDrivingScheme);
        return NULL;
    }
    return (IusDrivingScheme *)_3DDrivingScheme;
}

float *iusCreateTransmitApodization(int numTransmitPulses,int numElements)
{
    // 2D array: per transmitted pulse we have numElement gains
    // (which are numTransducerElements or numChannelElements)
    float *pTransmitApodization = (float *)calloc( numTransmitPulses * numElements, sizeof(float) );
    if( pTransmitApodization == NULL ) return NULL;
    for (int i = 0; i < numElements*numTransmitPulses; ++i)
    {
        pTransmitApodization[i] = 1.0f;
    }
    return pTransmitApodization;
};

IusDrivingScheme *iusHLCreateDrivingScheme
(
    IusDrivingSchemeType type,
    IusShape shape,       // determines whether to use 2D or 3D positions/Angles
    int numTransmitPulses,
    int numTransmitSources,
    int numElements
)
{
    if( !isValidSchemeType(type) ) return IUDS_INVALID;
    if( !isValidShape(shape) ) return IUDS_INVALID;
    if( numTransmitPulses <= 0 ) return IUDS_INVALID;
    if( numElements <= 0 ) return IUDS_INVALID;

    IusDrivingScheme *baseDrivingScheme = NULL;
    if( shape == IUS_2D_SHAPE )
    {
        baseDrivingScheme = iusCreate2DDrivingScheme(numTransmitSources);
    }

    if( shape == IUS_3D_SHAPE )
    {
        baseDrivingScheme = iusCreate3DDrivingScheme(numTransmitSources);
    }

    // Apodization
    if( baseDrivingScheme == NULL ) return NULL;
    baseDrivingScheme->pTransmitApodization = iusCreateTransmitApodization(numTransmitPulses , numElements);
    if ( baseDrivingScheme->pTransmitApodization == NULL )
    {
        iusHLDeleteDrivingScheme(baseDrivingScheme);
        return NULL;
    }

    // TransmitPatterns
    baseDrivingScheme->pTransmitPatterns = iusHLCreateTransmitPatternList(numTransmitPulses);

    baseDrivingScheme->numElements = numElements;
    baseDrivingScheme->numTransmitSources = numTransmitSources;
    baseDrivingScheme->numTransmitPulses = numTransmitPulses;
    baseDrivingScheme->shape = shape;
    baseDrivingScheme->type = type;
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

int iusHLDrivingSchemeSetTransmitPulse
(
    iuds_t drivingScheme,
    iutp_t transmitPulse
)
{
    // Todo: use pulse list to alter transnit pulses
    assert(IUS_FALSE);
    return IUS_E_OK;
}


int iusHLDrivingSchemeSet3DSourceLocation
(
    IusDrivingScheme *drivingScheme,
    Ius3DPosition *position,
    int index
)
{
    Ius3DDrivingScheme *castedScheme = (Ius3DDrivingScheme *)drivingScheme;
    if( index >= castedScheme->base.numTransmitSources || index < 0 )
        return IUS_ERR_VALUE;
    castedScheme->pSourceLocations[index]=*position;
    return IUS_E_OK;
}

int iusHLDrivingSchemeSet2DSourceLocation
(
    IusDrivingScheme *drivingScheme,
    Ius2DPosition *position,
    int index
)
{
    Ius2DDrivingScheme *castedScheme = (Ius2DDrivingScheme *)drivingScheme;
    if( index >= castedScheme->base.numTransmitSources || index < 0 )
        return IUS_ERR_VALUE;
    castedScheme->pSourceLocations[index]=*position;
    return IUS_E_OK;
}


int iusDrivingSchemeSetTransmitApodization
    (
        iuds_t drivingScheme,
        float apodization,
        int pulseIndex,
        int elementIndex
    )
{
    if( pulseIndex >= drivingScheme->numTransmitPulses || pulseIndex < 0 )
        return IUS_ERR_VALUE;
    if( elementIndex >= drivingScheme->numElements || elementIndex < 0 )
        return IUS_ERR_VALUE;
    if( apodization < 0.0f || apodization > 1.0f )
        return IUS_ERR_VALUE;
    drivingScheme->pTransmitApodization[pulseIndex*drivingScheme->numElements+elementIndex]=apodization;
    return IUS_E_OK;
}

int iusDrivingSchemeSetSourceStartPhi
(
    iuds_t drivingScheme,
    float startPhi
)
{
    if( drivingScheme->shape != IUS_3D_SHAPE )
        return IUS_ERR_VALUE;
    ((Ius3DDrivingScheme *)drivingScheme)->sourceStartPhi = startPhi;
    return IUS_E_OK;
}

int iusDrivingSchemeSetSourceDeltaPhi
(
    iuds_t drivingScheme,
    float deltaPhi
)
{
    if( drivingScheme->shape != IUS_3D_SHAPE )
        return IUS_ERR_VALUE;
    ((Ius3DDrivingScheme *)drivingScheme)->sourceDeltaPhi = deltaPhi;
    return IUS_E_OK;
}

int iusDrivingSchemeSetSourceStartTheta
(
    iuds_t drivingScheme,
    float startTheta
)
{
    if( drivingScheme->shape == IUS_3D_SHAPE )
    {
        ((Ius3DDrivingScheme *)drivingScheme)->sourceStartTheta = startTheta;
        return IUS_E_OK;
    }

    if( drivingScheme->shape == IUS_2D_SHAPE )
    {
        ((Ius2DDrivingScheme *)drivingScheme)->sourceStartTheta = startTheta;
        return IUS_E_OK;
    }
    return IUS_ERR_VALUE;
}

int iusDrivingSchemeSetSourceDeltaTheta
(
    iuds_t drivingScheme,
    float deltaTheta
)
{
    if( drivingScheme->shape == IUS_3D_SHAPE )
    {
        ((Ius3DDrivingScheme *)drivingScheme)->sourceDeltaTheta = deltaTheta;
        return IUS_E_OK;
    }

    if( drivingScheme->shape == IUS_2D_SHAPE )
    {
        ((Ius2DDrivingScheme *)drivingScheme)->sourceDeltaTheta = deltaTheta;
        return IUS_E_OK;
    }
    return IUS_ERR_VALUE;
}

int iusDrivingSchemeSetSourceFNumber
(
    iuds_t drivingScheme,
    float FNumber
)
{
    if( drivingScheme->type != IUS_DIVERGING_WAVES_PARAMETRIZED && drivingScheme->type != IUS_FOCUSED_WAVES_PARAMETRIZED )
        return IUS_ERR_VALUE;
    ((Ius2DDrivingScheme *)drivingScheme)->sourceFNumber = FNumber;
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


int iusHLDrivingSchemeGetNumElements
(
    iuds_t drivingScheme
)
{
    return drivingScheme->numElements;
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


float iusDrivingSchemeGetTransmitApodization
(
    iuds_t drivingScheme,
    int pulseIndex,
    int elementIndex
)
{
    if( pulseIndex >= drivingScheme->numTransmitPulses || pulseIndex < 0 )
        return NAN;
    if( elementIndex >= drivingScheme->numElements || elementIndex < 0 )
        return NAN;
    return drivingScheme->pTransmitApodization[pulseIndex*drivingScheme->numElements+elementIndex];
}

iu3dp_t iusHLDrivingSchemeGet3DSourceLocation
(
    iuds_t drivingScheme,
    int elementIndex
)
{
    Ius3DDrivingScheme *castedScheme = (Ius3DDrivingScheme *) drivingScheme;
    if( elementIndex >= castedScheme->base.numTransmitSources || elementIndex < 0 )
        return NULL;
    return &castedScheme->pSourceLocations[elementIndex];
}

float iusDrivingSchemeGetSourceStartTheta
(
    iuds_t drivingScheme
)
{
    if( drivingScheme->shape == IUS_3D_SHAPE )
        return ((Ius3DDrivingScheme *)drivingScheme)->sourceStartTheta;
    if( drivingScheme->shape == IUS_2D_SHAPE )
        return ((Ius2DDrivingScheme *)drivingScheme)->sourceStartTheta;
    return NAN;
}

float iusDrivingSchemeGetSourceDeltaTheta
(
    iuds_t drivingScheme
)
{
    if( drivingScheme->shape == IUS_3D_SHAPE )
        return ((Ius3DDrivingScheme *)drivingScheme)->sourceDeltaTheta;
    if( drivingScheme->shape == IUS_2D_SHAPE )
        return ((Ius2DDrivingScheme *)drivingScheme)->sourceDeltaTheta;
    return NAN;
}

IusTransmitPatternList *iusHLDrivingSchemeGetTransmitPatternList
(
    iuds_t drivingScheme
)
{
    return drivingScheme->pTransmitPatterns;
}



IUS_BOOL iusHLCompareDrivingScheme
(
    iuds_t reference,
    iuds_t actual
)
{
    IUS_BOOL isEqual = IUS_FALSE;
    if( reference == actual ) return IUS_TRUE;
    if( reference == NULL || actual == NULL ) return IUS_FALSE;
    if( reference->type != actual->type ) return IUS_FALSE;
    if( reference->shape != actual->shape ) return IUS_FALSE;
    if( reference->numElements != actual->numElements ) return IUS_FALSE;
    if( reference->numTransmitPulses != actual->numTransmitPulses ) return IUS_FALSE;
    if( reference->numTransmitSources != actual->numTransmitSources ) return IUS_FALSE;
    if( IUS_EQUAL_FLOAT(reference->transmitPatternDelay, actual->transmitPatternDelay) == IUS_FALSE) return IUS_FALSE;
    if( iusCompareTransmitPatternList(
        reference->pTransmitPatterns,
        actual->pTransmitPatterns) == IUS_FALSE) return IUS_FALSE;


    assert(IUS_FALSE);
    return IUS_TRUE;
}

