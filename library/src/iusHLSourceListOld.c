//
// Created by Ruijzendaal on 16/04/2018.
//
#include <stddef.h>
#include <stdlib.h>

#include <include/ius.h>
#include <include/iusError.h>
#include <include/iusTypes.h>
#include <include/iusHLSourceListOld.h>
#include <include/iusUtil.h>

struct IusSourceList {
    IusSourceType locationType;           // Parametric2D/3D, NonParametric2D/3D locationtypes
    int count;
};

typedef struct {
    iusl_t base;           // Parametric2D/3D, NonParametric2D/3D locationtypes
    Ius2DPosition *pSources;
} Ius2DSourceList;

typedef struct {
    Ius2DSourceList base2d;
    float sourceFNumber;          /**< distance in [m] of sources to transducer for POLAR */
    float sourceDeltaTheta;     /**< angle in [rad] between sources */
    float sourceStartTheta;       /**< angle in [rad] between sources */
} IusParametric2DSourceList;

typedef struct {
    iusl_t base;           // Parametric2D/3D, NonParametric2D/3D locationtypes
    Ius3DPosition *pSources;
} Ius3DSourceList;

typedef struct {
    Ius3DSourceList base3d;
    float sourceFNumber;          /**< distance in [m] of sources to transducer for POLAR */
    float sourceDeltaTheta;     /**< angle in [rad] between sources */
    float sourceStartTheta;       /**< angle in [rad] between sources */
    float sourceDeltaPhi;     /**< angle in [rad] between sources */
    float sourceStartPhi;       /**< angle in [rad] between sources */
} IusParametric3DSourceList;

iusl_t iusCreate2DSourceList
(
    int numTransmitSources
)
{
    Ius2DSourceList *sources = calloc( 1, sizeof( Ius2DSourceList ));
    if (sources != NULL) {
        sources->pSources = calloc( numTransmitSources, sizeof( Ius2DPosition ));
    }
    if (sources->pSources == NULL) {
        return NULL;
    }
    return (iusl_t) sources;
}

IusParametric2DSourceList *iusCreateParametric2DSourceList
(
    int numTransmitSources
)
{
    IusParametric2DSourceList *sources = calloc( 1, sizeof( IusParametric2DSourceList ));
    if (sources != NULL) {
        sources->base2d.pSources = calloc( numTransmitSources, sizeof( Ius2DPosition ));
    }
    if (sources->base2d.pSources == NULL) {
        return NULL;
    }
    return sources;
}


Ius3DSourceList *iusCreate3DSourceList
(
    int numTransmitSources
)
{
    Ius3DSourceList *sources = calloc( 1, sizeof( Ius3DSourceList ));
    if (sources != NULL) {
        sources->pSources = calloc( numTransmitSources, sizeof( Ius3DPosition ));
    }
    if (sources->pSources == NULL) {
        return NULL;
    }
    return sources;
}

IusParametric3DSourceList *iusCreateParametric3DSourceList
(
    int numTransmitSources
)
{
    IusParametric3DSourceList *sources = calloc( 1, sizeof( IusParametric3DSourceList ));
    if (sources != NULL) {
        sources->base3d.pSources = calloc( numTransmitSources, sizeof( Ius3DPosition ));
    }
    if (sources->base3d.pSources == NULL) {
        return NULL;
    }
    return sources;
}

iusl_t iusHLCreateSourceList
(
    IusSourceType locationType,
    int numTransmitSources
)
{
    iusl_t locationList = NULL;
    if (numTransmitSources <= 0) return IUSLL_INVALID;

    switch (locationType) {
        case IUS_2D_SOURCE_LOCATION:
            locationList = (iusl_t) iusCreate2DSourceList( numTransmitSources );
            break;
        case IUS_3D_SOURCE_LOCATION:
            locationList = (iusl_t) iusCreate3DSourceList( numTransmitSources );
            break;
        case IUS_PARAMETRIC_2D_SOURCE_LOCATION:
            locationList = (iusl_t) iusCreateParametric2DSourceList( numTransmitSources );
            break;
        case IUS_PARAMETRIC_3D_SOURCE_LOCATION:
            locationList = (iusl_t) iusCreateParametric3DSourceList( numTransmitSources );
            break;
        default:
            break;
    }

    // base members
    if (locationList != NULL) {
        locationList->count = numTransmitSources;
        locationList->locationType = locationType;
    }
    return locationList;
}


int iusHLDeleteSourceList
(
    iusl_t list
)
{
    int status = 0;
    if (list == NULL) return IUS_ERR_VALUE;

    switch (list->locationType) {
        case IUS_2D_SOURCE_LOCATION:
            free(((Ius2DSourceList *) list)->pSources );
            free(((Ius2DSourceList *) list));
            break;
        case IUS_3D_SOURCE_LOCATION:
            free(((Ius3DSourceList *) list)->pSources );
            free(((Ius3DSourceList *) list));
            break;
        case IUS_PARAMETRIC_2D_SOURCE_LOCATION:
            free(((IusParametric2DSourceList *) list)->base2d.pSources );
            free(((IusParametric2DSourceList *) list));
            break;
        case IUS_PARAMETRIC_3D_SOURCE_LOCATION:
            free(((IusParametric3DSourceList *) list)->base3d.pSources );
            free(((IusParametric3DSourceList *) list));
            break;
        default:
            break;
    }

    return status;
}

IUS_BOOL compare2DSourceList
(
    Ius2DSourceList *reference,
    Ius2DSourceList *actual
)
{
    int index;
    for (index = 0; index < actual->base->count; index++) {
        if (iusHLCompare2DPosition( &reference->pSources[index], &actual->pSources[index] ) ==
            IUS_FALSE)
            return IUS_FALSE;
    }
    return IUS_TRUE;
}

