
//
// Created by nlv09165 on 23/05/2018.
//
#include <stdlib.h>
#include <string.h>

#include <ius.h>
#include <iusHDF5.h>
#include <iusError.h>
#include <iusTypes.h>
#include <iusUtil.h>

#include <iusPositionImp.h>
#include <iusTransducer.h>
#include <ius3DTransducer.h>
#include <iusTransducerImp.h>
#include <ius3DTransducerElementList.h>
#include <include/ius3DTransducerElementListImp.h>

struct Ius3DTransducer
{
  struct IusTransducer     baseTransducer;
  iu3dtel_t                elements;         /**< an array of numElements transducer element (position, angle, size) */
}  ;

// ADT
iu3dt_t ius3DTransducerCreate
(
  char *name,
  IusTransducerShape shape,
  float centerFrequency,
  int numElements
)
{
    if ( name == NULL ) return IU3DT_INVALID;
    if ( shape == IUS_INVALID_TRANSDUCER_SHAPE ) return IU3DT_INVALID;
    if ( shape == IUS_LINE || shape == IUS_CIRCLE) return IU3DT_INVALID;

    // NAN check
    if ( centerFrequency != centerFrequency ) return IU3DT_INVALID;
    if ( numElements < 0 ) return IU3DT_INVALID;

    iu3dt_t created = calloc(1,sizeof(Ius3DTransducer));
    created->elements = ius3DTransducerElementListCreate(numElements);
    created->baseTransducer.pTransducerName = strdup(name);
    created->baseTransducer.shape = shape;
    created->baseTransducer.centerFrequency = centerFrequency;
    created->baseTransducer.type = IUS_3D_SHAPE;

    return created;
}


int ius3DTransducerDelete
(
iu3dt_t ius3DTransducer
)
{
  int status = IUS_ERR_VALUE;
  if(ius3DTransducer != NULL)
  {
    free(ius3DTransducer);
    ius3DTransducer = NULL;
    status = IUS_E_OK;
  }
  return status;
}


// operations
int ius3DTransducerCompare
(
  iu3dt_t reference,
  iu3dt_t actual
)
{
  if( reference == actual ) return IUS_TRUE;
  if( reference == NULL || actual == NULL ) return IUS_FALSE;
  if( ius3DTransducerElementListGetSize(reference->elements) !=
  ius3DTransducerElementListGetSize(actual->elements) ) return IUS_FALSE;
  if( ius3DTransducerElementListCompare(reference->elements,actual->elements) == IUS_FALSE )  return IUS_FALSE;
  return iusBaseTransducerCompare((iut_t) reference, (iut_t) actual);
}

// getters
iu3dte_t ius3DTransducerGetElement
(
  iu3dt_t transducer,
  int elementIndex
)
{
  if( transducer == NULL ) return IU3DTE_INVALID;
  return ius3DTransducerElementListGet(transducer->elements,elementIndex);
}

int ius3DTransducerGetNumElements
(
  iu3dt_t transducer
)
{
  if( transducer == NULL ) return -1;
  return ius3DTransducerElementListGetSize(transducer->elements);
}

// setters
int ius3DTransducerSetElement
(
  iu3dt_t transducer,
  int elementIndex,
  iu3dte_t element
)
{
  if( transducer == NULL ) return IUS_ERR_VALUE;
  if( elementIndex >= ius3DTransducerElementListGetSize(transducer->elements) ) return IUS_ERR_VALUE;
  if( elementIndex < 0 ) return IUS_ERR_VALUE;
  if( element == NULL ) return IUS_ERR_VALUE;

  return ius3DTransducerElementListSet(transducer->elements,element,elementIndex);
}

