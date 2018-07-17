
//
// Created by nlv09165 on 24/05/2018.
//
#include <math.h>
#include <stdlib.h>

#include <ius.h>
#include <iusError.h>
#include <iusTypes.h>
#include <iusUtil.h>
#include <iusHL2DSize.h>

//struct Ius2DSize
//{
//  float sx;
//  float sz;
//} ;

// ADT

iu2ds_t iusHL2DSizeCreate
(
    float sx,
    float sz
)
{
    if( sx == NAN || sz == NAN ) return IU2DS_INVALID;
    iu2ds_t created = calloc(1,sizeof(Ius2DSize));
    created->sx = sx;
    created->sz = sz;
    return created;
}

int iusHL2DSizeDelete
(
    iu2ds_t ius2DSize
)
{
    int status = IUS_ERR_VALUE;
    if(ius2DSize != NULL)
    {
        free(ius2DSize);
        ius2DSize = NULL;
        status = IUS_E_OK;
    }
    return status;
}


// operations
int iusHL2DSizeCompare
(
    iu2ds_t reference,
    iu2ds_t actual
)
{
    if( reference == actual ) return IUS_TRUE;
    if( reference == NULL || actual == NULL ) return IUS_FALSE;
    if( IUS_EQUAL_FLOAT(reference->sx, actual->sx ) == IUS_FALSE ) return IUS_FALSE;
    if( IUS_EQUAL_FLOAT(reference->sz, actual->sz ) == IUS_FALSE ) return IUS_FALSE;
    return IUS_TRUE;
}
