
#include <math.h>
#include <stdlib.h>

#include <iuf.h>
#include <iuf2DSizePrivate.h>
#include <iuf2DTransducerElementPrivate.h>
#include <iuf3DTransducerElementPrivate.h>
#include <iufPositionPrivate.h>
#include <iufTransducerElementPrivate.h>


// ADT
iu2dte_t iuf2DTransducerElementCreate
(
    iu2dp_t pos,
    float theta,
    iu2ds_t size
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(pos, IU2DTE_INVALID);
    IUF_ERR_CHECK_NULL_N_RETURN(size, IU2DTE_INVALID);
    // According to the IEEE standard, NaN values have the odd property that comparisons involving them are always false.
    // That is, for a float theta, theta != theta will be true only if f is NaN.
    IUF_ERR_EVAL_N_RETURN(theta != theta, IU2DTE_INVALID);
    iu2dte_t created = (Iuf2DTransducerElement *) calloc(1,sizeof(struct Iuf2DTransducerElement));
    IUF_ERR_ALLOC_NULL_N_RETURN(created, Iuf2DTransducerElement, IU2DTE_INVALID);
    created->position = pos;
    created->theta = theta;
    created->size = size;
    created->deepDelete = IUF_FALSE;
    return created;
}

int iuf2DTransducerElementDeepDelete
(
    iu2dte_t element
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(element, IUF_ERR_VALUE);
    element->deepDelete=IUF_TRUE;
    return iuf2DTransducerElementDelete(element);
}

int iuf2DTransducerElementDelete
(
    iu2dte_t element
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(element, IUF_ERR_VALUE);
    if (element->deepDelete == IUF_TRUE)
    {
        iuf2DPositionDelete(element->position);
        iuf2DSizeDelete(element->size);
    }
    free(element);
    return IUF_E_OK;
}


// operations
int iuf2DTransducerElementCompare
(
    iu2dte_t reference,
    iu2dte_t actual
)
{
    if( reference == actual ) return IUF_TRUE;
    if( reference == NULL || actual == NULL ) return IUF_FALSE;
    if( iuf2DPositionCompare(reference->position, actual->position) == IUF_FALSE ) return IUF_FALSE;
    if( iuf2DSizeCompare(reference->size, actual->size) == IUF_FALSE ) return IUF_FALSE;
    if( IUF_EQUAL_FLOAT(reference->theta, actual->theta ) == IUF_FALSE ) return IUF_FALSE;
    return IUF_TRUE;
}


int iuf2DTransducerElementSave
(
    iu2dte_t element,
    hid_t handle
)
{

    IUF_ERR_CHECK_NULL_N_RETURN(element, IUF_ERR_VALUE);
    IUF_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUF_ERR_VALUE);

    int status = iuf2DSizeSave(element->size, handle);
    status |= iuf2DPositionSave(element->position, handle);
    status |= iufHdf5WriteFloat(handle, IUF_INPUTFILE_PATH_ANGLE_THETA, &(element->theta),1);

    return status;
}


iu2dte_t iuf2DTransducerElementLoad
(
    hid_t handle
)
{
    float theta;
    int status;
    IUF_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IU2DTE_INVALID);

    iu2dp_t elemPos = iuf2DPositionLoad(handle);
    if (elemPos == IU2DP_INVALID) return IU2DTE_INVALID;

    iu2ds_t elemSize = iuf2DSizeLoad(handle);
    if (elemSize == IU2DS_INVALID) return IU2DTE_INVALID;

    status = iufHdf5ReadFloat(handle, IUF_INPUTFILE_PATH_ANGLE_THETA, &theta);
    if (status < 0) return IU2DTE_INVALID;

    iu2dte_t element = iuf2DTransducerElementCreate(elemPos, theta, elemSize);
    element->deepDelete = IUF_TRUE;
    return element;
}

// Getters
iu2dp_t iuf2DTransducerElementGetPosition
(
	iu2dte_t element
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(element, IU2DP_INVALID);
	return element->position;
}

float iuf2DTransducerElementGetAngle
(
	iu2dte_t element
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(element, NAN);
    return element->theta;
}

iu2ds_t iuf2DTransducerElementGetSize
(
	iu2dte_t element
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(element, IU2DS_INVALID);
	return element->size;
}
