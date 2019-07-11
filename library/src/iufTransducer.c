#include <math.h>
#include <string.h>
#include <stdlib.h>

#include <iuf.h>
#include <iufTransducerADT.h>
#include <iufTransducerPrivate.h>
#include <iuf2DTransducerPrivate.h>
#include <iuf3DTransducerPrivate.h>

// ADT
iut_t iufTransducerCreate
(
    const char *name,
    IufTransducerShape shape,
    float centerFrequency
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(name, IUT_INVALID);
    if( shape == IUF_INVALID_TRANSDUCER_SHAPE )
    {
        IUF_ERROR_PUSH(IUF_ERR_MAJ_VALUE, IUF_ERR_MIN_ARG_VALUE, "shape argument was invalid");
        return IUT_INVALID;
    }

    if( centerFrequency == NAN )
    {
        IUF_ERROR_PUSH(IUF_ERR_MAJ_VALUE, IUF_ERR_MIN_ARG_VALUE, "centerFrequency argument was invalid");
        return IUT_INVALID;
    }

    iut_t created = calloc(1,sizeof(IufTransducer));
    IUF_ERR_ALLOC_NULL_N_RETURN(created, IufTransducer, IUT_INVALID);

    if(shape == IUF_LINE || shape == IUF_CIRCLE)
        created->type = IUF_2D_SHAPE;
    else
        created->type = IUF_3D_SHAPE;

    created->pTransducerName = strdup(name);
    created->shape = shape;
    created->centerFrequency = centerFrequency;
    created->deepDelete = IUF_FALSE;
    return created;
}

int iufTransducerDeepDelete
(
    iut_t transducer
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(transducer, IUF_ERR_VALUE);
    transducer->deepDelete = IUF_TRUE;
    if( transducer->type == IUF_2D_SHAPE )
        return iuf2DTransducerDeepDelete((iu2dt_t) transducer);
    if( transducer->type == IUF_3D_SHAPE )
        return iuf3DTransducerDeepDelete((iu3dt_t) transducer);

    IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_VALUE, IUF_ERR_MIN_ARG_VALUE, "invalid transducer transducer shape: '%d'", transducer->type );
    return IUF_ERR_VALUE;
}


int iufTransducerDelete
(
    iut_t transducer
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(transducer, IUF_ERR_VALUE);
    if( transducer->type == IUF_2D_SHAPE )
        return iuf2DTransducerDelete((iu2dt_t) transducer);
    if( transducer->type == IUF_3D_SHAPE )
        return iuf3DTransducerDelete((iu3dt_t) transducer);

    IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_VALUE, IUF_ERR_MIN_ARG_VALUE, "invalid transducer transducer shape: '%d'", transducer->type );
    return IUF_ERR_VALUE;
}


int iufBaseTransducerDelete
(
    iut_t transducer
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(transducer, IUF_ERR_VALUE);
    free(transducer->pTransducerName);
    free(transducer);
    return IUF_E_OK;
}


// operations
int iufBaseTransducerCompare
(
    iut_t reference,
    iut_t actual
)
{
    if( reference == actual )
        return IUF_TRUE;
    if( reference == NULL || actual == NULL )
        return IUF_FALSE;
    if( reference->shape != actual->shape )
        return IUF_FALSE;
    if( reference->type != actual->type )
        return IUF_FALSE;
    if( IUF_EQUAL_FLOAT(reference->centerFrequency, actual->centerFrequency ) == IUF_FALSE )
        return IUF_FALSE;
    if( strcmp( reference->pTransducerName, actual->pTransducerName) != 0 )
        return IUF_FALSE;
    return IUF_TRUE;
}

int iufTransducerCompare
(
    iut_t reference,
    iut_t actual
)
{
    if( reference == actual )
        return IUF_TRUE;
    if( reference == NULL || actual == NULL )
        return IUF_FALSE;

    if( reference->type == IUF_2D_SHAPE )
        return iuf2DTransducerCompare((iu2dt_t) reference,(iu2dt_t) actual);

    if( reference->type == IUF_3D_SHAPE )
        return iuf3DTransducerCompare((iu3dt_t) reference,(iu3dt_t) actual);

    return iufBaseTransducerCompare(reference,actual);
}

// Getters
float iufTransducerGetCenterFrequency
(
    iut_t transducer
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(transducer, NAN);
    if (transducer->type == IUF_2D_SHAPE)
      return iuf2DTransducerGetCenterFrequency((iu2dt_t)transducer);
    if (transducer->type == IUF_3D_SHAPE)
        return iuf3DTransducerGetCenterFrequency((iu3dt_t)transducer);
    IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_VALUE, IUF_ERR_MIN_ARG_VALUE, "invalid transducer transducer shape: '%d'", transducer->type);
    return 0.0f;
}

int iufTransducerGetNumElements
(
    iut_t transducer
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(transducer, -1);
    if( transducer->type == IUF_2D_SHAPE )
        return iuf2DTransducerGetNumElements((iu2dt_t)transducer);
    if( transducer->type == IUF_3D_SHAPE )
        return iuf3DTransducerGetNumElements((iu3dt_t)transducer);

    IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_VALUE, IUF_ERR_MIN_ARG_VALUE, "invalid transducer transducer shape: '%d'", transducer->type );
    return -1;
}

