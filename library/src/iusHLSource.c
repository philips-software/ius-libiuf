
//
// Created by nlv09165 on 24/07/2018.
//
#include <stdlib.h>
#include <stdarg.h>
#define IUSLIBRARY_IMPLEMENTATION

#include <ius.h>
#include <iusError.h>
#include <iusTypes.h>
#include <iusUtil.h>
#include <string.h>
#include <include/iusHL3DNonParametricSource.h>
#include <include/iusHL2DParametricSource.h>
#include <include/iusHL2DNonParametricSource.h>

#include "include/iusHLSourceImp.h"
#include "include/iusHL3DParametricSource.h"


// ADT

ius_t iusHLSourceCreate
(
    IusSourceType sourceType,
    char *pSourceLabel,
    int numLocations,
    ...
)
{
    va_list ap;
    ius_t source = IUS_INVALID;
    va_start(ap,numLocations);
    if (sourceType == IUS_2D_PARAMETRIC_SOURCE)
    {
        float fNumber = va_arg(ap,double);
        float angularDelta = va_arg(ap,double);
        float startAngle = va_arg(ap,double);
        iu2dps_t obj = iusHL2DParametricSourceCreate(pSourceLabel,
                                                     numLocations,
                                                     fNumber,
                                                     angularDelta,
                                                     startAngle);
        source = (ius_t) obj;
    }
    if (sourceType == IUS_3D_PARAMETRIC_SOURCE)
    {
        float fNumber = va_arg(ap,double);
        float angularDelta = va_arg(ap,double);
        float startAngle = va_arg(ap,double);
        float deltaPhi = va_arg(ap,double);
        float startPhi = va_arg(ap,double);
        iu3dps_t obj = iusHL3DParametricSourceCreate(pSourceLabel,
                                                    numLocations,
                                                    fNumber,
                                                    angularDelta,
                                                    startAngle,
                                                    deltaPhi,
                                                    startPhi);
        source = (ius_t) obj;
    }
    if (sourceType == IUS_2D_NON_PARAMETRIC_SOURCE)
    {
        iu2dnps_t obj = iusHL2DNonParametricSourceCreate(pSourceLabel,numLocations);
        source = (ius_t) obj;
    }
    if (sourceType == IUS_3D_NON_PARAMETRIC_SOURCE)
    {
        iu3dnps_t obj = iusHL3DNonParametricSourceCreate(pSourceLabel,numLocations);
        source = (ius_t) obj;
    }
    va_end(ap);
    return source;
}


int iusHLSourceDelete
(
    ius_t iusSource
)
{
    int status = IUS_ERR_VALUE;
    if(iusSource != NULL)
    {
        free(iusSource);
        iusSource = NULL;
        status = IUS_E_OK;
    }
    return status;
}


IUS_BOOL iusHLBaseSourceCompare
(
  ius_t reference,
  ius_t actual
)
{
    if ( reference == actual ) return IUS_TRUE;
    if ( reference == NULL || actual == NULL ) return IUS_FALSE;
    if ( reference->type != actual->type ) return IUS_FALSE;
    if ( reference->locationCount != actual->locationCount ) return IUS_FALSE;
    if ( strcmp(reference->label,actual->label) != 0 ) return IUS_FALSE;
    return IUS_TRUE;
}

IUS_BOOL iusHLSourceCompare
(
  ius_t reference,
  ius_t actual
)
{
  if( reference == actual )
    return IUS_TRUE;
  if( reference == NULL || actual == NULL )
    return IUS_FALSE;
  if( reference->type == IUS_3D_PARAMETRIC_SOURCE )
    return iusHL3DParametricSourceCompare((iu3dps_t) reference, (iu3dps_t)actual);
  return IUS_FALSE;
}


// Getters
IusSourceType iusHLSourceGetType
(
    ius_t iusSource
)
{
    if( iusSource == IUS_INVALID )
    {
        return IUS_INVALID_SOURCE_TYPE;
    }
    return iusSource->type;
}

char * iusHLSourceGetLabel
(
    ius_t iusSource
)
{
    if( iusSource == IUS_INVALID )
    {
        return NULL;
    }
    return iusSource->label;
}

