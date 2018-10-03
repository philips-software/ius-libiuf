
//
// Created by nlv09165 on 23/05/2018.
//
#include <stdlib.h>

#include <ius.h>
#include <iusError.h>
#include <iusTypes.h>
#include <iusUtil.h>
#include <iusInputFileStructure.h>
#include <iusPosition.h>
#include <iusTransducerElement.h>
#include <ius3DTransducerElementImp.h>
#include <iusTransducerElementImp.h>
#include <include/ius3DSizeImp.h>
#include <include/iusHDF5.h>
#include <include/iusPositionImp.h>
#include <include/ius3DAngleImp.h>

struct Ius3DTransducerElement
{
  iu3dp_t   position; /**< 3D Location of the element */
  iu3da_t      angle;    /**< orientation of the elements */
  iu3ds_t       size;     /**< size of the element */
} ;

// ADT
iu3dte_t ius3DTransducerElementCreate
(
    iu3dp_t pos,
    iu3da_t ang,
    iu3ds_t siz
)
{
    if( pos == NULL || ang == NULL || siz == NULL ) return IU3DTE_INVALID;
    iu3dte_t created = calloc(1,sizeof(Ius3DTransducerElement));
    created->position = pos;
    created->angle = ang;
    created->size = siz;
    return created;
}

int ius3DTransducerElementDelete
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
int ius3DTransducerElementCompare
(
    iu3dte_t reference,
    iu3dte_t actual
)
{
    if( reference == actual ) return IUS_TRUE;
    if( reference == NULL || actual == NULL ) return IUS_FALSE;
    if( ius3DPositionCompare(reference->position, actual->position) != IUS_TRUE)
      return IUS_FALSE;
    if(ius3DSizeCompare(reference->size, actual->size) != IUS_TRUE)
      return IUS_FALSE;
    if(ius3DAngleCompare(reference->angle, actual->angle) != IUS_TRUE)
      return IUS_FALSE;
    return IUS_TRUE;
}

int ius3DTransducerElementSave
(
    iu3dte_t element,
    hid_t handle
)
{
    if( element == IU3DTE_INVALID ) 
		return IUS_ERR_VALUE;

    int status = ius3DSizeSave(element->size, handle);
    status |= ius3DPositionSave(element->position, handle);
    status |= ius3DAngleSave(element->angle, handle);

    return status;
}



iu3dte_t ius3DTransducerElementLoad
(
    hid_t handle
)
{
    iu3dte_t element = IU3DTE_INVALID;
    iu3dp_t elemPos = ius3DPositionLoad(handle);
    if (elemPos == IU3DP_INVALID) 
		return element;

    iu3ds_t elemSize = ius3DSizeLoad(handle);
    if (elemSize == IU3DS_INVALID) 
		return element;

    iu3da_t elemAngle = ius3DAngleLoad(handle);
    if (elemAngle == IU3DA_INVALID) 
		return element;

    element = ius3DTransducerElementCreate(elemPos, elemAngle, elemSize);
    return element;
}




// Getters
iu3dp_t ius3DTransducerElementGetPosition
(
	iu3dte_t ius3DTransducerElement
)
{
	return ius3DTransducerElement->position;
}

iu3da_t ius3DTransducerElementGetAngle
(
	iu3dte_t ius3DTransducerElement
)
{
	return ius3DTransducerElement->angle;
}

iu3ds_t ius3DTransducerElementGetSize
(
	iu3dte_t ius3DTransducerElement
)
{
	return ius3DTransducerElement->size;
}
