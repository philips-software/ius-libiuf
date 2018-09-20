
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
#include <iusHL3DTransducerElementImp.h>
#include <iusHLTransducerElementImp.h>
#include <include/iusHL3DSizeImp.h>
#include <include/iusHDF5.h>
#include <include/iusHLPositionImp.h>
#include <include/iusHL3DAngleImp.h>

struct Ius3DTransducerElement
{
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


#define ELEMENTSIZEPATH "%s/Size"
#define ELEMENTPOSITIONPATH  "%s/Position"
#define ELEMENTANGLEPATH  "%s/Angle"

int iusHL3DTransducerElementSave
(
    iu3dte_t element,
    hid_t handle
)
{
    //char path[IUS_MAX_HDF5_PATH];
    if( element == IU3DTE_INVALID ) return IUS_ERR_VALUE;

    //hid_t group_id = H5Gcreate(handle, parentPath, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

    //sprintf(path, ELEMENTSIZEPATH, parentPath);
    int status = iusHL3DSizeSave(element->size, handle); //todo cenralize this string

    //sprintf(path, ELEMENTPOSITIONPATH, parentPath);
    status |= iusHL3DPositionSave(element->position, handle);//todo cenralize this string

    //sprintf(path, ELEMENTANGLEPATH, parentPath);
    status |= iusHL3DAngleSave(element->angle, handle); //todo cenralize this string
    //status |= H5Gclose(group_id );

    return status;
}



iu3dte_t iusHL3DTransducerElementLoad
(
    hid_t handle
)
{
    //char path[IUS_MAX_HDF5_PATH];
    iu3dte_t element = IU3DTE_INVALID;
    //sprintf(path, ELEMENTPOSITIONPATH, parentPath);
    iu3dp_t elemPos = iusHL3DPositionLoad(handle); //todo centralize these
    if (elemPos == IU3DP_INVALID) return element;

    //sprintf(path, ELEMENTSIZEPATH, parentPath);
    iu3ds_t elemSize = iusHL3DSizeLoad(handle);
    if (elemSize == IU3DS_INVALID) return element;

    //sprintf(path, ELEMENTANGLEPATH, parentPath);
    iu3da_t elemAngle = iusHL3DAngleLoad(handle);
    if (elemAngle == IU3DA_INVALID) return element;

    element = iusHL3DTransducerElementCreate(elemPos, elemAngle, elemSize);
    return element;
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
