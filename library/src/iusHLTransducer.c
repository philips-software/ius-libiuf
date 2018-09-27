//
// Created by nlv09165 on 24/05/2018.
//
#include <math.h>
#include <string.h>
#include <stdlib.h>

#include <hdf5.h>
#include <hdf5_hl.h>

#include <ius.h>
#include <iusHDF5.h>
#include <iusError.h>
#include <iusTypes.h>
#include <iusUtil.h>

#include <iusHLTransducerImp.h>
#include <iusHL2DTransducerImp.h>
#include <iusHL3DTransducerImp.h>
#include <iusHLTransducerElement.h>
#include <iusHL2DTransducerElementList.h>
#include <iusHL3DTransducerElementList.h>

// ADT
iut_t iusHLTransducerCreate
(
    const char *name,
    IusTransducerShape shape,
    float centerFrequency
)
{
    if( name == NULL ) return IUT_INVALID;
    if( shape == IUS_INVALID_SHAPE ) return IUT_INVALID;
    if( centerFrequency == NAN ) return IUT_INVALID;
    iut_t created = calloc(1,sizeof(IusTransducer));

    if(shape == IUS_LINE || shape == IUS_CIRCLE)
        created->type = IUS_2D_SHAPE;
    else
        created->type = IUS_3D_SHAPE;

    created->pTransducerName = strdup(name);
    created->shape = shape;
    created->centerFrequency = centerFrequency;
    return created;
}

int iusHLTransducerDelete
(
    iut_t iusTransducer
)
{
    int status = IUS_ERR_VALUE;
    if(iusTransducer != NULL)
    {
        free(iusTransducer->pTransducerName);
        free(iusTransducer);
        status = IUS_E_OK;
    }
    return status;
}


// operations
int iusHLBaseTransducerCompare
(
    iut_t reference,
    iut_t actual
)
{
    if( reference == actual )
        return IUS_TRUE;
    if( reference == NULL || actual == NULL )
        return IUS_FALSE;
    if( reference->shape != actual->shape )
        return IUS_FALSE;
    if( reference->type != actual->type )
        return IUS_FALSE;
    if( IUS_EQUAL_FLOAT(reference->centerFrequency, actual->centerFrequency ) == IUS_FALSE )
        return IUS_FALSE;
    if( strcmp( reference->pTransducerName, actual->pTransducerName) != 0 )
        return IUS_FALSE;
    return IUS_TRUE;
}

int iusHLTransducerCompare
(
    iut_t reference,
    iut_t actual
)
{
    if( reference == actual )
        return IUS_TRUE;
    if( reference == NULL || actual == NULL )
        return IUS_FALSE;

    if( reference->type == IUS_2D_SHAPE )
        return iusHL2DTransducerCompare((iu2dt_t) reference,(iu2dt_t) actual);

    if( reference->type == IUS_3D_SHAPE )
        return iusHL3DTransducerCompare((iu3dt_t) reference,(iu3dt_t) actual);
    return IUS_FALSE;
}

// Getters
float iusHLTransducerGetCenterFrequency
(
    iut_t transducer
)
{
      if( transducer == NULL ) return NAN;
      return transducer->centerFrequency;
};

int iusHLTransducerGetNumElements
(
    iut_t transducer
)
{
    if (transducer == NULL) return -1;
    if( transducer->type == IUS_2D_SHAPE )
        return iusHL2DTransducerGetNumElements((iu2dt_t)transducer);

    if( transducer->type == IUS_3D_SHAPE )
        return iusHL3DTransducerGetNumElements((iu3dt_t)transducer);
    return -1;
}

char *iusHLTransducerGetName
(
    iut_t transducer
)
{
    if (transducer == NULL) return NULL;
    return transducer->pTransducerName;
}

IusTransducerShape iusHLTransducerGetShape
(
    iut_t transducer
)
{
    if (transducer == NULL) return IUS_INVALID_TRANSDUCER_SHAPE;
    return transducer->shape;
}

