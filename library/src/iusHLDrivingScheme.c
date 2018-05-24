//
// Created by Ruijzendaal on 27/03/2018.
//
#include <assert.h>
#include <stdlib.h>
#include <math.h>

#include <include/ius.h>
#include <iusUtil.h>
#include <include/iusError.h>
#include <include/iusTypes.h>

#include <include/iusHLDrivingScheme.h>

// ADT
/** \brief the driving scheme for an experiment */
struct IusDrivingScheme
{
    IusDrivingSchemeType          type;      /**< driving scheme: e.g. diveringwaves, planeswaves, ... */
    IusShape                      shape;
    int                           numTransmitSources;     /**< number of US sources (tyically these are virtual) */
    int                           numTransmitPulses;      /**< number of pulses in a frame == numPulsesPerFrame */
    int                           numElements;
    //int numFrames;                                  /**< number of repetitions of the driving pattern */

// TODO    IusFrameList *                pTransmitFrames;          /**< array of numFrames */
    iutpal_t      pTransmitPatterns;       /**< array (time, index) of length numTransmitPulses */
    iupl_t       pTransmitPulses;          /**< waveform of the transmit pulse */
    iusl_t       pSourceLocations;
//TODO    IusChannelMapList *           pChannelMaps;
    iual_t        pApodizations;   /**< 2D array: per transmit event we have numElements gains */
//TODO    IusTimeGainControlList *      pTimeGainControls;
} ;

// forward declarations
static IUS_BOOL isValidSchemeType
(
    IusDrivingSchemeType type
);

static IUS_BOOL isValidShape
(
    IusShape shape
);


iuds_t iusHLDrivingSchemeCreate
(
IusDrivingSchemeType type,
IusShape shape,       // determines whether to use 2D or 3D positions/Angles
iusl_t transmitSources,
iupl_t transmitPulses,
iutpal_t transmitPatterns,
iual_t apodizations,
int numElements
)
{
    if( !isValidSchemeType(type) ) return IUDS_INVALID;
    if( !isValidShape(shape) ) return IUDS_INVALID;
    if( transmitSources == NULL ) return IUDS_INVALID;
    if( transmitPulses == NULL ) return IUDS_INVALID;
    if( transmitPatterns == NULL ) return IUDS_INVALID;
    if( apodizations == NULL ) return IUDS_INVALID;
    if( numElements <= 0 ) return IUDS_INVALID;

    IusDrivingScheme *baseDrivingScheme = calloc(1,sizeof(IusDrivingScheme));

    if( baseDrivingScheme == NULL ) return IUDS_INVALID;

    // TransmitPatterns
    baseDrivingScheme->pSourceLocations = transmitSources;
    baseDrivingScheme->pTransmitPulses = transmitPulses;
    baseDrivingScheme->pTransmitPatterns = transmitPatterns;
    baseDrivingScheme->pApodizations = apodizations;
    baseDrivingScheme->numElements = numElements;
    baseDrivingScheme->numTransmitSources = iusHLSourceListGetSize( transmitSources );
    baseDrivingScheme->numTransmitPulses = iusHLPulseListGetSize(transmitPulses);
    baseDrivingScheme->shape = shape;
    baseDrivingScheme->type = type;
    return baseDrivingScheme;
}

int iusHLDrivingSchemeDelete
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



IUS_BOOL iusHLDrivingSchemeCompare
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

    if( iusCompareTransmitPatternList(
        reference->pTransmitPatterns,
        actual->pTransmitPatterns) == IUS_FALSE) return IUS_FALSE;

    if(iusHLApodizationListCompare(
    reference->pApodizations,
    actual->pApodizations) == IUS_FALSE) return IUS_FALSE;

    if(iusHLPulseListCompare(
    reference->pTransmitPulses,
    actual->pTransmitPulses) == IUS_FALSE) return IUS_FALSE;

    // TODO
//    if( iusHLCompareSourceList(
//        reference->pSources,
//        actual->pSources) == IUS_FALSE) return IUS_FALSE;

//    assert(IUS_FALSE);
    return IUS_TRUE;
}


int iusHLDrivingSchemeSetTransmitPulse
(
    iuds_t drivingScheme,
    iup_t *transmitPulse
)
{
    // Todo: use pulse list to alter transnit pulses
    assert(IUS_FALSE);
    return IUS_E_OK;
}
//
//
//int iusHLDrivingSchemeSet3DSourceLocation
//(
//    IusDrivingScheme *drivingScheme,
//    Ius3DPosition *position,
//    int index
//)
//{
//    Ius3DDrivingScheme *castedScheme = (Ius3DDrivingScheme *)drivingScheme;
//    if( index >= castedScheme->base.numTransmitSources || index < 0 )
//        return IUS_ERR_VALUE;
//    castedScheme->pSources[index]=*position;
//    return IUS_E_OK;
//}
//
//int iusHLDrivingSchemeSet2DSourceLocation
//(
//    IusDrivingScheme *drivingScheme,
//    Ius2DPosition *position,
//    int index
//)
//{
//    Ius2DDrivingScheme *castedScheme = (Ius2DDrivingScheme *)drivingScheme;
//    if( index >= castedScheme->base.numTransmitSources || index < 0 )
//        return IUS_ERR_VALUE;
//    castedScheme->pSources[index]=*position;
//    return IUS_E_OK;
//}


