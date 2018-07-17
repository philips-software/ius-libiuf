
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
#include <iusHLTransducer.h>
#include <iusHL3DTransducer.h>
#include <iusHLTransducerImp.h>
#include <iusHL3DTransducerElementList.h>

struct Ius3DTransducer
{
  struct IusTransducer     baseTransducer;
  iu3dtel_t                elements;         /**< an array of numElements transducer element (position, angle, size) */
}  ;

// ADT

iu3dt_t iusHL3DTransducerCreate
(
  char *name,
  IusTransducerShape shape,
  float centerFrequency,
  int numElements
)
{
  if( numElements < 0 ) return IU3DT_INVALID;
  iu3dt_t created = calloc(1,sizeof(Ius3DTransducer));
  created->elements = iusHL3DTransducerElementListCreate(numElements);
  created->baseTransducer.pTransducerName = strdup(name);
  created->baseTransducer.shape = shape;
  created->baseTransducer.centerFrequency = centerFrequency;
  created->baseTransducer.type = IUS_3D_SHAPE;

  return created;
}


int iusHL3DTransducerDelete
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
int iusHL3DTransducerCompare
(
  iu3dt_t reference,
  iu3dt_t actual
)
{
  if( reference == actual ) return IUS_TRUE;
  if( reference == NULL || actual == NULL ) return IUS_FALSE;
  if( iusHL3DTransducerElementListGetSize(reference->elements) !=
  iusHL3DTransducerElementListGetSize(actual->elements) ) return IUS_FALSE;
  if( iusHL3DTransducerElementListCompare(reference->elements,actual->elements) == IUS_FALSE )  return IUS_FALSE;
  return IUS_TRUE;
}

// getters
iu3dte_t iusHL3DTransducerGetElement
(
  iu3dt_t transducer,
  int elementIndex
)
{
  if( transducer == NULL ) return IU3DTE_INVALID;
  return iusHL3DTransducerElementListGet(transducer->elements,elementIndex);
}

int iusHL3DTransducerGetNumElements
(
  iu3dt_t transducer
)
{
  if( transducer == NULL ) return -1;
  return iusHL3DTransducerElementListGetSize(transducer->elements);
}

// setters
int iusHL3DTransducerSetElement
(
  iu3dt_t transducer,
  int elementIndex,
  iu3dte_t element
)
{
  if( transducer == NULL ) return IUS_TRUE;
  return iusHL3DTransducerElementListSet(transducer->elements,element,elementIndex);
}

<<<<<<< Updated upstream
hid_t iusHL3DTransducerSave
(
	iu3dt_t transducer,
	char * parentPath,
	hid_t handle
)
{
	return handle;
}
=======

int ius3DTransducerWriteElementPositions(Ius3DTransducer *pTransducer, hid_t subgroup_id, int verbose)
{
	herr_t        status = 0;
	hid_t position_tid; // File datatype identifier for IusPosition
	hid_t dataset, space;
	hsize_t dims[1] = { 1 };
	Ius3DPosition * pPositionArray;
	int numElements = iusHL3DTransducerElementListGetSize(pTransducer->elements);
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
		iu3dte_t element = iusHL3DTransducerGetElement(pTransducer, i);
		iu3dp_t elemPosition = iusHL3DTransducerElementGetPosition(element);
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
	int numElements = iusHL3DTransducerElementListGetSize(pTransducer->elements);
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
		iu3dte_t element = iusHL3DTransducerGetElement(pTransducer, i);
		iu3ds_t elemSize = iusHL3DTransducerElementGetSize(element);
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
	int numElements = iusHL3DTransducerElementListGetSize(pTransducer->elements);
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
		iu3dte_t element = iusHL3DTransducerGetElement(pTransducer, i);
		iu3da_t angle = iusHL3DTransducerElementGetAngle(element);
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

herr_t ius3DTransducerWrite
(
	iu3dt_t transducer,
	hid_t subgroup_id,
	int verbose
)
{
	herr_t status;
	status = ius3DTransducerWriteElementPositions(transducer, subgroup_id, verbose);
	status |= ius3DTransducerWriteElementSizes(transducer, subgroup_id, verbose);
	status |= ius3DTransducerWriteElementAngles(transducer, subgroup_id, verbose);
	return status;
}

>>>>>>> Stashed changes
