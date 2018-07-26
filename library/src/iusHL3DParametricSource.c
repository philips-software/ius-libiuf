
//
// Created by nlv09165 on 25/07/2018.
//
#include <stdlib.h>

#include <ius.h>
#include <iusError.h>
#include <iusTypes.h>
#include <iusUtil.h>

#include <include/iusHLSourceImp.h>
#include <memory.h>
#include "include/iusHL3DParametricSource.h"

struct Ius3DParametricSource
{
    struct IusSource base;

    float fNumber;          /**< distance in [m] of sources to transducer for POLAR */
    float deltaTheta;     /**< angle in [rad] between sources */
    float startTheta;       /**< angle in [rad] between sources */
    float deltaPhi;          /**< angle in [rad] between sources */
    float startPhi;       /**< angle in [rad] between sources */
} ;



// ADT

iu3dps_t iusHL3DParametricSourceCreate
(
    char *pLabel,
    float fNumber,
    float deltaTheta,
    float startTheta,
    float deltaPhi,
    float startPhi
)
{
    if( pLabel == NULL ) return NULL;
    if( strcmp(pLabel,"") == 0 ) return NULL;

    iu3dps_t created = calloc(1,sizeof(Ius3DParametricSource));
    created->base.type = IUS_3D_PARAMETRIC_SOURCE;
    created->base.label = strdup(pLabel);
    created->deltaTheta = deltaTheta;
    created->startTheta = startTheta;
    created->deltaPhi = deltaPhi;
    created->startPhi = startPhi;
    created->fNumber = fNumber;
    return created;
}

int iusHL3DParametricSourceDelete
(
    iu3dps_t ius3DParametricSource
)
{
    int status = IUS_ERR_VALUE;
    if(ius3DParametricSource != NULL)
    {
        free(ius3DParametricSource);
        ius3DParametricSource = NULL;
        status = IUS_E_OK;
    }
    return status;
}


// operations
int iusHL3DParametricSourceCompare
(
    iu3dps_t reference,
    iu3dps_t actual
)
{
    if( reference == actual ) return IUS_TRUE;
    if( reference == NULL || actual == NULL ) return IUS_FALSE;
    if( IUS_EQUAL_FLOAT(reference->fNumber, actual->fNumber) == IUS_FALSE ) return IUS_FALSE;
    if( IUS_EQUAL_FLOAT(reference->startPhi, actual->startPhi) == IUS_FALSE ) return IUS_FALSE;
    if( IUS_EQUAL_FLOAT(reference->deltaPhi, actual->deltaPhi) == IUS_FALSE ) return IUS_FALSE;
    if( IUS_EQUAL_FLOAT(reference->startTheta, actual->startTheta) == IUS_FALSE ) return IUS_FALSE;
    if( IUS_EQUAL_FLOAT(reference->deltaTheta, actual->deltaTheta) == IUS_FALSE ) return IUS_FALSE;
    return iusHLBaseSourceCompare((ius_t)reference,(ius_t)actual);
}

// Getters
