
//
// Created by nlv09165 on 24/05/2018.
//
#include <stdlib.h>
#define IUSLIBRARY_IMPLEMENTATION

#include <ius.h>
#include <iusError.h>
#include <iusTypes.h>
#include <iusUtil.h>
#include <math.h>

#include "include/iusHL3DSize.h"

struct Ius3DSize
{
  float sx;
  float sy;
  float sz;
} ;


// ADT
iu3ds_t iusHL3DSizeCreate
(
    float sx,
    float sy,
    float sz
)
{
    if( sx == NAN || sy == NAN || sz == NAN  ) return IU3DS_INVALID;
    iu3ds_t created = calloc(1,sizeof(Ius3DSize));
    created->sx = sx;
    created->sy = sy;
    created->sz = sz;
    return created;
}

int iusHL3DSizeDelete
(
    iu3ds_t ius3DSize
)
{
    int status = IUS_ERR_VALUE;
    if(ius3DSize != NULL)
    {
        free(ius3DSize);
        ius3DSize = NULL;
        status = IUS_E_OK;
    }
    return status;
}


// operations
int iusHL3DSizeCompare
(
    iu3ds_t reference,
    iu3ds_t actual
)
{
    if( reference == actual ) return IUS_TRUE;
    if( reference == NULL || actual == NULL ) return IUS_FALSE;
    if( IUS_EQUAL_FLOAT(reference->sx, actual->sx ) == IUS_FALSE ) return IUS_FALSE;
    if( IUS_EQUAL_FLOAT(reference->sy, actual->sy ) == IUS_FALSE ) return IUS_FALSE;
    if( IUS_EQUAL_FLOAT(reference->sz, actual->sz ) == IUS_FALSE ) return IUS_FALSE;
    return IUS_TRUE;
}

