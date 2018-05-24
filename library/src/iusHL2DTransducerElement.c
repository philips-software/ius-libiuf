
//
// Created by nlv09165 on 23/05/2018.
//
#include <stdlib.h>
#define IUSLIBRARY_IMPLEMENTATION

#include <ius.h>
#include <iusError.h>
#include <iusTypes.h>
#include <iusUtil.h>
#include <math.h>
#include <include/iusHL3DTransducerElement.h>
#include <include/iusHL2DSize.h>

#include "include/iusHLTransducerElement.h"
#include "include/iusHL2DTransducerElement.h"


struct Ius2DTransducerElement
{
  iute_t    base;
  iu2dp_t   position;
  float     theta;
  iu2ds_t   size;
} ;

// ADT

iu2dte_t iusHL2DTransducerElementCreate
(
    iu2dp_t pos,
    float theta,
    iu2ds_t siz
)
{
    if( pos == NULL || siz == NULL ) return IU2DTE_INVALID;
    if( theta == NAN ) return IU2DTE_INVALID;
    iu2dte_t created = (Ius2DTransducerElement *) calloc(1,sizeof(struct Ius2DTransducerElement));
    created->base = iusHLTransducerElementCreate(IUS_2D_SHAPE);
    created->position = pos;
    created->theta = theta;
    created->size = siz;
    return created;
}

int iusHL2DTransducerElementDelete
(
    iu2dte_t ius2DTransducerElement
)
{
    int status = IUS_ERR_VALUE;
    if(ius2DTransducerElement != NULL)
    {
        free(ius2DTransducerElement);
        ius2DTransducerElement = NULL;
        status = IUS_E_OK;
    }
    return status;
}


// operations
int iusHL2DTransducerElementCompare
(
    iu2dte_t reference,
    iu2dte_t actual
)
{
    if( reference == actual ) return IUS_TRUE;
    if( reference == NULL || actual == NULL ) return IUS_FALSE;
    if( iusHL2DPositionCompare(reference->position, actual->position) == IUS_FALSE ) return IUS_FALSE;
    if( iusHL2DSizeCompare(reference->size, actual->size) == IUS_FALSE ) return IUS_FALSE;
    if( IUS_EQUAL_FLOAT(reference->theta, actual->theta ) == IUS_FALSE ) return IUS_FALSE;
    return IUS_TRUE;
}


// Getters
