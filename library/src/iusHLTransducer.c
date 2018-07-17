
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
#include <iusHLTransducer.h>
#include <iusHL3DTransducer.h>
#include <iusHL2DTransducer.h>
#include <iusHLTransducerImp.h>
#include <iusHLTransducerElement.h>
#include <iusHL2DTransducerElementList.h>
#include <iusHL3DTransducerElementList.h>

// Todo:
//
// nlv09165 -ADt should not be public?
// Refactor to BaseClass routines like PulseDict?
//
// ADT
iut_t iusHLTransducerCreate
(
    char *name,
    IusTransducerShape shape,
    float centerFrequency
)
{
    if( name == NULL ) return IUT_INVALID;
    if( shape == IUS_INVALID_SHAPE ) return IUT_INVALID;
    if( centerFrequency == NAN ) return IUT_INVALID;
    iut_t created = calloc(1,sizeof(IusTransducer));
    if(shape == IUS_LINE || shape == IUS_CIRCLE)
    {
        created->type = IUS_2D_SHAPE;
    }
    else
    {
        created->type = IUS_3D_SHAPE;
    }

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
        iusTransducer = NULL;
        status = IUS_E_OK;
    }
    return status;
}


// operations
int iusHLTransducerCompare
(
    iut_t reference,
    iut_t actual
)
{
    if( reference == actual ) return IUS_TRUE;
    if( reference == NULL || actual == NULL ) return IUS_FALSE;
    if( reference->shape != actual->shape ) return IUS_FALSE;
    if( reference->type != actual->type ) return IUS_FALSE;
    if( IUS_EQUAL_FLOAT(reference->centerFrequency, actual->centerFrequency ) == IUS_FALSE ) return IUS_FALSE;
    if( strcmp( reference->pTransducerName, actual->pTransducerName) != 0 ) return IUS_FALSE;
    if( reference->type == IUS_2D_SHAPE )
    {
        iu2dt_t castedReferenceTransducer =  (iu2dt_t) reference;
        iu2dt_t castedActualTransducer =  (iu2dt_t) actual;
        return iusHL2DTransducerCompare(castedReferenceTransducer,castedActualTransducer);
    }

    if( reference->type == IUS_3D_SHAPE )
    {
        iu3dt_t castedReferenceTransducer =  (iu3dt_t) reference;
        iu3dt_t castedActualTransducer =  (iu3dt_t) actual;
        return iusHL3DTransducerCompare(castedReferenceTransducer,castedActualTransducer);
    }
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
  {
      return iusHL2DTransducerGetNumElements((iu2dt_t)transducer);
  }

  if( transducer->type == IUS_3D_SHAPE )
  {
      return iusHL3DTransducerGetNumElements((iu3dt_t)transducer);
  }

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
  {
    iu3dt_t castedTransducer = (iu3dt_t) transducer;
    return (iute_t) iusHL3DTransducerGetElement(castedTransducer, elementIndex);
  }

  if( transducer->type == IUS_2D_SHAPE )
  {
    iu2dt_t castedTransducer = (iu2dt_t) transducer;
    return (iute_t) iusHL2DTransducerGetElement(castedTransducer, elementIndex);
  }
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
  {
    iu3dt_t castedTransducer = (iu3dt_t) transducer;
    iu3dte_t castedTransducerElement = (iu3dte_t)element;
    status = iusHL3DTransducerSetElement(
    castedTransducer,
    elementIndex,
    castedTransducerElement
    );
  }

  if( transducer->type == IUS_2D_SHAPE && iusHLTransducerElementGetShape(element) == IUS_2D_SHAPE )
  {
    iu2dt_t castedTransducer = (iu2dt_t) transducer;
    iu2dte_t castedTransducerElement = (iu2dte_t)element;
    status = iusHL2DTransducerSetElement(
    castedTransducer,
    elementIndex,
    castedTransducerElement
    );
  }

  return status;
}


hid_t iusWriteTransducerType
(
	hid_t handle,
	char *path,
	IusShape transducerType
)
{

	return handle;
}


// serialization

#define LABELPATH          "%s/%s"
#define TRANSDUCER_ELEMENTS_FMT "%s/"

static herr_t iusWriteShape(hid_t group_id, char *pVariableString, IusTransducerShape shape, int verbose)
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

static herr_t iusWriteBaseTransducer
(
	IusTransducer * pTransducer,
	hid_t group_id,
	int verbose
)
{
	herr_t status = 0;
	status |= iusWriteShape(groupd_id, "/Transducer/shape", pTransducer->shape, verbose);
	status |= iusHdf5WriteString(groupd_id, "/Transducer/transducerName", pTransducer->pTransducerName, 1, verbose);
	status |= iusHdf5WriteFloat(groupd_id, "/Transducer/centerFrequency", &(pTransducer->centerFrequency), 1, verbose);
	return status;
}

herr_t iusHLTransducerSave
(
	iut_t transducer,
	char *parentPath,
	hid_t group_id
)
{
	/* write the /Transducer data */
	herr_t  status;
	hsize_t dims[1] = { 1 };
	hid_t   subgroup_id;
	char    path[128];

	status = iusWriteBaseTransducer(transducer, group_id, 1);
	if (status < 0)
	{
		return status;
	}

	snprintf(path, 128, TRANSDUCER_ELEMENTS_FMT, parentPath);
	subgroup_id = H5Gcreate(group_id, path, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
	if (transducer->type == IUS_3D_SHAPE)
	{
		Ius3DTransducer *p3DTransducer = (Ius3DTransducer *)transducer;
		status |= iusWrite3DTransducer(p3DTransducer, subgroup_id, 1);
	}

	if (transducer->type == IUS_2D_SHAPE)
	{
		Ius2DTransducer *p2DTransducer = (Ius2DTransducer *)transducer;
		status |= iusWrite2DTransducer(p2DTransducer, subgroup_id, 1);
	}
	status |= H5Gclose(subgroup_id);

	return status;
}