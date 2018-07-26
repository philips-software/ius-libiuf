
//
// Created by nlv09165 on 24/07/2018.
//
#include <stdlib.h>
#define IUSLIBRARY_IMPLEMENTATION

#include <ius.h>
#include <iusError.h>
#include <iusTypes.h>
#include <iusUtil.h>
#include <string.h>

#include "include/iusHLSourceImp.h"
#include "include/iusHL3DParametricSource.h"


// ADT

ius_t iusHLSourceCreate
(
    IusSourceType sourceType,
    char *pSourceLabel
)
{
    if( sourceType == IUS_INVALID_SOURCE_TYPE ) return IUS_INVALID;
    if( sourceType != IUS_2D_NON_PARAMETRIC_SOURCE &&
        sourceType != IUS_2D_PARAMETRIC_SOURCE &&
        sourceType != IUS_3D_NON_PARAMETRIC_SOURCE &&
        sourceType != IUS_3D_PARAMETRIC_SOURCE) return IUS_INVALID;
    if( pSourceLabel == NULL ) return IUS_INVALID;
    if( strcmp(pSourceLabel, "") == 0 ) return IUS_INVALID;
    ius_t created = calloc(1,sizeof(struct IusSource));
    created->type = sourceType;
    created->label = strdup(pSourceLabel);
    return created;
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
  if( reference == actual )
    return IUS_TRUE;
  if( reference == NULL || actual == NULL )
    return IUS_FALSE;
  if( reference->type != actual->type )
    return IUS_FALSE;
  if( strcmp(reference->label,actual->label) != 0 )
    return IUS_FALSE;
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

