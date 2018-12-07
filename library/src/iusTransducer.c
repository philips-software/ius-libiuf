//
// Created by nlv09165 on 24/05/2018.
//
#include <math.h>
#include <string.h>
#include <stdlib.h>

#include <ius.h>
#include <iusTransducerPrivate.h>
#include <ius2DTransducerPrivate.h>
#include <ius3DTransducerPrivate.h>

// ADT
iut_t iusTransducerCreate
(
    const char *name,
    IusTransducerShape shape,
    float centerFrequency
)
{
    if( name == NULL ) return IUT_INVALID;
    if( shape == IUS_INVALID_TRANSDUCER_SHAPE ) return IUT_INVALID;
    if( centerFrequency == NAN ) return IUT_INVALID;
    iut_t created = calloc(1,sizeof(IusTransducer));

    if(shape == IUS_LINE || shape == IUS_CIRCLE)
        created->type = IUS_2D_SHAPE;
    else
        created->type = IUS_3D_SHAPE;

    created->pTransducerName = strdup(name);
    created->shape = shape;
    created->centerFrequency = centerFrequency;
    created->deepDelete = IUS_FALSE;
    return created;
}

int iusTransducerDeepDelete
(
    iut_t iusTransducer
)
{
    int status = IUS_ERR_VALUE;
    if (iusTransducer == NULL) return status;
    iusTransducer->deepDelete = IUS_TRUE;
    if( iusTransducer->type == IUS_2D_SHAPE )
        return ius2DTransducerDeepDelete((iu2dt_t) iusTransducer);
    if( iusTransducer->type == IUS_3D_SHAPE )
        return ius3DTransducerDeepDelete((iu3dt_t) iusTransducer);
    return status;
}


int iusTransducerDelete
(
    iut_t iusTransducer
)
{
    int status = IUS_ERR_VALUE;
    if (iusTransducer == NULL) return status;
    if( iusTransducer->type == IUS_2D_SHAPE )
        return ius2DTransducerDelete((iu2dt_t) iusTransducer);
    if( iusTransducer->type == IUS_3D_SHAPE )
        return ius3DTransducerDelete((iu3dt_t) iusTransducer);
    return status;
}


int iusBaseTransducerDelete
(
    iut_t iusTransducer
)
{
    if (iusTransducer == NULL) return IUS_ERR_VALUE;
    free(iusTransducer->pTransducerName);
    free(iusTransducer);
    return IUS_E_OK;
}


// operations
int iusBaseTransducerCompare
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

int iusTransducerCompare
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
        return ius2DTransducerCompare((iu2dt_t) reference,(iu2dt_t) actual);

    if( reference->type == IUS_3D_SHAPE )
        return ius3DTransducerCompare((iu3dt_t) reference,(iu3dt_t) actual);
    return IUS_FALSE;
}

// Getters
float iusTransducerGetCenterFrequency
(
    iut_t transducer
)
{
      if( transducer == NULL ) return NAN;
      return transducer->centerFrequency;
}

int iusTransducerGetNumElements
(
    iut_t transducer
)
{
    if (transducer == NULL) return -1;
    if( transducer->type == IUS_2D_SHAPE )
        return ius2DTransducerGetNumElements((iu2dt_t)transducer);

    if( transducer->type == IUS_3D_SHAPE )
        return ius3DTransducerGetNumElements((iu3dt_t)transducer);
    return -1;
}

char *iusTransducerGetName
(
    iut_t transducer
)
{
    if (transducer == NULL) return NULL;
    return transducer->pTransducerName;
}

IusTransducerShape iusTransducerGetShape
(
    iut_t transducer
)
{
    if (transducer == NULL) return IUS_INVALID_TRANSDUCER_SHAPE;
    return transducer->shape;
}

iute_t iusTransducerGetElement
(
    iut_t transducer,
    int elementIndex
)
{
    if( transducer->type == IUS_3D_SHAPE)
        return (iute_t) ius3DTransducerGetElement( (iu3dt_t) transducer, elementIndex);

    if( transducer->type == IUS_2D_SHAPE )
        return (iute_t) ius2DTransducerGetElement((iu2dt_t) transducer, elementIndex);

    return IUTE_INVALID;
}

