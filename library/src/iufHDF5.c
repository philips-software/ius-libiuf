#include <stdio.h>
#include <string.h>
#include <hdf5.h>
#include <hdf5_hl.h>

#include <iuf.h>


//------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------
static int verbose = IUF_FALSE;

herr_t iufHdf5ReadFloat
(
    hid_t        handle,
    const char * pVariableString,
    float *      pValue
)
{
    herr_t status;

    IUF_ASSERT_MEMORY( pVariableString && pValue );

    status = H5LTread_dataset_float( handle, pVariableString, pValue );
    if ( status < 0 )
    {

        IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_HDF5, IUF_ERR_MIN_HDF5, "H5LTread_dataset_float %s failed", pVariableString );
        return status;
    }
    if ( verbose )
    {
        fprintf( stdout, "read: %s %f\n", pVariableString, *pValue );
    }

    return status;
}

//------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------
herr_t iufHdf5ReadShort
(
    hid_t        handle,
    const char * pVariableString,
    short *      pValue
)
{
    herr_t status;

    IUF_ASSERT_MEMORY( pVariableString && pValue );

    status = H5LTread_dataset_short( handle, pVariableString, pValue );
    if ( status < 0 )
    {
        IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_HDF5, IUF_ERR_MIN_HDF5, "H5LTread_dataset_short %s failed", pVariableString );
        return status;
    }
    if ( verbose )
    {
        fprintf( stdout, "read: %s %d\n", pVariableString, *pValue );
    }

    return status;
}

//------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------
herr_t iufHdf5ReadInt
(
    hid_t        handle,
    const char * pVariableString,
    int *        pValue
)
{
    herr_t status;

    IUF_ASSERT_MEMORY( pVariableString && pValue );

    status = H5LTread_dataset_int( handle, pVariableString, pValue );
    if ( status < 0 )
    {
        IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_HDF5, IUF_ERR_MIN_HDF5, "H5LTread_dataset_int %s failed", pVariableString );
        return status;
    }
    if ( verbose )
    {
        fprintf( stdout, "read: %s %d\n", pVariableString, *pValue );
    }

    return status;
}

//------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------
herr_t iufHdf5ReadLong
(
    hid_t        handle,
    const char * pVariableString,
    long int *   pValue
)
{
    herr_t status;

    IUF_ASSERT_MEMORY( pVariableString && pValue );

    status = H5LTread_dataset_long(handle, pVariableString, pValue);
    if ( status < 0 )
    {
        IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_HDF5, IUF_ERR_MIN_HDF5, "H5LTread_dataset_long %s failed", pVariableString );
        return status;
    }
    if ( verbose )
    {
        fprintf( stdout, "read: %s %ld\n", pVariableString, *pValue );
    }

    return status;
}

//------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------
herr_t iufHdf5ReadString
(
    hid_t        handle,
    const char * pVariableString,
    char * ppReturnString
)
{
    herr_t status;
    size_t strLength;

//    IUF_ASSERT_MEMORY( pVariableString && ppReturnString );
    status = H5LTread_dataset_string( handle, pVariableString, ppReturnString );
    if ( status < 0 )
    {
        IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_HDF5, IUF_ERR_MIN_HDF5, "H5LTread_dataset_string %s failed", pVariableString );
        return status;
    }
    if ( verbose )
    {
        strLength = strlen(pVariableString);
        fprintf( stdout, "read: %s [%d]: %s\n", pVariableString, (int) strLength,
                 ppReturnString );
    }
    return status;
}


