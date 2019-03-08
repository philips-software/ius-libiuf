
//
// Created by nlv12901 on 08/02/2019.
//
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include <ius.h>

#define IUS_PATH_DIMENSION_STARTPOS "StartPos"
#define IUS_PATH_DIMENSION_DELTA "Delta"
#define IUS_PATH_DIMENSION_NUMVALUES "NumValues"
#define IUS_PATH_DIMENSION_LABEL "Label"

// ADT
iudim_t iusDimensionCreate
(
	float startPos,
	float delta,
	int numValues,
	char *label
)
{
	IUS_ERR_EVAL_N_RETURN(startPos == NAN, IUDIM_INVALID);
	IUS_ERR_EVAL_N_RETURN(delta == NAN, IUDIM_INVALID);
	IUS_ERR_EVAL_N_RETURN(numValues > 0, IUDIM_INVALID);
	IUS_ERR_CHECK_NULL_N_RETURN(label, IUDIM_INVALID);
	iudim_t created = calloc(1, sizeof(IusDimension));
	IUS_ERR_ALLOC_NULL_N_RETURN(created, IusDimension, IUDIM_INVALID);
	created->startPos = startPos;
	created->delta = delta;
	created->numValues = numValues;
	created->label = strdup(label);
	return created;
}

int iusDimensionDelete
(
	iudim_t dimension
)
{
	IUS_ERR_CHECK_NULL_N_RETURN(dimension, IUS_ERR_VALUE);
	IUS_ERR_CHECK_NULL_N_RETURN(dimension->label, IUS_ERR_VALUE);
	free(dimension->label);
	free(dimension);
	return IUS_E_OK;
}

float iusDimensionGetStartPos
(
	iudim_t iusDimension
)
{
	IUS_ERR_CHECK_NULL_N_RETURN(iusDimension, IUS_ERR_VALUE);
	return iusDimension->startPos;
}

float iusDimensionGetDelta
(
	iudim_t iusDimension 
)
{
	IUS_ERR_CHECK_NULL_N_RETURN(iusDimension, IUS_ERR_VALUE);
	return iusDimension->startPos;
}

int iusDimensionGetNumValues
(
	iudim_t iusDimension 
)
{
	IUS_ERR_CHECK_NULL_N_RETURN(iusDimension, IUS_ERR_VALUE);
	return iusDimension->numValues;
}

char *iusDimensionGetLabel
(
	iudim_t iusDimension
)
{
	IUS_ERR_CHECK_NULL_N_RETURN(iusDimension, NULL);
	return iusDimension->label;
}

// operations
int iusDimensionCompare
(
	iudim_t reference,
	iudim_t actual
)
{
	if (reference == actual) return IUS_TRUE;
	if (reference == NULL || actual == NULL) return IUS_FALSE;
	if (IUS_EQUAL_FLOAT(reference->startPos, actual->startPos) == IUS_FALSE) return IUS_FALSE;
	if (IUS_EQUAL_FLOAT(reference->delta, actual->delta) == IUS_FALSE) return IUS_FALSE;
	if (IUS_EQUAL_FLOAT(reference->numValues, actual->numValues) == IUS_FALSE) return IUS_FALSE;
	if (strcmp(reference->label, actual->label) != 0) return IUS_FALSE;
	return IUS_TRUE;
}

iudim_t iusDimensionLoad
(
	hid_t handle
)
{
	int status = 0;
	float startPos, delta;
	int numValues;
	char label[IUS_MAX_STRING_LENGTH];
	IUS_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUDIM_INVALID);
	
	status |= iusHdf5ReadFloat(handle, IUS_PATH_DIMENSION_STARTPOS, &(startPos));
	status |= iusHdf5ReadFloat(handle, IUS_PATH_DIMENSION_DELTA, &(delta));
	status |= iusHdf5ReadInt(handle, IUS_PATH_DIMENSION_NUMVALUES, &(numValues));
	status |= iusHdf5ReadString(handle, IUS_PATH_DIMENSION_NUMVALUES, label);
	if (status < 0)
		return IUDIM_INVALID;
	return iusDimensionCreate(startPos, delta, numValues, label);
}

int iusDimensionSave
(
	iudim_t dimension,
	hid_t handle
)
{
	int status = 0;
	IUS_ERR_CHECK_NULL_N_RETURN(dimension, IUS_ERR_VALUE);
	IUS_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUS_ERR_VALUE);

	status |= iusHdf5WriteFloat(handle, IUS_PATH_DIMENSION_STARTPOS, &(dimension->startPos), 1);
	status |= iusHdf5WriteFloat(handle, IUS_PATH_DIMENSION_DELTA, &(dimension->delta), 1);
	status |= iusHdf5WriteInt(handle, IUS_PATH_DIMENSION_NUMVALUES, &(dimension->numValues), 1);
	status |= iusHdf5WriteString(handle, IUS_PATH_DIMENSION_LABEL, dimension->label);

	return status;
}