// Setters
int iusTransducerSetElement
(
      iut_t transducer,
      int elementIndex,
      iute_t element
)
{
    int status = IUS_ERR_VALUE;

    if( transducer->type == IUS_3D_SHAPE && iusTransducerElementGetShape(element) == IUS_3D_SHAPE )
        status = ius3DTransducerSetElement( (iu3dt_t) transducer, elementIndex, (iu3dte_t)element );

    if( transducer->type == IUS_2D_SHAPE && iusTransducerElementGetShape(element) == IUS_2D_SHAPE )
        status = ius2DTransducerSetElement((iu2dt_t) transducer, elementIndex, (iu2dte_t)element );

    return status;
}

static herr_t iusBaseTransducerSaveShape(hid_t group_id,
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

herr_t iusBaseTransducerSave
(
    iut_t transducer,
    hid_t handle
)
{
	herr_t status = IUS_E_OK;

	status |= iusBaseTransducerSaveShape(handle, IUS_INPUTFILE_PATH_TRANSDUCER_SHAPE, transducer->shape);
	status |= iusHdf5WriteString(handle, IUS_INPUTFILE_PATH_TRANSDUCER_NAME, transducer->pTransducerName);
	status |= iusHdf5WriteFloat(handle, IUS_INPUTFILE_PATH_TRANSDUCER_CENTERFREQUENCY, &(transducer->centerFrequency), 1);
    
	return status;
}


herr_t iusTransducerSave
(
    iut_t transducer,
    hid_t handle
)
{
	/* write the /Transducer data in "Transducer" */
	herr_t  status=0;
	hid_t transducer_id;

	status = H5Gget_objinfo(handle, IUS_INPUTFILE_PATH_TRANSDUCER, 0, NULL);
	if (status != 0) // the group does not exist yet
		transducer_id = H5Gcreate(handle, IUS_INPUTFILE_PATH_TRANSDUCER, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
	else
		transducer_id = H5Gopen(handle, IUS_INPUTFILE_PATH_TRANSDUCER, H5P_DEFAULT);
    if (transducer_id == H5I_INVALID_HID) return -1;

	status = 0;
    if (transducer->type == IUS_3D_SHAPE)
		status |= ius3DTransducerSave((Ius3DTransducer *) transducer, transducer_id);

	if (transducer->type == IUS_2D_SHAPE)
		status |= ius2DTransducerSave((Ius2DTransducer *) transducer, transducer_id);
	return status;
}

static int iusTransducerLoadShape
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

iut_t iusBaseTransducerLoad
(
    hid_t handle
)
{
    int status = 0;

    char name[IUS_MAX_HDF5_PATH];
    IusTransducerShape shape;
    float centerFrequency;

    status |= iusHdf5ReadString(handle, IUS_INPUTFILE_PATH_TRANSDUCER_NAME, name);
    status |= iusHdf5ReadFloat(handle, IUS_INPUTFILE_PATH_TRANSDUCER_CENTERFREQUENCY, &(centerFrequency));
    status |= iusTransducerLoadShape( handle, IUS_INPUTFILE_PATH_TRANSDUCER_SHAPE, &(shape));
    if( status < 0 )
        return IUT_INVALID;
    return iusTransducerCreate(name,shape,centerFrequency);
}

iut_t iusTransducerLoad
(
    hid_t handle
)
{
	int status = 0;
	IusTransducerShape shape;
    iut_t transducer=IUT_INVALID;
	hid_t group_id = H5Gopen(handle, IUS_INPUTFILE_PATH_TRANSDUCER, H5P_DEFAULT);

	status |= iusTransducerLoadShape(group_id, IUS_INPUTFILE_PATH_TRANSDUCER_SHAPE, &(shape));
	if (status < 0)
		return IUT_INVALID;

    if( shape == IUS_LINE || shape == IUS_CIRCLE )
        transducer = (iut_t) ius2DTransducerLoad(group_id);
    if( shape == IUS_PLANE || shape == IUS_CYLINDER || shape == IUS_SPHERE)
        transducer = (iut_t) ius3DTransducerLoad(group_id);
	H5Gclose(group_id);

	if( transducer != IUT_INVALID )
    {
        transducer->deepDelete = IUS_TRUE;
    }
    return transducer;
}