//------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------
herr_t iufHdf5ReadGridSize
(
hid_t        handle,
const char * pGridName,
int *        pDim0,
int *        pDim1,
int *        pDim2
)
{
    herr_t status = -1;
    char pVariableNameDim1[128];
    char pVariableNameDim2[128];

    IUF_ASSERT_MEMORY( pGridName && pDim0 && pDim1 && pDim2 );

    if ( strcmp( pGridName, "PolarGrid" ) == 0 )
    {
        sprintf( pVariableNameDim2, "%s/numPointsRadial", pGridName );
        sprintf( pVariableNameDim1, "%s/numPointsTheta", pGridName );
        status  = iufHdf5ReadInt( handle, pVariableNameDim2, pDim2 );
        status |= iufHdf5ReadInt( handle, pVariableNameDim1, pDim1 );
        *pDim0 = 0;  // elevation dimension not yet supported
    }
    else if ( strcmp( pGridName, "CartesianGrid" ) == 0 )
    {
        sprintf( pVariableNameDim2, "%s/numPointsZ", pGridName );
        sprintf( pVariableNameDim1, "%s/numPointsX", pGridName );
        status  = iufHdf5ReadInt( handle, pVariableNameDim2, pDim2 );
        status |= iufHdf5ReadInt( handle, pVariableNameDim1, pDim1 );
        *pDim0 = 0;  // elevation dimension not yet supported
    }
    else
    {
        IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_ERROR, IUF_ERR_MIN_ARG_VALUE, "Grid type %s, is not supported", pGridName );
    }

    return status;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
herr_t iufHdf5ReadGrid
(
    hid_t        handle,
    const char * pGridName,
    IufGrid *    pGrid
)
{
    herr_t status = -1;
    char pVariableNameDim1[128];
    char pVariableNameDim2[128];
    int  dims[3];

    IUF_ASSERT_MEMORY( pGrid && pGridName );

    if (strcmp(pGridName, "PolarGrid") == 0 ||
    strcmp(pGridName, "CartesianGrid") == 0)
    {
        status = iufHdf5ReadGridSize( handle, pGridName, &dims[0], &dims[1],
                                      &dims[2] );
        if ( status < 0 )
        {
            IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_HDF5, IUF_ERR_MIN_HDF5, "Reading (iufHdf5ReadGridSize): H5Dread returned: %d", status );
            return status;
        }

        IUF_ASSERT_VALUE( dims[0] == pGrid->numPoints0 );
        IUF_ASSERT_VALUE( dims[1] == pGrid->numPoints1 );
        IUF_ASSERT_VALUE( dims[2] == pGrid->numPoints2 );

        status  = H5LTread_dataset_float( handle, pVariableNameDim2,
                                          pGrid->pPoints2 );
        status |= H5LTread_dataset_float( handle, pVariableNameDim1,
                                          pGrid->pPoints1 );
        //TODO status = H5LTread_dataset_float( handle, pointsZName,
        //         pGrid.pointsZ );
        if ( status < 0 )
        {
            IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_HDF5, IUF_ERR_MIN_HDF5, "Reading (H5LTread_dataset_float): H5Dread returned: %d", status );
            return status;
        }
    }
    else
    {
        IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_ERROR, IUF_ERR_MIN_ARG_VALUE, "Grid type %s, is not supported", pGridName );
        return status;
    }

    return status;
}


//------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------
herr_t iufHdf5WriteFloat
(
    hid_t handle,
    const char *  pVariableString,
    float * pValues,
    int numValues
)
{
    herr_t   returnValue;
    hsize_t  dims[1];

    IUF_ASSERT_MEMORY( pVariableString && pValues );

    dims[0] = (hsize_t)numValues;

    returnValue =
    H5LTmake_dataset_float( handle, pVariableString, 1, dims, pValues );

    if (returnValue !=0 )
    {
        IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_HDF5, IUF_ERR_MIN_HDF5, "H5LTmake_dataset_float returned: %d\n", returnValue);
    }
    return returnValue;
}


//------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------
herr_t iufHdf5WriteInt
(
    hid_t              handle,
    const char * pVariableString,
    int * pValues,
    int  numValues
)
{
    herr_t returnValue;
    hsize_t  dims[1];

    IUF_ASSERT_MEMORY( pVariableString && pValues );

    dims[0] = (hsize_t)numValues;

    returnValue =
    H5LTmake_dataset_int( handle, pVariableString, 1, dims, pValues );
    if ( returnValue != 0 )
    {
        IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_HDF5, IUF_ERR_MIN_HDF5, "H5LTmake_dataset_int returned: %d\n", returnValue);
    }
    return returnValue;
}