#if 0
int ius3DTransducerWriteElementPositions(Ius3DTransducer *pTransducer, hid_t subgroup_id, int verbose)
{
	herr_t        status = 0;
	hid_t position_tid; // File datatype identifier for IusPosition
	hid_t dataset, space;
	hsize_t dims[1] = { 1 };
	Ius3DPosition * pPositionArray;
	int numElements = ius3DTransducerElementListGetSize(pTransducer->elements);
	int i; //iterator

		   /* write the /Transducer/Elements/ positions, angles and sizes are compound types */
	dims[0] = numElements;

	//Positions
	space = H5Screate_simple(1, dims, NULL);
	// step a:  create H5 dataset
	position_tid = H5Tcreate(H5T_COMPOUND, sizeof(Ius3DPosition));
	status |= H5Tinsert(position_tid, "x", HOFFSET(Ius3DPosition, x), H5T_NATIVE_FLOAT);
	status |= H5Tinsert(position_tid, "y", HOFFSET(Ius3DPosition, y), H5T_NATIVE_FLOAT);
	status |= H5Tinsert(position_tid, "z", HOFFSET(Ius3DPosition, z), H5T_NATIVE_FLOAT);
	dataset = H5Dcreate(subgroup_id, "positions", position_tid, space, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

	// step b:  create array of positions
	pPositionArray = (Ius3DPosition *)calloc(numElements, sizeof(Ius3DPosition)); //three dimensions for position
	for (i = 0; i < numElements; i++)
	{
		iu3dte_t element = ius3DTransducerGetElement(pTransducer, i);
		iu3dp_t elemPosition = ius3DTransducerElementGetPosition(element);
		pPositionArray[i].x = elemPosition->x;
		pPositionArray[i].y = elemPosition->y;
		pPositionArray[i].z = elemPosition->z;
	}

	// step c: write the array to the dataset
	status |= H5Dwrite(dataset, position_tid, H5S_ALL, H5S_ALL, H5P_DEFAULT, pPositionArray);

	free(pPositionArray);

	// step d: release resources
	status |= H5Tclose(position_tid);
	status |= H5Sclose(space);
	status |= H5Dclose(dataset);
	return status;
}

int ius3DTransducerWriteElementSizes(Ius3DTransducer *pTransducer, hid_t subgroup_id, int verbose)
{
	herr_t        status = 0;
	hid_t size_tid; // File datatype identifier for IusPosition
	hid_t dataset, space;
	hsize_t dims[1] = { 1 };
	Ius3DSize * pSizeArray;
	int numElements = ius3DTransducerElementListGetSize(pTransducer->elements);
	int i; //iterator

	space = H5Screate_simple(1, dims, NULL);

	// step a:  create H5 dataset
	size_tid = H5Tcreate(H5T_COMPOUND, sizeof(Ius3DSize));
	status |= H5Tinsert(size_tid, "sx", HOFFSET(Ius3DSize, sx), H5T_NATIVE_FLOAT);
	status |= H5Tinsert(size_tid, "sx", HOFFSET(Ius3DSize, sx), H5T_NATIVE_FLOAT);
	status |= H5Tinsert(size_tid, "sz", HOFFSET(Ius3DSize, sz), H5T_NATIVE_FLOAT);
	dataset = H5Dcreate(subgroup_id, "sizes",
		size_tid, space, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

	// step b:  create array of sizes
	pSizeArray = (Ius3DSize *)calloc(numElements, sizeof(Ius3DSize)); //three dimensions for size
	for (i = 0; i < numElements; i++)
	{
		iu3dte_t element = ius3DTransducerGetElement(pTransducer, i);
		iu3ds_t elemSize = ius3DTransducerElementGetSize(element);
		pSizeArray[i].sx = elemSize->sx;
		pSizeArray[i].sy = elemSize->sy;
		pSizeArray[i].sz = elemSize->sz;
	}
	// step c: write the array to the dataset
	status |= H5Dwrite(dataset, size_tid, H5S_ALL, H5S_ALL, H5P_DEFAULT, pSizeArray);

	// step d: release resources
	free(pSizeArray);
	status |= H5Tclose(size_tid);
	status |= H5Sclose(space);
	status |= H5Dclose(dataset);
	return status;
}

int ius3DTransducerWriteElementAngles(Ius3DTransducer *pTransducer, hid_t subgroup_id, int verbose)
{
	herr_t        status = 0;
	hid_t dataset, space;
	Ius3DAngle * pAngleArray;
	int numElements = ius3DTransducerElementListGetSize(pTransducer->elements);
	hsize_t dims[1] = { numElements };
	int i; //iterator

		   // Angles
	space = H5Screate_simple(1, dims, NULL);
	// step a:  create H5 dataset
	dataset = H5Dcreate(subgroup_id, "theta", H5T_NATIVE_FLOAT, space, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

	// step b:  create array of angles
	pAngleArray = (Ius3DAngle *)calloc(numElements, sizeof(Ius3DAngle)); //two dimensions for angle
	for (i = 0; i < numElements; i++)
	{
		iu3dte_t element = ius3DTransducerGetElement(pTransducer, i);
		iu3da_t angle = ius3DTransducerElementGetAngle(element);
		pAngleArray[i].theta = angle->theta;
		pAngleArray[i].phi = angle->phi;
	}

	// step c: write the array to the dataset
	status |= H5Dwrite(dataset, H5T_NATIVE_FLOAT, H5S_ALL, H5S_ALL, H5P_DEFAULT, pAngleArray);

	free(pAngleArray);
	// step d: release resources
	status |= H5Sclose(space);
	status |= H5Dclose(dataset);
	return status;
}
#endif

herr_t ius3DTransducerSave
(
    iu3dt_t transducer,
    hid_t handle
)
{
	herr_t status=0;

    status = iusBaseTransducerSave((iut_t)transducer, handle);
    if (status != 0)
        return status;

    status = ius3DTransducerElementListSave(transducer->elements, handle);
    return status;
}


iu3dt_t ius3DTransducerLoad
(
	hid_t handle
)
{
	iut_t baseTransducer = iusBaseTransducerLoad(handle);
	if (baseTransducer == IUT_INVALID) return IU3DT_INVALID;

	iu3dtel_t elements = ius3DTransducerElementListLoad(handle);
	if (elements == IU3DTEL_INVALID) return IU3DT_INVALID;

	int numElements = ius3DTransducerElementListGetSize(elements);
	iu3dt_t transducer = ius3DTransducerCreate( baseTransducer->pTransducerName,
	                                              baseTransducer->shape,
	                                              baseTransducer->centerFrequency,
	                                              numElements);
	transducer->elements = elements;
	return transducer;
}

