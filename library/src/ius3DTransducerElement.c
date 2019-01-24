
//
// Created by nlv09165 on 23/05/2018.
//
#include <stdlib.h>

#include <ius.h>
#include <ius3DSizePrivate.h>
#include <iusPositionPrivate.h>
#include <ius3DAnglePrivate.h>
#include <iusTransducerElementPrivate.h>
#include <ius3DTransducerElementPrivate.h>

struct Ius3DTransducerElement
{
    iu3dp_t   position; /**< 3D Location of the element */
    iu3da_t      angle;    /**< orientation of the elements */
    iu3ds_t       size;     /**< size of the element */
    IUS_BOOL deepDelete;
} ;

// ADT
iu3dte_t ius3DTransducerElementCreate
(
    iu3dp_t pos,
    iu3da_t ang,
    iu3ds_t siz
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(pos, IU3DTE_INVALID);
    IUS_ERR_CHECK_NULL_N_RETURN(ang, IU3DTE_INVALID);
    IUS_ERR_CHECK_NULL_N_RETURN(siz, IU3DTE_INVALID);
    iu3dte_t created = calloc(1,sizeof(Ius3DTransducerElement));
    IUS_ERR_ALLOC_NULL_N_RETURN(created, Ius3DTransducerElement, IU3DTE_INVALID);
    created->position = pos;
    created->angle = ang;
    created->size = siz;
    created->deepDelete = IUS_FALSE;
    return created;
}


int ius3DTransducerElementDeepDelete
(
    iu3dte_t element
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(element, IUS_ERR_VALUE);
    element->deepDelete = IUS_TRUE;
    return ius3DTransducerElementDelete(element);
}

int ius3DTransducerElementDelete
(
    iu3dte_t element
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(element, IUS_ERR_VALUE);
    if(element->deepDelete == IUS_TRUE)
    {
        ius3DPositionDelete(ius3DTransducerElementGetPosition(element));
        ius3DSizeDelete(ius3DTransducerElementGetSize(element));
        ius3DAngleDelete(ius3DTransducerElementGetAngle(element));
    }
    free(element);
    return IUS_E_OK;
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
    IUS_ERR_CHECK_NULL_N_RETURN(element, IUS_ERR_VALUE);
    IUS_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUS_ERR_VALUE);

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
    IUS_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IU3DTE_INVALID);
    iu3dp_t elemPos = ius3DPositionLoad(handle);
    if (elemPos == IU3DP_INVALID)
        return IU3DTE_INVALID;

    iu3ds_t elemSize = ius3DSizeLoad(handle);
    if (elemSize == IU3DS_INVALID)
        return IU3DTE_INVALID;

    iu3da_t elemAngle = ius3DAngleLoad(handle);
    if (elemAngle == IU3DA_INVALID)
        return IU3DTE_INVALID;

    iu3dte_t element = ius3DTransducerElementCreate(elemPos, elemAngle, elemSize);
    element->deepDelete = IUS_TRUE;
    return element;
}



// Getters
iu3dp_t ius3DTransducerElementGetPosition
(
    iu3dte_t element
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(element, IU3DP_INVALID);
    return element->position;
}

iu3da_t ius3DTransducerElementGetAngle
(
    iu3dte_t element
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(element, IU3DA_INVALID);
    return element->angle;
}

iu3ds_t ius3DTransducerElementGetSize
(
    iu3dte_t element
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(element, IU3DS_INVALID);
    return element->size;
}
