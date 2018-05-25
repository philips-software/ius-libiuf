
//
// Created by nlv09165 on 24/05/2018.
//
#include <math.h>
#include <stdlib.h>

#include <ius.h>
#include <iusError.h>
#include <iusTypes.h>
#include <iusUtil.h>
#include <iusHL3DAngle.h>

struct Ius3DAngle
{
  float theta; /**< rotation over z axis */
  float phi;   /**< rotation over x axis */
} ;

// ADT
iu3da_t iusHL3DAngleCreate
(
    float theta,
    float phi
)
{
    if( theta == NAN || phi == NAN ) return IU3DA_INVALID;
    iu3da_t created = calloc(1,sizeof(Ius3DAngle));
    created->theta = theta;
    created->phi = phi;
    return created;
}

int iusHL3DAngleDelete
(
    iu3da_t ius3DAngle
)
{
    int status = IUS_ERR_VALUE;
    if(ius3DAngle != NULL)
    {
        free(ius3DAngle);
        ius3DAngle = NULL;
        status = IUS_E_OK;
    }
    return status;
}


// operations
int iusHL3DAngleCompare
(
    iu3da_t reference,
    iu3da_t actual
)
{
    if( reference == actual ) return IUS_TRUE;
    if( reference == NULL || actual == NULL ) return IUS_FALSE;
    if( IUS_EQUAL_FLOAT(reference->theta, actual->theta ) == IUS_FALSE ) return IUS_FALSE;
    if( IUS_EQUAL_FLOAT(reference->phi, actual->phi ) == IUS_FALSE ) return IUS_FALSE;
    return IUS_TRUE;
}
