
//
// Created by nlv09165 on 23/05/2018.
//
#include <stdlib.h>

#include <ius.h>
#include <iusError.h>
#include <iusTypes.h>
#include <iusUtil.h>
#include <iusHLPosition.h>
#include <iusHLTransducerElement.h>
#include <iusHL3DTransducerElement.h>
#include <iusHLTransducerElementImp.h>


struct Ius3DTransducerElement
{
  struct IusTransducerElement base;
  iu3dp_t   position; /**< 3D Location of the element */
  iu3da_t      angle;    /**< orientation of the elements */
  iu3ds_t       size;     /**< size of the element */
} ;

// ADT
iu3dte_t iusHL3DTransducerElementCreate
(
    iu3dp_t pos,
    iu3da_t ang,
    iu3ds_t siz
)
{
    if( pos == NULL || ang == NULL || siz == NULL ) return IU3DTE_INVALID;
    iu3dte_t created = calloc(1,sizeof(Ius3DTransducerElement));
    created->base.shape = IUS_3D_SHAPE;
    created->position = pos;
    created->angle = ang;
    created->size = siz;
    return created;
}

int iusHL3DTransducerElementDelete
(
    iu3dte_t ius3DTransducerElement
)
{
    int status = IUS_ERR_VALUE;
    if(ius3DTransducerElement != NULL)
    {
        free(ius3DTransducerElement);
        ius3DTransducerElement = NULL;
        status = IUS_E_OK;
    }
    return status;
}


// operations
int iusHL3DTransducerElementCompare
(
    iu3dte_t reference,
    iu3dte_t actual
)
{
    if( reference == actual ) return IUS_TRUE;
    if( reference == NULL || actual == NULL ) return IUS_FALSE;
    if( iusHL3DPositionCompare(reference->position, actual->position) != IUS_TRUE)
      return IUS_FALSE;
    if(iusHL3DSizeCompare(reference->size, actual->size) != IUS_TRUE)
      return IUS_FALSE;
    if(iusHL3DAngleCompare(reference->angle, actual->angle) != IUS_TRUE)
      return IUS_FALSE;
    return IUS_TRUE;
}


// Getters
iu3dp_t iusHL3DTransducerElementGetPosition
(
	iu3dte_t ius3DTransducerElement
)
{
	return ius3DTransducerElement->position;
}

iu3da_t iusHL3DTransducerElementGetAngle
(
	iu3dte_t ius3DTransducerElement
)
{
	return ius3DTransducerElement->angle;
}

iu3ds_t iusHL3DTransducerElementGetSize
(
	iu3dte_t ius3DTransducerElement
)
{
	return ius3DTransducerElement->size;
}