char *iufTransducerGetName
(
    iut_t transducer
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(transducer, NULL);
    if (transducer->type == IUF_2D_SHAPE)
        return iuf2DTransducerGetName((iu2dt_t)transducer);
    if (transducer->type == IUF_3D_SHAPE)
        return iuf3DTransducerGetName((iu3dt_t)transducer);
    IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_VALUE, IUF_ERR_MIN_ARG_VALUE, "invalid transducer transducer shape: '%d'", transducer->type);
    return NULL;
}

IufTransducerShape iufTransducerGetShape
(
    iut_t transducer
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(transducer, IUF_INVALID_TRANSDUCER_SHAPE);
    if (transducer->type == IUF_2D_SHAPE)
        return iuf2DTransducerGetShape((iu2dt_t)transducer);
    if (transducer->type == IUF_3D_SHAPE)
        return iuf3DTransducerGetShape((iu3dt_t)transducer);
    
    IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_VALUE, IUF_ERR_MIN_ARG_VALUE, "invalid transducer transducer shape: '%d'", transducer->type);
    return -1;
}

iute_t iufTransducerGetElement
(
    iut_t transducer,
    int elementIndex
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(transducer, IUTE_INVALID);
    if( transducer->type == IUF_3D_SHAPE)
        return (iute_t) iuf3DTransducerGetElement( (iu3dt_t) transducer, elementIndex);

    if( transducer->type == IUF_2D_SHAPE )
        return (iute_t) iuf2DTransducerGetElement((iu2dt_t) transducer, elementIndex);

    IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_VALUE, IUF_ERR_MIN_ARG_VALUE, "invalid transducer transducer shape: '%d'", transducer->type );
    return IUTE_INVALID;
}

// Setters
int iufTransducerSetElement
(
      iut_t transducer,
      int elementIndex,
      iute_t element
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(transducer, IUF_ERR_VALUE);
    if( transducer->type == IUF_3D_SHAPE && iufTransducerElementGetShape(element) == IUF_3D_SHAPE )
        return iuf3DTransducerSetElement( (iu3dt_t) transducer, elementIndex, (iu3dte_t)element );

    if( transducer->type == IUF_2D_SHAPE && iufTransducerElementGetShape(element) == IUF_2D_SHAPE )
        return iuf2DTransducerSetElement((iu2dt_t) transducer, elementIndex, (iu2dte_t)element );

    IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_VALUE, IUF_ERR_MIN_ARG_VALUE, "invalid transducer transducer shape: '%d'", transducer->type );
    return IUF_ERR_VALUE;
}

static herr_t iufBaseTransducerSaveShape(hid_t group_id,
                                           const char *pVariableString,
                                           IufTransducerShape shape)
{
    herr_t status = 0;
    hsize_t dims[1] = { 1 };
    /* Based on a native signed short */
    hid_t hdf_shapeType = H5Tcreate(H5T_ENUM, sizeof(short));
    short enumValue;
    status |= H5Tenum_insert(hdf_shapeType, TRANSDUCER_SHAPE_LINE, (enumValue = IUF_LINE, &enumValue));
    status |= H5Tenum_insert(hdf_shapeType, TRANSDUCER_SHAPE_CIRCLE, (enumValue = IUF_CIRCLE, &enumValue));
    status |= H5Tenum_insert(hdf_shapeType, TRANSDUCER_SHAPE_PLANE, (enumValue = IUF_PLANE, &enumValue));
    status |= H5Tenum_insert(hdf_shapeType, TRANSDUCER_SHAPE_CYLINDER, (enumValue = IUF_CYLINDER, &enumValue));
    status |= H5Tenum_insert(hdf_shapeType, TRANSDUCER_SHAPE_SPHERE, (enumValue = IUF_SPHERE, &enumValue));
    enumValue = shape;
    status |= H5LTmake_dataset(group_id, pVariableString, 1, dims, hdf_shapeType, &enumValue);
    status |= H5Tclose(hdf_shapeType);
    return status;
}

herr_t iufBaseTransducerSave
(
    iut_t transducer,
    hid_t handle
)
{
    herr_t status = IUF_E_OK;
    IUF_ERR_CHECK_NULL_N_RETURN(transducer, IUF_ERR_VALUE);
    IUF_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUF_ERR_VALUE);
    status |= iufBaseTransducerSaveShape(handle, IUF_INPUTFILE_PATH_TRANSDUCER_SHAPE, transducer->shape);
    status |= iufHdf5WriteString(handle, IUF_INPUTFILE_PATH_TRANSDUCER_NAME, transducer->pTransducerName);
    status |= iufHdf5WriteFloat(handle, IUF_INPUTFILE_PATH_TRANSDUCER_CENTERFREQUENCY, &(transducer->centerFrequency), 1);


    if (status != IUF_E_OK)
    {
        IUF_ERROR_PUSH(IUF_ERR_MAJ_HDF5, IUF_ERR_MIN_HDF5, "during write of shape, name or center frequency of transducer");
    }
    return status;
}


