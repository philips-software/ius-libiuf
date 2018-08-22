
//
// Created by nlv09165 on 23/05/2018.
//
#include <math.h>
#include <stdlib.h>

#include <ius.h>
#include <iusError.h>
#include <iusTypes.h>
#include <iusUtil.h>
#include <iusHL2DSizeImp.h>
#include <iusHLTransducerElement.h>
#include <iusHLTransducerElementImp.h>
#include <iusHL2DTransducerElementImp.h>
#include <iusHL3DTransducerElementImp.h>
#include <include/iusHLPositionImp.h>

struct Ius2DTransducerElement
{
  iu2dp_t   position;
  float     theta;
  iu2ds_t   size;
} ;

// ADT

iu2dte_t iusHL2DTransducerElementCreate
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

int iusHL2DTransducerElementDelete
(
    iu2dte_t ius2DTransducerElement
)
{
    int status = IUS_ERR_VALUE;
    if(ius2DTransducerElement != NULL)
    {
        free(ius2DTransducerElement);
        ius2DTransducerElement = NULL;
        status = IUS_E_OK;
    }
    return status;
}


// operations
int iusHL2DTransducerElementCompare
(
    iu2dte_t reference,
    iu2dte_t actual
)
{
    if( reference == actual ) return IUS_TRUE;
    if( reference == NULL || actual == NULL ) return IUS_FALSE;
    if( iusHL2DPositionCompare(reference->position, actual->position) == IUS_FALSE ) return IUS_FALSE;
    if( iusHL2DSizeCompare(reference->size, actual->size) == IUS_FALSE ) return IUS_FALSE;
    if( IUS_EQUAL_FLOAT(reference->theta, actual->theta ) == IUS_FALSE ) return IUS_FALSE;
    return IUS_TRUE;
}


#define ELEMENTSIZEPATH "%s/Size"
#define ELEMENTPOSITIONPATH  "%s/Position"
#define ELEMENTANGLEPATH  "%s/Angle"

int iusHL2DTransducerElementSave
(
    iu2dte_t element,
    const char *parentPath,
    hid_t handle
)
{
    char path[IUS_MAX_HDF5_PATH];
    if( element == IU2DTE_INVALID ) return IUS_ERR_VALUE;

    hid_t group_id = H5Gcreate(handle, parentPath, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

    sprintf(path, ELEMENTSIZEPATH, parentPath);
    int status = iusHL2DSizeSave(element->size,path,group_id);

    sprintf(path, ELEMENTPOSITIONPATH, parentPath);
    status |= iusHL2DPositionSave(element->position,path,group_id);

    sprintf(path, ELEMENTANGLEPATH, parentPath);
    status |= iusHdf5WriteFloat(group_id,path,&(element->theta),1,1);
    status |= H5Gclose(group_id );

    return status;
}



iu2dte_t iusHL2DTransducerElementLoad
(
    hid_t handle,
    const char *parentPath
)
{
    char path[IUS_MAX_HDF5_PATH];
    float theta;
    int status;

    sprintf(path, ELEMENTPOSITIONPATH, parentPath);
    iu2dp_t elemPos = iusHL2DPositionLoad(handle,path);
    if (elemPos == IU2DP_INVALID) return IU2DTE_INVALID;

    sprintf(path, ELEMENTSIZEPATH, parentPath);
    iu2ds_t elemSize = iusHL2DSizeLoad(handle,path);
    if (elemSize == IU2DS_INVALID) return IU2DTE_INVALID;

    sprintf(path, ELEMENTANGLEPATH, parentPath);
    status = iusHdf5ReadFloat(handle,path,&theta);
    if (status < 0) return IU2DTE_INVALID;

    return iusHL2DTransducerElementCreate(elemPos, theta, elemSize);
}



// Getters
iu2dp_t iusHL2DTransducerElementGetPosition
(
	iu2dte_t ius2DTransducerElement
)
{
	return ius2DTransducerElement->position;
}

float iusHL2DTransducerElementGetAngle
(
	iu2dte_t ius2DTransducerElement
)
{
	return ius2DTransducerElement->theta;
}

iu2ds_t iusHL2DTransducerElementGetSize
(
	iu2dte_t ius2DTransducerElement
)
{
	return ius2DTransducerElement->size;
}
