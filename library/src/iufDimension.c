#include <math.h>
#include <stdlib.h>
#include <string.h>

#include <iuf.h>

#define IUF_PATH_DIMENSION_STARTPOS "StartPos"
#define IUF_PATH_DIMENSION_DELTA "Delta"
#define IUF_PATH_DIMENSION_NUMVALUES "NumValues"
#define IUF_PATH_DIMENSION_LABEL "Label"

// ADT
iudim_t iufDimensionCreate
(
	float startPos,
	float delta,
	int numValues,
	char *label
)
{
	IUF_ERR_EVAL_N_RETURN(startPos == NAN, IUDIM_INVALID);
	IUF_ERR_EVAL_N_RETURN(delta == NAN, IUDIM_INVALID);
	IUF_ERR_EVAL_N_RETURN(numValues > 0, IUDIM_INVALID);
	IUF_ERR_CHECK_NULL_N_RETURN(label, IUDIM_INVALID);
	iudim_t created = calloc(1, sizeof(IufDimension));
	IUF_ERR_ALLOC_NULL_N_RETURN(created, IufDimension, IUDIM_INVALID);
	created->startPos = startPos;
	created->delta = delta;
	created->numValues = numValues;
	created->label = strdup(label);
	return created;
}

int iufDimensionDelete
(
	iudim_t dimension
)
{
	IUF_ERR_CHECK_NULL_N_RETURN(dimension, IUF_ERR_VALUE);
	IUF_ERR_CHECK_NULL_N_RETURN(dimension->label, IUF_ERR_VALUE);
	free(dimension->label);
	free(dimension);
	return IUF_E_OK;
}

float iufDimensionGetStartPos
(
	iudim_t iufDimension
)
{
	IUF_ERR_CHECK_NULL_N_RETURN(iufDimension, IUF_ERR_VALUE);
	return iufDimension->startPos;
}

float iufDimensionGetDelta
(
	iudim_t iufDimension
)
{
	IUF_ERR_CHECK_NULL_N_RETURN(iufDimension, IUF_ERR_VALUE);
	return iufDimension->startPos;
}

int iufDimensionGetNumValues
(
	iudim_t iufDimension
)
{
	IUF_ERR_CHECK_NULL_N_RETURN(iufDimension, IUF_ERR_VALUE);
	return iufDimension->numValues;
}

char *iufDimensionGetLabel
(
	iudim_t iufDimension
)
{
	IUF_ERR_CHECK_NULL_N_RETURN(iufDimension, NULL);
	return iufDimension->label;
}

// operations
int iufDimensionCompare
(
	iudim_t reference,
	iudim_t actual
)
{
	if (reference == actual) return IUF_TRUE;
	if (reference == NULL || actual == NULL) return IUF_FALSE;
	if (IUF_EQUAL_FLOAT(reference->startPos, actual->startPos) == IUF_FALSE) return IUF_FALSE;
	if (IUF_EQUAL_FLOAT(reference->delta, actual->delta) == IUF_FALSE) return IUF_FALSE;
	if (IUF_EQUAL_FLOAT(reference->numValues, actual->numValues) == IUF_FALSE) return IUF_FALSE;
	if (strcmp(reference->label, actual->label) != 0) return IUF_FALSE;
	return IUF_TRUE;
}

iudim_t iufDimensionLoad
(
	hid_t handle
)
{
	int status = 0;
	float startPos, delta;
	int numValues;
	char label[IUF_MAX_STRING_LENGTH];
	IUF_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUDIM_INVALID);
	
	status |= iufHdf5ReadFloat(handle, IUF_PATH_DIMENSION_STARTPOS, &(startPos));
	status |= iufHdf5ReadFloat(handle, IUF_PATH_DIMENSION_DELTA, &(delta));
	status |= iufHdf5ReadInt(handle, IUF_PATH_DIMENSION_NUMVALUES, &(numValues));
	status |= iufHdf5ReadString(handle, IUF_PATH_DIMENSION_NUMVALUES, label);
	if (status < 0)
		return IUDIM_INVALID;
	return iufDimensionCreate(startPos, delta, numValues, label);
}

int iufDimensionSave
(
	iudim_t dimension,
	hid_t handle
)
{
	int status = 0;
	IUF_ERR_CHECK_NULL_N_RETURN(dimension, IUF_ERR_VALUE);
	IUF_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUF_ERR_VALUE);

	status |= iufHdf5WriteFloat(handle, IUF_PATH_DIMENSION_STARTPOS, &(dimension->startPos), 1);
	status |= iufHdf5WriteFloat(handle, IUF_PATH_DIMENSION_DELTA, &(dimension->delta), 1);
	status |= iufHdf5WriteInt(handle, IUF_PATH_DIMENSION_NUMVALUES, &(dimension->numValues), 1);
	status |= iufHdf5WriteString(handle, IUF_PATH_DIMENSION_LABEL, dimension->label);

	return status;
}

