
//
// Created by nlv09165 on 23/05/2018.
//
#include <stdlib.h>

#include <ius.h>
#include <ius2DSizePrivate.h>
#include <ius2DTransducerElementPrivate.h>
#include <ius3DTransducerElementPrivate.h>
#include <iusPositionPrivate.h>
#include <iusTransducerElementPrivate.h>

struct Ius2DTransducerElement
{
  iu2dp_t   position;
  float     theta;
  iu2ds_t   size;
} ;

// ADT
iu2dte_t ius2DTransducerElementCreate
(
    iu2dp_t pos,
    float theta,
    iu2ds_t siz
)
{
    if( pos == NULL || siz == NULL ) return IU2DTE_INVALID;
    // According to the IEEE standard, NaN values have the odd property that comparisons involving them are always false.
    // That is, for a float theta, theta != theta will be true only if f is NaN.
    if( theta != theta ) return IU2DTE_INVALID;
    iu2dte_t created = (Ius2DTransducerElement *) calloc(1,sizeof(struct Ius2DTransducerElement));
    created->position = pos;
    created->theta = theta;
    created->size = siz;
    return created;
}

int ius2DTransducerElementDeepDelete
(
    iu2dte_t ius2DTransducerElement
)
{
    if (ius2DTransducerElement == NULL) return IUS_ERR_VALUE;
    ius2DPositionDelete(ius2DTransducerElementGetPosition(ius2DTransducerElement));
    ius2DSizeDelete(ius2DTransducerElementGetSize(ius2DTransducerElement));
    return ius2DTransducerElementDelete(ius2DTransducerElement);
}

int ius2DTransducerElementDelete
(
    iu2dte_t ius2DTransducerElement
)
{
    if (ius2DTransducerElement == NULL) return IUS_ERR_VALUE;
    free(ius2DTransducerElement);
    return IUS_E_OK;
}


// operations
int ius2DTransducerElementCompare
(
    iu2dte_t reference,
    iu2dte_t actual
)
{
    if( reference == actual ) return IUS_TRUE;
    if( reference == NULL || actual == NULL ) return IUS_FALSE;
    if( ius2DPositionCompare(reference->position, actual->position) == IUS_FALSE ) return IUS_FALSE;
    if( ius2DSizeCompare(reference->size, actual->size) == IUS_FALSE ) return IUS_FALSE;
    if( IUS_EQUAL_FLOAT(reference->theta, actual->theta ) == IUS_FALSE ) return IUS_FALSE;
    return IUS_TRUE;
}


int ius2DTransducerElementSave
(
    iu2dte_t element,
    hid_t handle
)
{
    
    if( element == IU2DTE_INVALID ) return IUS_ERR_VALUE;

    int status = ius2DSizeSave(element->size, handle);
    status |= ius2DPositionSave(element->position, handle);
    status |= iusHdf5WriteFloat(handle, IUS_INPUTFILE_PATH_ANGLE_THETA, &(element->theta),1);

    return status;
}


iu2dte_t ius2DTransducerElementLoad
(
    hid_t handle
)
{
    float theta;
    int status;
	
    iu2dp_t elemPos = ius2DPositionLoad(handle);
    if (elemPos == IU2DP_INVALID) return IU2DTE_INVALID;

    iu2ds_t elemSize = ius2DSizeLoad(handle);
    if (elemSize == IU2DS_INVALID) return IU2DTE_INVALID;

    status = iusHdf5ReadFloat(handle, IUS_INPUTFILE_PATH_ANGLE_THETA, &theta);
    if (status < 0) return IU2DTE_INVALID;

    return ius2DTransducerElementCreate(elemPos, theta, elemSize);
}

// Getters
iu2dp_t ius2DTransducerElementGetPosition
(
	iu2dte_t ius2DTransducerElement
)
{
	return ius2DTransducerElement->position;
}

float ius2DTransducerElementGetAngle
(
	iu2dte_t ius2DTransducerElement
)
{
	return ius2DTransducerElement->theta;
}

iu2ds_t ius2DTransducerElementGetSize
(
	iu2dte_t ius2DTransducerElement
)
{
	return ius2DTransducerElement->size;
}