IUS_BOOL compare3DSourceList
(
    Ius3DSourceList *reference,
    Ius3DSourceList *actual
)
{
    int index;
    for (index = 0; index < actual->base->count; index++) {
        if (iusHLCompare3DPosition( &reference->pSources[index], &actual->pSources[index] ) ==
            IUS_FALSE)
            return IUS_FALSE;
    }
    return IUS_TRUE;
}

IUS_BOOL compareParametric2DSourceList
(
    IusParametric2DSourceList *reference,
    IusParametric2DSourceList *actual
)
{
    int index;
    if (IUS_EQUAL_FLOAT( reference->sourceDeltaTheta, actual->sourceDeltaTheta ) == IUS_FALSE)
        return IUS_FALSE;
    if (IUS_EQUAL_FLOAT( reference->sourceFNumber, actual->sourceFNumber ) == IUS_FALSE)
        return IUS_FALSE;
    if (IUS_EQUAL_FLOAT( reference->sourceStartTheta, actual->sourceStartTheta ) == IUS_FALSE)
        return IUS_FALSE;
    for (index = 0; index < actual->base2d.base->count; index++) {
        if (iusHLCompare2DPosition( &reference->base2d.pSources[index],
                                    &actual->base2d.pSources[index] ) == IUS_FALSE)
            return IUS_FALSE;
    }
    return IUS_TRUE;
}

IUS_BOOL compareParametric3DSourceList
(
    IusParametric3DSourceList *reference,
    IusParametric3DSourceList *actual
)
{
    int index;
    if (IUS_EQUAL_FLOAT( reference->sourceDeltaTheta, actual->sourceDeltaTheta ) == IUS_FALSE)
        return IUS_FALSE;
    if (IUS_EQUAL_FLOAT( reference->sourceFNumber, actual->sourceFNumber ) == IUS_FALSE)
        return IUS_FALSE;
    if (IUS_EQUAL_FLOAT( reference->sourceStartTheta, actual->sourceStartTheta ) == IUS_FALSE)
        return IUS_FALSE;
    if (IUS_EQUAL_FLOAT( reference->sourceDeltaPhi, actual->sourceDeltaPhi ) == IUS_FALSE)
        return IUS_FALSE;
    if (IUS_EQUAL_FLOAT( reference->sourceStartPhi, actual->sourceStartPhi ) == IUS_FALSE)
        return IUS_FALSE;
    for (index = 0; index < actual->base3d.base->count; index++) {
        if (iusHLCompare3DPosition( &reference->base3d.pSources[index],
                                    &actual->base3d.pSources[index] ) == IUS_FALSE)
            return IUS_FALSE;
    }
    return IUS_TRUE;
}

IUS_BOOL iusHLCompareSourceList
(
    iusl_t reference,
    iusl_t actual
)
{
    if (reference == actual)
        return IUS_TRUE;
    if (reference == NULL || actual == NULL)
        return IUS_FALSE;
    if (reference->count != actual->count)
        return IUS_FALSE;
    if (reference->locationType != actual->locationType)
        return IUS_FALSE;

    switch (reference->locationType) {
        case IUS_2D_SOURCE_LOCATION:
            return compare2DSourceList((Ius2DSourceList *) reference, (Ius2DSourceList *) actual );
        case IUS_3D_SOURCE_LOCATION:
            return compare3DSourceList((Ius3DSourceList *) reference, (Ius3DSourceList *) actual );
        case IUS_PARAMETRIC_2D_SOURCE_LOCATION:
            return compareParametric2DSourceList((IusParametric2DSourceList *) reference,
                                                 (IusParametric2DSourceList *) actual );
        case IUS_PARAMETRIC_3D_SOURCE_LOCATION:
            return compareParametric3DSourceList((IusParametric3DSourceList *) reference,
                                                 (IusParametric3DSourceList *) actual );
        default:
            break;
    }
    return IUS_FALSE;
}

// getters
int iusHLSourceListGetSize
    (
        iusl_t list
    )
{
    if (list == NULL) return -1;
    return list->count;
}

// setters
int iusHLSourceListSet2DPosition
    (
        iusl_t list,
        iu2dp_t pos,
        int index
    )
{
    int status = IUS_ERR_VALUE;
    if (list == NULL) return IUS_ERR_VALUE;
    if (pos == NULL) return IUS_ERR_VALUE;
    if (index >= list->count) return IUS_ERR_VALUE;

    if (list->locationType == IUS_PARAMETRIC_2D_SOURCE_LOCATION) {
        ((IusParametric2DSourceList *) list)->base2d.pSources[index] = *pos;
        status = IUS_E_OK;
    }

    if (list->locationType == IUS_2D_SOURCE_LOCATION) {
        ((IusParametric2DSourceList *) list)->base2d.pSources[index] = *pos;
        status = IUS_E_OK;
    }

    return status;
}


int iusHLSourceListSet3DPosition
    (
        iusl_t list,
        iu3dp_t pos,
        int index
    )
{
    int status = IUS_ERR_VALUE;
    if (list == NULL) return IUS_ERR_VALUE;
    if (pos == NULL) return IUS_ERR_VALUE;
    if (index >= list->count) return IUS_ERR_VALUE;

    if (list->locationType == IUS_PARAMETRIC_3D_SOURCE_LOCATION) {
        ((IusParametric3DSourceList *) list)->base3d.pSources[index] = *pos;
        status = IUS_E_OK;
    }

    if (list->locationType == IUS_3D_SOURCE_LOCATION) {
        ((IusParametric3DSourceList *) list)->base3d.pSources[index] = *pos;
        status = IUS_E_OK;
    }

    return status;
}