//------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------
herr_t iufHdf5WriteLong
(
    hid_t handle,
    const char * pVariableString,
    long * pValues,
    int  numValues
)
{
    herr_t returnValue;
    hsize_t dims[1];

    IUF_ASSERT_MEMORY( pVariableString && pValues );

    dims[0] = (hsize_t)numValues;

    returnValue =
    H5LTmake_dataset_long( handle, pVariableString, 1, dims, pValues );
    if ( returnValue != 0 )
    {
        IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_HDF5, IUF_ERR_MIN_HDF5, "H5LTmake_dataset_long returned: %d\n", returnValue);
    }
    return returnValue;
}


//------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------
herr_t iufHdf5WriteString
(
hid_t              handle,
const char * pVariableString,
const char * pString
)
{
    herr_t returnValue;
    IUF_ASSERT_MEMORY( pVariableString && pString );

    returnValue = H5LTmake_dataset_string( handle, pVariableString, pString );
    if ( returnValue != 0 )
    {
        IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_HDF5, IUF_ERR_MIN_HDF5, "H5LTmake_dataset_string returned: %d\n", returnValue);
    }
    return returnValue;
}


//------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------
herr_t iufHdf5WriteGrid
(
    hid_t        handle,
    const char * pGridName,
    IufGrid *    pGrid
)
{
    herr_t returnValue;
    char pVariableNameDim[128];
    hsize_t dims[1];

    IUF_ASSERT_MEMORY( pGrid && pGridName );

    dims[0] = (hsize_t)1;

    if ( strcmp( pGridName, "PolarGrid" ) == 0 )
    {
        sprintf(pVariableNameDim, "%s/numPointsRadial",pGridName );
        returnValue = H5LTmake_dataset_int( handle, pVariableNameDim, 1,
                                            &dims[0], &(pGrid->numPoints2) );
        sprintf(pVariableNameDim, "%s/numPointsTheta", pGridName );
        returnValue |= H5LTmake_dataset_int( handle, pVariableNameDim, 1,
                                             &dims[0], &(pGrid->numPoints1) );
        //sprintf(pVariableNameDim, "%s/numPointsPhi");
        //returnValue |= H5LTmake_dataset_int( handle, pVariableNameDim, 1,
        //    &dim[0], &(pGrid->numPoints0) );
    }
    else if ( strcmp( pGridName, "CartesianGrid" ) == 0 )
    {
        sprintf( pVariableNameDim, "%s/numPointsZ", pGridName );
        returnValue = H5LTmake_dataset_int( handle, pVariableNameDim, 1,
                                            &dims[0], &(pGrid->numPoints2) );
        sprintf( pVariableNameDim, "%s/numPointsX", pGridName );
        returnValue |= H5LTmake_dataset_int( handle, pVariableNameDim, 1,
                                             &dims[0], &(pGrid->numPoints1) );
        //sprintf(pVariableNameDim, "%s/numPointsY");
        //returnValue |= H5LTmake_dataset_int( handle, pVariableNameDim, 1,
        //                                     &dims[0], &(pGrid->numPoints0) );
    }
    else
    {
        IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_ERROR, IUF_ERR_MIN_ARG_VALUE, "Grid name unknown (%s)\n", pGridName );
        returnValue = -1;
    }

    if ( verbose )
    {
        printf( "iufHdf5WriteGrid: writing grid to hdf5\n" );
    }

    if ( returnValue != 0 )
    {
        IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_HDF5, IUF_ERR_MIN_HDF5, "One of the H5LTmake_dataset_int's returned: %d\n", returnValue);
    }

    return returnValue;
}