iute_t iusHLTransducerGetElement
(
    iut_t transducer,
    int elementIndex
)
{
    if( transducer->type == IUS_3D_SHAPE)
        return (iute_t) iusHL3DTransducerGetElement( (iu3dt_t) transducer, elementIndex);

    if( transducer->type == IUS_2D_SHAPE )
        return (iute_t) iusHL2DTransducerGetElement((iu2dt_t) transducer, elementIndex);

    return IUTE_INVALID;
}

// Setters
int iusHLTransducerSetElement
(
      iut_t transducer,
      int elementIndex,
      iute_t element
)
{
    int status = IUS_ERR_VALUE;

    if( transducer->type == IUS_3D_SHAPE && iusHLTransducerElementGetShape(element) == IUS_3D_SHAPE )
        status = iusHL3DTransducerSetElement( (iu3dt_t) transducer, elementIndex, (iu3dte_t)element );

    if( transducer->type == IUS_2D_SHAPE && iusHLTransducerElementGetShape(element) == IUS_2D_SHAPE )
        status = iusHL2DTransducerSetElement((iu2dt_t) transducer, elementIndex, (iu2dte_t)element );

    return status;
}

/*
hid_t iusWriteTransducerType
(
	hid_t handle,
	IusShape transducerType
)
{
	// TODO: implement this write function
	return handle;
} */

#define TRANSDUCER_ELEMENTS_FMT "Elements"

static herr_t iusHLBaseTransducerSaveShape(hid_t group_id,
                                           const char *pVariableString,
                                           IusTransducerShape shape)
{
	herr_t status = 0;
	hsize_t dims[1] = { 1 };
	/* Based on a native signed short */
	hid_t hdf_shapeType = H5Tcreate(H5T_ENUM, sizeof(short));
	short enumValue;
	status |= H5Tenum_insert(hdf_shapeType, TRANSDUCER_SHAPE_LINE, (enumValue = IUS_LINE, &enumValue));
	status |= H5Tenum_insert(hdf_shapeType, TRANSDUCER_SHAPE_CIRCLE, (enumValue = IUS_CIRCLE, &enumValue));
	status |= H5Tenum_insert(hdf_shapeType, TRANSDUCER_SHAPE_PLANE, (enumValue = IUS_PLANE, &enumValue));
	status |= H5Tenum_insert(hdf_shapeType, TRANSDUCER_SHAPE_CYLINDER, (enumValue = IUS_CYLINDER, &enumValue));
	status |= H5Tenum_insert(hdf_shapeType, TRANSDUCER_SHAPE_SPHERE, (enumValue = IUS_SPHERE, &enumValue));
	enumValue = shape;
	status |= H5LTmake_dataset(group_id, pVariableString, 1, dims, hdf_shapeType, &enumValue);
	return status;
}

#define SHAPEFMT "shape"
#define NAMEFMT "transducerName"
#define CENTERFREQUENCYFMT "centerFrequency"

#define IUS_INPUT_STRUCTURE_TRANSDUCER "Transducer"

herr_t iusHLBaseTransducerSave
(
    iut_t transducer,
    hid_t handle
)
{
	herr_t status = IUS_E_OK;
    //char path[IUS_MAX_HDF5_PATH];
 
	//TODO create a Transducer group if it doesn't exist yet
	//status = H5Gget_objinfo(handle, IUS_INPUT_STRUCTURE_TRANSDUCER, 0, NULL);

    // sprintf(path, SHAPEFMT, parentPath);
    status |= iusHLBaseTransducerSaveShape(handle, SHAPEFMT, transducer->shape);
    // sprintf(path, NAMEFMT, parentPath);
	status |= iusHdf5WriteString(handle, NAMEFMT, transducer->pTransducerName, 1);
    // sprintf(path, CENTERFREQUENCYFMT, parentPath);
	status |= iusHdf5WriteFloat(handle, CENTERFREQUENCYFMT, &(transducer->centerFrequency), 1, 1);
    //status |= H5Gclose(group_id );
    
	return status;
}


