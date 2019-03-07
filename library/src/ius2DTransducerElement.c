
//
// Created by nlv09165 on 23/05/2018.
//
#include <math.h>
#include <stdlib.h>

#include <ius.h>
#include <ius2DSizePrivate.h>
#include <ius2DTransducerElementPrivate.h>
#include <ius3DTransducerElementPrivate.h>
#include <iusPositionPrivate.h>
#include <iusTransducerElementPrivate.h>


// ADT
iu2dte_t ius2DTransducerElementCreate
(
    iu2dp_t pos,
    float theta,
    iu2ds_t size
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(pos, IU2DTE_INVALID);
    IUS_ERR_CHECK_NULL_N_RETURN(size, IU2DTE_INVALID);
    // According to the IEEE standard, NaN values have the odd property that comparisons involving them are always false.
    // That is, for a float theta, theta != theta will be true only if f is NaN.
    IUS_ERR_EVAL_N_RETURN(theta != theta, IU2DTE_INVALID);
    iu2dte_t created = (Ius2DTransducerElement *) calloc(1,sizeof(struct Ius2DTransducerElement));
    IUS_ERR_ALLOC_NULL_N_RETURN(created, Ius2DTransducerElement, IU2DTE_INVALID);
    created->position = pos;
    created->theta = theta;
    created->size = size;
    created->deepDelete = IUS_FALSE;
    return created;
}

int ius2DTransducerElementDeepDelete
(
    iu2dte_t element
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(element, IUS_ERR_VALUE);
    element->deepDelete=IUS_TRUE;
    return ius2DTransducerElementDelete(element);
}

int ius2DTransducerElementDelete
(
    iu2dte_t element
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(element, IUS_ERR_VALUE);
    if (element->deepDelete == IUS_TRUE)
    {
        ius2DPositionDelete(element->position);
        ius2DSizeDelete(element->size);
    }
    free(element);
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

    IUS_ERR_CHECK_NULL_N_RETURN(element, IUS_ERR_VALUE);
    IUS_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUS_ERR_VALUE);

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
    IUS_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IU2DTE_INVALID);

    iu2dp_t elemPos = ius2DPositionLoad(handle);
    if (elemPos == IU2DP_INVALID) return IU2DTE_INVALID;

    iu2ds_t elemSize = ius2DSizeLoad(handle);
    if (elemSize == IU2DS_INVALID) return IU2DTE_INVALID;

    status = iusHdf5ReadFloat(handle, IUS_INPUTFILE_PATH_ANGLE_THETA, &theta);
    if (status < 0) return IU2DTE_INVALID;

    iu2dte_t element = ius2DTransducerElementCreate(elemPos, theta, elemSize);
    element->deepDelete = IUS_TRUE;
    return element;
}

// Getters
iu2dp_t ius2DTransducerElementGetPosition
(
	iu2dte_t element
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(element, IU2DP_INVALID);
	return element->position;
}

float ius2DTransducerElementGetAngle
(
	iu2dte_t element
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(element, NAN);
    return element->theta;
}

iu2ds_t ius2DTransducerElementGetSize
(
	iu2dte_t element
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(element, IU2DS_INVALID);
	return element->size;
}
