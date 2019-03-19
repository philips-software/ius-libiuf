
//
// Created by nlv09165 on 23/05/2018.
//
#include <stdlib.h>

#include <iuf.h>
#include <iuf3DSizePrivate.h>
#include <iufPositionPrivate.h>
#include <iuf3DAnglePrivate.h>
#include <iufTransducerElementPrivate.h>
#include <iuf3DTransducerElementPrivate.h>

struct Iuf3DTransducerElement
{
    iu3dp_t   position; /**< 3D Location of the element */
    iu3da_t      angle;    /**< orientation of the elements */
    iu3ds_t       size;     /**< size of the element */
    IUF_BOOL deepDelete;
} ;

// ADT
iu3dte_t iuf3DTransducerElementCreate
(
    iu3dp_t pos,
    iu3da_t ang,
    iu3ds_t siz
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(pos, IU3DTE_INVALID);
    IUF_ERR_CHECK_NULL_N_RETURN(ang, IU3DTE_INVALID);
    IUF_ERR_CHECK_NULL_N_RETURN(siz, IU3DTE_INVALID);
    iu3dte_t created = calloc(1,sizeof(Iuf3DTransducerElement));
    IUF_ERR_ALLOC_NULL_N_RETURN(created, Iuf3DTransducerElement, IU3DTE_INVALID);
    created->position = pos;
    created->angle = ang;
    created->size = siz;
    created->deepDelete = IUF_FALSE;
    return created;
}


int iuf3DTransducerElementDeepDelete
(
    iu3dte_t element
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(element, IUF_ERR_VALUE);
    element->deepDelete = IUF_TRUE;
    return iuf3DTransducerElementDelete(element);
}

int iuf3DTransducerElementDelete
(
    iu3dte_t element
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(element, IUF_ERR_VALUE);
    if(element->deepDelete == IUF_TRUE)
    {
        iuf3DPositionDelete(iuf3DTransducerElementGetPosition(element));
        iuf3DSizeDelete(iuf3DTransducerElementGetSize(element));
        iuf3DAngleDelete(iuf3DTransducerElementGetAngle(element));
    }
    free(element);
    return IUF_E_OK;
}


// operations
int iuf3DTransducerElementCompare
(
    iu3dte_t reference,
    iu3dte_t actual
)
{
    if( reference == actual ) return IUF_TRUE;
    if( reference == NULL || actual == NULL ) return IUF_FALSE;
    if( iuf3DPositionCompare(reference->position, actual->position) != IUF_TRUE)
        return IUF_FALSE;
    if(iuf3DSizeCompare(reference->size, actual->size) != IUF_TRUE)
        return IUF_FALSE;
    if(iuf3DAngleCompare(reference->angle, actual->angle) != IUF_TRUE)
        return IUF_FALSE;
    return IUF_TRUE;
}

int iuf3DTransducerElementSave
(
    iu3dte_t element,
    hid_t handle
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(element, IUF_ERR_VALUE);
    IUF_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUF_ERR_VALUE);

    int status = iuf3DSizeSave(element->size, handle);
    status |= iuf3DPositionSave(element->position, handle);
    status |= iuf3DAngleSave(element->angle, handle);

    return status;
}



iu3dte_t iuf3DTransducerElementLoad
(
    hid_t handle
)
{
    IUF_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IU3DTE_INVALID);
    iu3dp_t elemPos = iuf3DPositionLoad(handle);
    if (elemPos == IU3DP_INVALID)
        return IU3DTE_INVALID;

    iu3ds_t elemSize = iuf3DSizeLoad(handle);
    if (elemSize == IU3DS_INVALID)
        return IU3DTE_INVALID;

    iu3da_t elemAngle = iuf3DAngleLoad(handle);
    if (elemAngle == IU3DA_INVALID)
        return IU3DTE_INVALID;

    iu3dte_t element = iuf3DTransducerElementCreate(elemPos, elemAngle, elemSize);
    element->deepDelete = IUF_TRUE;
    return element;
}



// Getters
iu3dp_t iuf3DTransducerElementGetPosition
(
    iu3dte_t element
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(element, IU3DP_INVALID);
    return element->position;
}

iu3da_t iuf3DTransducerElementGetAngle
(
    iu3dte_t element
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(element, IU3DA_INVALID);
    return element->angle;
}

iu3ds_t iuf3DTransducerElementGetSize
(
    iu3dte_t element
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(element, IU3DS_INVALID);
    return element->size;
}
