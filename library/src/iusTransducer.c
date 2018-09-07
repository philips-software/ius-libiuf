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

#include <iusTransducerImp.h>
#include <ius2DTransducerImp.h>
#include <ius3DTransducerImp.h>
#include <iusTransducerElement.h>
#include <ius2DTransducerElementList.h>
#include <ius3DTransducerElementList.h>

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
    return created;
}

int iusTransducerDelete
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

//
//hid_t iusWriteTransducerType
//(
//	hid_t handle,
//	char *path,
//	IusShape transducerType
//)
//{
//
//	return handle;
//}
//


#define TRANSDUCER_FMT "%s/"
#define TRANSDUCER_ELEMENTS_FMT "%s/Elements/"

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

#define SHAPEFMT "%s/shape"
#define NAMEFMT "%s/transducerName"
#define CENTERFREQUENCYFMT "%s/centerFrequency"

herr_t iusBaseTransducerSave
(
    iut_t transducer,
    char *parentPath,
    hid_t handle
)
{
	herr_t status = 0;
    char path[IUS_MAX_HDF5_PATH];
    hid_t group_id = H5Gcreate(handle, parentPath, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    sprintf(path, SHAPEFMT, parentPath);
    status |= iusBaseTransducerSaveShape(group_id, path, transducer->shape);
    sprintf(path, NAMEFMT, parentPath);
	status |= iusHdf5WriteString(group_id, path, transducer->pTransducerName, 1);
    sprintf(path, CENTERFREQUENCYFMT, parentPath);
	status |= iusHdf5WriteFloat(group_id, path, &(transducer->centerFrequency), 1, 1);
    status |= H5Gclose(group_id );
    return status;
}



herr_t iusTransducerSave
(
    iut_t transducer,
    char *parentPath,
    hid_t handle
)
{
	/* write the /Transducer data */
	herr_t  status=0;
    if (transducer->type == IUS_3D_SHAPE)
		status |= ius3DTransducerSave((Ius3DTransducer *) transducer, parentPath, handle);

	if (transducer->type == IUS_2D_SHAPE)
		status |= ius2DTransducerSave((Ius2DTransducer *) transducer, parentPath, handle);
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
    hid_t handle,
    char *parentPath
)
{
    int status = 0;
    char path[IUS_MAX_HDF5_PATH];

    const char *name;
    IusTransducerShape shape;
    float centerFrequency;

    sprintf(path, NAMEFMT, parentPath);
    status |= iusHdf5ReadString( handle, path, &name);
    sprintf(path, CENTERFREQUENCYFMT, parentPath);
    status |= iusHdf5ReadFloat( handle, path, &(centerFrequency));
    sprintf(path, SHAPEFMT, parentPath);
    status |= iusTransducerLoadShape( handle, path, &(shape));
    if( status < 0 )
        return IUT_INVALID;
    return iusTransducerCreate(name,shape,centerFrequency);
}

iut_t iusTransducerLoad
(
    hid_t handle,
    char *parentPath
)
{
    iut_t transducer = iusBaseTransducerLoad(handle, parentPath);
    if ( transducer == IUT_INVALID )
        return transducer;

    if( transducer->type == IUS_2D_SHAPE )
        return (iut_t) ius2DTransducerLoad(handle, parentPath);
    if( transducer->type == IUS_3D_SHAPE )
        return (iut_t) ius3DTransducerLoad(handle, parentPath);
    return transducer;
}