herr_t iufTransducerSave
(
    iut_t transducer,
    hid_t handle
)
{
    /* write the /Transducer data in "Transducer" */
    hid_t transducer_id;
    herr_t status = H5Gget_objinfo(handle, IUF_INPUTFILE_PATH_TRANSDUCER, 0, NULL);
    IUF_ERR_CHECK_NULL_N_RETURN(transducer, IUF_ERR_VALUE);
    if (status != 0) // the group does not exist yet
        transducer_id = H5Gcreate(handle, IUF_INPUTFILE_PATH_TRANSDUCER, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    else
        transducer_id = H5Gopen(handle, IUF_INPUTFILE_PATH_TRANSDUCER, H5P_DEFAULT);
    if (transducer_id == H5I_INVALID_HID)
    {
        IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_HDF5, IUF_ERR_MIN_HDF5, "Error getting handle for path: %s", IUF_INPUTFILE_PATH_TRANSDUCER);
        return IUF_ERR_VALUE;
    }

    if (transducer->type == IUF_3D_SHAPE)
    {
        status = iuf3DTransducerSave((Iuf3DTransducer *) transducer, transducer_id);
        status |= H5Gclose(transducer_id);
        return status;
    }

    if (transducer->type == IUF_2D_SHAPE)
    {
        status = iuf2DTransducerSave((Iuf2DTransducer *) transducer, transducer_id);
        status |= H5Gclose(transducer_id);
        return status;
    }

    IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_VALUE, IUF_ERR_MIN_ARG_VALUE, "invalid transducer transducer shape: '%d'", transducer->type );

    /* Close must be called in the transducer type specific save function*/
    H5Gclose(transducer_id);
    return IUF_ERR_VALUE;
}

static int iufTransducerLoadShape
(
    hid_t handle,
    char *pVariableString,
    IufTransducerShape *pShape
)
{
    herr_t status = 0;
    /* Based on a native signed short */
    hid_t hdf_shapeType = H5Tcreate( H5T_ENUM, sizeof(short) );
    short enumValue;
    status |= H5Tenum_insert(hdf_shapeType, TRANSDUCER_SHAPE_LINE, (enumValue = IUF_LINE, &enumValue));
    status |= H5Tenum_insert(hdf_shapeType, TRANSDUCER_SHAPE_CIRCLE, (enumValue = IUF_CIRCLE, &enumValue));
    status |= H5Tenum_insert(hdf_shapeType, TRANSDUCER_SHAPE_PLANE, (enumValue = IUF_PLANE, &enumValue));
    status |= H5Tenum_insert(hdf_shapeType, TRANSDUCER_SHAPE_CYLINDER, (enumValue = IUF_CYLINDER, &enumValue));
    status |= H5Tenum_insert(hdf_shapeType, TRANSDUCER_SHAPE_SPHERE, (enumValue = IUF_SPHERE, &enumValue));
    *pShape = 0;
    status |= H5LTread_dataset( handle, pVariableString , hdf_shapeType, pShape );
    status |= H5Tclose(hdf_shapeType);
    return status;
}

iut_t iufBaseTransducerLoad
(
    hid_t handle
)
{
    int status = IUF_E_OK;
    char name[IUF_MAX_HDF5_PATH];
    IufTransducerShape shape;
    float centerFrequency;

    IUF_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUT_INVALID);
    status |= iufHdf5ReadString(handle, IUF_INPUTFILE_PATH_TRANSDUCER_NAME, name);
    status |= iufHdf5ReadFloat(handle, IUF_INPUTFILE_PATH_TRANSDUCER_CENTERFREQUENCY, &(centerFrequency));
    status |= iufTransducerLoadShape( handle, IUF_INPUTFILE_PATH_TRANSDUCER_SHAPE, &(shape));

    if (status != IUF_E_OK)
    {
        IUF_ERROR_PUSH(IUF_ERR_MAJ_HDF5, IUF_ERR_MIN_HDF5, "during load of shape, name or center frequency of transducer");
        return IUT_INVALID;
    }

    return iufTransducerCreate(name,shape,centerFrequency);
}

iut_t iufTransducerLoad
(
    hid_t handle
)
{
    int status = 0;
    IufTransducerShape shape;

    IUF_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUT_INVALID);
    iut_t transducer=IUT_INVALID;
    hid_t group_id = H5Gopen(handle, IUF_INPUTFILE_PATH_TRANSDUCER, H5P_DEFAULT);

    status |= iufTransducerLoadShape(group_id, IUF_INPUTFILE_PATH_TRANSDUCER_SHAPE, &(shape));
    if (status < 0)
        return IUT_INVALID;

    if( shape == IUF_LINE || shape == IUF_CIRCLE )
        transducer = (iut_t) iuf2DTransducerLoad(group_id);
    if( shape == IUF_PLANE || shape == IUF_CYLINDER || shape == IUF_SPHERE)
        transducer = (iut_t) iuf3DTransducerLoad(group_id);
    H5Gclose(group_id);

    if( transducer != IUT_INVALID )
    {
        transducer->deepDelete = IUF_TRUE;
    }
    return transducer;
}