herr_t iusHLTransducerSave
(
    iut_t transducer,
    hid_t handle
)
{
	/* write the /Transducer data in "Transducer" */
	herr_t  status=0;
	hid_t transducer_id;

	status = H5Gget_objinfo(handle, "Transducer", 0, NULL); // todo centralize the path "Transducer"
	if (status != 0) // the group does not exist yet
	{
		transducer_id = H5Gcreate(handle, "Transducer", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
	}
	else
	{
		transducer_id = H5Gopen(handle, "Transducer", H5P_DEFAULT);
	}
	// todo check if transducer_id is valid
	status = 0;
    if (transducer->type == IUS_3D_SHAPE)
		status |= iusHL3DTransducerSave((Ius3DTransducer *) transducer, transducer_id);

	if (transducer->type == IUS_2D_SHAPE)
		status |= iusHL2DTransducerSave((Ius2DTransducer *) transducer, transducer_id);
	return status;
}

static int iusHLTransducerLoadShape
(
    hid_t handle,
    char *pVariableString,
    IusTransducerShape *pShape
)
{
    herr_t status = 0;
    /* Based on a native signed short */
    hid_t hdf_shapeType = H5Tcreate( H5T_ENUM, sizeof(short) );
    short enumValue;
    status |= H5Tenum_insert(hdf_shapeType, TRANSDUCER_SHAPE_LINE, (enumValue = IUS_LINE, &enumValue));
    status |= H5Tenum_insert(hdf_shapeType, TRANSDUCER_SHAPE_CIRCLE, (enumValue = IUS_CIRCLE, &enumValue));
    status |= H5Tenum_insert(hdf_shapeType, TRANSDUCER_SHAPE_PLANE, (enumValue = IUS_PLANE, &enumValue));
    status |= H5Tenum_insert(hdf_shapeType, TRANSDUCER_SHAPE_CYLINDER, (enumValue = IUS_CYLINDER, &enumValue));
    status |= H5Tenum_insert(hdf_shapeType, TRANSDUCER_SHAPE_SPHERE, (enumValue = IUS_SPHERE, &enumValue));
    *pShape = 0;
    status |= H5LTread_dataset( handle, pVariableString , hdf_shapeType, pShape );
    return status;
}

iut_t iusHLBaseTransducerLoad
(
    hid_t handle
)
{
    int status = 0;

    const char *name;
    IusTransducerShape shape;
    float centerFrequency;

    status |= iusHdf5ReadString(handle, NAMEFMT, &name);
    status |= iusHdf5ReadFloat(handle, CENTERFREQUENCYFMT, &(centerFrequency));
    status |= iusHLTransducerLoadShape( handle, SHAPEFMT, &(shape));
    if( status < 0 )
        return IUT_INVALID;
    return iusHLTransducerCreate(name,shape,centerFrequency);
}

iut_t iusHLTransducerLoad
(
    hid_t handle
)
{
	int status = 0;
	IusTransducerShape shape;
    //iut_t transducer = iusHLBaseTransducerLoad(handle);
	hid_t group_id = H5Gopen(handle, IUS_INPUT_STRUCTURE_TRANSDUCER, H5P_DEFAULT);
	status |= iusHLTransducerLoadShape(group_id, SHAPEFMT, &(shape));
	if (status < 0)
		return IUT_INVALID;

    if( shape == IUS_LINE || shape == IUS_CIRCLE )
        return (iut_t) iusHL2DTransducerLoad(group_id);
    if( shape == IUS_PLANE || shape == IUS_CYLINDER || shape == IUS_SPHERE)
        return (iut_t) iusHL3DTransducerLoad(group_id);

	H5Gclose(group_id);
    return IUT_INVALID;
}