int iusHLDrivingSchemeSetTransmitApodization
(
iuds_t drivingScheme,
float apodization,
int apodizationIndex,
int elementIndex
)
{
    // TODO: check if still valid
    if( apodizationIndex >= drivingScheme->numTransmitPulses || apodizationIndex < 0 )
        return IUS_ERR_VALUE;
    if( elementIndex >= drivingScheme->numElements || elementIndex < 0 )
        return IUS_ERR_VALUE;
    if( apodization < 0.0f || apodization > 1.0f )
        return IUS_ERR_VALUE;
    iusHLApodizationListSetApodization(drivingScheme->pApodizations, apodizationIndex, elementIndex, apodization);
    return IUS_E_OK;
}
//
//int iusDrivingSchemeSetSourceStartPhi
//(
//    iuds_t drivingScheme,
//    float startPhi
//)
//{
//    if( drivingScheme->shape != IUS_3D_SHAPE )
//        return IUS_ERR_VALUE;
//    ((Ius3DDrivingScheme *)drivingScheme)->sourceStartPhi = startPhi;
//    return IUS_E_OK;
//}
//
//int iusDrivingSchemeSetSourceDeltaPhi
//(
//    iuds_t drivingScheme,
//    float deltaPhi
//)
//{
//    if( drivingScheme->shape != IUS_3D_SHAPE )
//        return IUS_ERR_VALUE;
//    ((Ius3DDrivingScheme *)drivingScheme)->sourceDeltaPhi = deltaPhi;
//    return IUS_E_OK;
//}
//
//int iusDrivingSchemeSetSourceStartTheta
//(
//    iuds_t drivingScheme,
//    float startTheta
//)
//{
//    if( drivingScheme->shape == IUS_3D_SHAPE )
//    {
//        ((Ius3DDrivingScheme *)drivingScheme)->sourceStartTheta = startTheta;
//        return IUS_E_OK;
//    }
//
//    if( drivingScheme->shape == IUS_2D_SHAPE )
//    {
//        ((Ius2DDrivingScheme *)drivingScheme)->sourceStartTheta = startTheta;
//        return IUS_E_OK;
//    }
//    return IUS_ERR_VALUE;
//}
//
//int iusDrivingSchemeSetSourceDeltaTheta
//(
//    iuds_t drivingScheme,
//    float deltaTheta
//)
//{
//    if( drivingScheme->shape == IUS_3D_SHAPE )
//    {
//        ((Ius3DDrivingScheme *)drivingScheme)->sourceDeltaTheta = deltaTheta;
//        return IUS_E_OK;
//    }
//
//    if( drivingScheme->shape == IUS_2D_SHAPE )
//    {
//        ((Ius2DDrivingScheme *)drivingScheme)->sourceDeltaTheta = deltaTheta;
//        return IUS_E_OK;
//    }
//    return IUS_ERR_VALUE;
//}
//
//int iusDrivingSchemeSetSourceFNumber
//(
//    iuds_t drivingScheme,
//    float FNumber
//)
//{
//    if( drivingScheme->type != IUS_DIVERGING_WAVES_PARAMETRIZED && drivingScheme->type != IUS_FOCUSED_WAVES_PARAMETRIZED )
//        return IUS_ERR_VALUE;
//    ((Ius2DDrivingScheme *)drivingScheme)->sourceFNumber = FNumber;
//    return IUS_E_OK;
//}
//


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


/*
float iusDrivingSchemeGetSourceFNumber
(
    iuds_t drivingScheme
)
{
    if( drivingScheme->shape != IUS_2D_SHAPE )
        return NAN;
    return ((Ius2DDrivingScheme *)drivingScheme)->sourceFNumber;
}
*/


float iusHLDrivingSchemeGetTransmitApodization
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
    return iusHLApodizationListGetApodization(drivingScheme->pApodizations, pulseIndex, elementIndex);
}

int iusHLDrivingSchemeGetNumChannels
(
    iuds_t drivingScheme
)
{
    return 0;
}


//iu3dp_t iusHLDrivingSchemeGet3DSourceLocation
//(
//    iuds_t drivingScheme,
//    int elementIndex
//)
//{
//    Ius3DDrivingScheme *castedScheme = (Ius3DDrivingScheme *) drivingScheme;
//    if( elementIndex >= castedScheme->base.numTransmitSources || elementIndex < 0 )
//        return NULL;
//    return &castedScheme->pSources[elementIndex];
//}
//
//float iusDrivingSchemeGetSourceStartTheta
//(
//    iuds_t drivingScheme
//)
//{
//    if( drivingScheme->shape == IUS_3D_SHAPE )
//        return ((Ius3DDrivingScheme *)drivingScheme)->sourceStartTheta;
//    if( drivingScheme->shape == IUS_2D_SHAPE )
//        return ((Ius2DDrivingScheme *)drivingScheme)->sourceStartTheta;
//    return NAN;
//}
//
//float iusDrivingSchemeGetSourceDeltaTheta
//(
//    iuds_t drivingScheme
//)
//{
//    if( drivingScheme->shape == IUS_3D_SHAPE )
//        return ((Ius3DDrivingScheme *)drivingScheme)->sourceDeltaTheta;
//    if( drivingScheme->shape == IUS_2D_SHAPE )
//        return ((Ius2DDrivingScheme *)drivingScheme)->sourceDeltaTheta;
//    return NAN;
//}

iutpal_t iusHLDrivingSchemeGetTransmitPatternList
(
    iuds_t drivingScheme
)
{
    return drivingScheme->pTransmitPatterns;
}

iusl_t iusHLDrivingSchemeGetSourceLocationList
(
    iuds_t drivingScheme
)
{
    return drivingScheme->pSourceLocations;
}


// util


static IUS_BOOL isValidSchemeType
(
    IusDrivingSchemeType type
)
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

        case IUS_INVALID_DRIVING_SCHEME:
            return IUS_FALSE;
    }
    return IUS_FALSE;
}

static IUS_BOOL isValidShape
(
    IusShape shape
)
{
    return (shape == IUS_2D_SHAPE || shape == IUS_3D_SHAPE);
}