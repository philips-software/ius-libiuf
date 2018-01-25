//==============================================================================
//    COPYRIGHT 2017 PHILIPS RESEARCH
//==============================================================================
//
//  Source Name   : iusCWCFile.c
//                  CWC File handling routines (in hdf5 format )
//  iusVersion    : 255.255.255.255
//
//==============================================================================

#include "iusCWCFile.h"
#include "iusError.h"
#include <stdlib.h>


//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
IusCwcFileInstance * iusCwcFileCreate
(
    const char * const     pFullFileName,
    IusCwcInstance * const pInst,
    int                    version
)
{
    hsize_t chunkDims[4];
    hsize_t cwcDataDims[4];
    hid_t space;
    hid_t dataset;

    IusCwcFileInstance * pFileInst;
    int success = 0;

    if ( pFullFileName == NULL || pInst == NULL )
    {
        fprintf( stderr,
            "iusInputFileCreate: Input arguments can not be NULL \n" );
        return NULL;
    }
    //--------------------------------------------------------------------------
    // alloc instance ; using calloc to clear all state.
    //--------------------------------------------------------------------------
    pFileInst = (IusCwcFileInstance *)calloc( 1, sizeof(IusCwcFileInstance) );
    if ( pFileInst == NULL )
    {
        fprintf( stderr, "iusInputFileCreate: calloc of instance failed\n" );
        return NULL;
    }

    pFileInst->handle = H5Fopen( pFullFileName, H5F_ACC_RDONLY, H5P_DEFAULT );

    chunkDims[0] = pInst->pSampleGrid->numPoints1; // number of Cwc lines
    chunkDims[1] = pInst->pSampleGrid->numPoints2; // number of samples per line
    chunkDims[2] = 1;
    chunkDims[3] = 1;

    cwcDataDims[0] = pInst->pSampleGrid->numPoints1; // number of Cwc lines
    cwcDataDims[1] = pInst->pSampleGrid->numPoints2; // number of samples per line
    cwcDataDims[2] = pInst->numComponents;
    cwcDataDims[3] = 1;

    hid_t dataChunkConfig = H5Pcreate( H5P_DATASET_CREATE );
    H5Pset_chunk( dataChunkConfig, 4, chunkDims );

    space   = H5Screate_simple( 4, cwcDataDims, NULL );
    dataset = H5Dcreate( pFileInst->handle, "/cwcData", H5T_NATIVE_FLOAT, space,
        H5P_DEFAULT, dataChunkConfig, H5P_DEFAULT );
    H5Sclose( space );

    /* set state variables */
    pFileInst->cwcDataset       = dataset;
    pFileInst->pIusCwc          = pInst;
    pFileInst->fileChunkConfig  = dataChunkConfig;
    pFileInst->currentFrame     = 0;
    pFileInst->currentComponent = 0;

    success = iusCwcWrite( pFileInst->handle, pInst, version );

    return success ? pFileInst : NULL;
}


//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
IusCwcFileInstance * iusCwcFileOpen
(
    const char * const pFullFileName,
    const int          verbose
)
{
    hsize_t  chunkDims[4];
    IusCwcFileInstance * pFileInst =
        (IusCwcFileInstance *)calloc(1, sizeof(IusCwcFileInstance));
    IUS_ASSERT_MEMORY( pFileInst );

    /* Open a Hdf5 file using default properties. */
    pFileInst->handle = H5Fopen( pFullFileName, H5F_ACC_RDONLY, H5P_DEFAULT );
    if ( pFileInst->handle <= 0 )
    {
        fprintf( stderr,
            "iusCwcFileOpen: could not create file: %s\n", pFullFileName );
        return NULL;
    }

    //create pCwcInst
    pFileInst->pIusCwc = iusCwcRead( pFileInst->handle, verbose );

    chunkDims[0] = pFileInst->pIusCwc->pSampleGrid->numPoints1; // num lines
    chunkDims[1] = pFileInst->pIusCwc->pSampleGrid->numPoints2; // num samples
    chunkDims[2] = 1;
    chunkDims[3] = 1;

    hid_t dataChunkConfig = H5Pcreate(H5P_DATASET_CREATE);
    H5Pset_chunk( dataChunkConfig, 4, chunkDims );

    //assign pFileInst properties
    pFileInst->cwcDataset       = H5Dopen( pFileInst->handle, "/cwcData",
                                           H5P_DEFAULT );
    // TODO get the datachunk info from rfDataSet
    pFileInst->fileChunkConfig  = dataChunkConfig;
    pFileInst->currentFrame     = 0;
    pFileInst->currentComponent = 0;

    return pFileInst;
}


//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
int iusCwcFileRead
(    
    IusCwcFileInstance * const            pInst,
    float * const * const * const * const ppppCwcOut
)
{
    int i;

    for ( i = 0; i < pInst->pIusCwc->numFrames; i++ )
    {
        iusCwcFileReadNextFrame( pInst, *ppppCwcOut );
    }

    return 0;
}


//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
int iusCwcFileReadFrame
(    
    IusCwcFileInstance * const    pInst,
    float * const * const * const pppCwcOut,
    int                           frameNum
)
{
    pInst->currentFrame = frameNum;

    return iusCwcFileReadNextFrame( pInst, pppCwcOut );
}


//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
int iusCwcFileReadNextFrame
(    
    IusCwcFileInstance * const    pInst,
    float * const * const * const pppCwcOut
)
{
    int r, t, z;
    hsize_t memdim[3];
    hsize_t offset[4];
    hsize_t count[4];
    hid_t memspace, dataspace;
    herr_t status;
    float * pFrame1D;

    memdim[0] = pInst->pIusCwc->pSampleGrid->numPoints1; // lateral, num lines
    memdim[1] = pInst->pIusCwc->pSampleGrid->numPoints2; // radial , num samples
    memdim[2] = pInst->pIusCwc->numComponents;

    pFrame1D  = (float *)malloc( (size_t)(memdim[0] * memdim[1] *
                                 memdim[2] * sizeof(float)) );
    memspace  = H5Screate_simple( 3, memdim, NULL );
    dataspace = H5Dget_space( pInst->cwcDataset );
    if ( pFrame1D == NULL || memspace ==0 || dataspace ==0 )
    {
        fprintf( stderr, "iusCwcFileReadNextFrame: Memory error \n" );
        return -1;
    }

    offset[0] = 0;
    offset[1] = 0;
    offset[2] = 0;
    offset[3] = pInst->currentFrame;

    count[0] = memdim[0];
    count[1] = memdim[1];
    count[2] = memdim[2];
    count[3] = 1;

    status = H5Sselect_hyperslab( dataspace, H5S_SELECT_SET, offset, NULL,
                                  count, NULL );
    if ( status < 0 )
    {
        fprintf( stderr,
            "iusCWXReadNextFrame: Error selecting hyperslab frame: status=%d\n",
            (int)status );
        return -1;
    }
    status = H5Dread( pInst->cwcDataset, H5T_NATIVE_FLOAT, memspace, dataspace,
                      H5P_DEFAULT, pFrame1D );
    if ( status < 0 )
    {
        fprintf( stderr, "iusCWXReadNextFrame: Error reading frame\n" );
        return -1;
    }

    for (z = 0; z < memdim[2]; z++ )
    {
        for ( r = 0; r < memdim[1]; r++ )
        {
            for ( t = 0; t < memdim[0]; t++ )
            {
                pppCwcOut[z][t][r] =
                    pFrame1D[z + r * memdim[2] + t * memdim[1] * memdim[2]];
            }
        }
    }
    free( pFrame1D );

    status  = H5Sclose( memspace );
    status |= H5Sclose( dataspace );

    pInst->currentFrame++;

    return status;
}


//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
int iusCwcFileWriteFrame
(
    IusCwcFileInstance * const          pInst,
    const float * const * const * const pppFrame,
    int                                 frameNum
)
{
     pInst->currentFrame = frameNum;

     return (int)iusCwcFileWriteNextFrame( pInst, pppFrame );
}


//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
int iusCwcFileWriteNextFrame
(
    IusCwcFileInstance * const          pInst,
    const float * const * const * const pppFrame
)
{
    int i;
    int error = 0;

    pInst->currentComponent = 0;
    for ( i = 0; i < pInst->pIusCwc->numComponents; i++ )
    {
        error = (int)LF_CwcFileWriteNextComponent( pInst, pppFrame[i] );
        IUS_ASSERT_VALUE( error == 0 );
    }

    return error;
}


//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
int LF_CwcFileWriteNextComponent
(
    IusCwcFileInstance * const  pInst,
    const float * const * const ppComponent
)
{
    hid_t   memspace;
    hid_t   dataspace;
    hsize_t offset[4];
    hsize_t count[4];
    hsize_t memdim[2];
    herr_t  status;

    memdim[0] = pInst->pIusCwc->pSampleGrid->numPoints1; // num lines
    memdim[1] = pInst->pIusCwc->pSampleGrid->numPoints2; // num samples per line
    memspace  = H5Screate_simple( 2, memdim, NULL );

    if ( pInst->pIusCwc->numFrames == 1 )
    {
        offset[0] = 0;
        offset[1] = 0;
        offset[2] = pInst->currentComponent;

        count[0]  = pInst->pIusCwc->pSampleGrid->numPoints1; // num lines
        count[1]  = pInst->pIusCwc->pSampleGrid->numPoints2; // num samples
        count[2]  = 1;
    }
    else
    {
        offset[0] = 0;
        offset[1] = 0;
        offset[2] = pInst->currentComponent;
        offset[3] = pInst->currentFrame;

        count[0] = memdim[0];
        count[1] = memdim[1];
        count[2] = 1;
        count[3] = 1;
    }
    dataspace = H5Dget_space( pInst->cwcDataset );
    status    = H5Sselect_hyperslab( dataspace, H5S_SELECT_SET, offset, NULL,
                                     count, NULL);

    // Write the data to the dataset.
    status |= H5Dwrite( pInst->cwcDataset, H5T_NATIVE_FLOAT, memspace,
                        dataspace, H5P_DEFAULT, ppComponent );
    
    // Close and release memspace but not (file)dataspace
    status |= H5Sclose( memspace );
    status |= H5Sclose( dataspace );

    //--------------------------------------------------------------------------
    // increment file read pointers
    //--------------------------------------------------------------------------
    pInst->currentComponent++;
    if ( pInst->currentComponent == pInst->pIusCwc->numComponents )
    {
        pInst->currentComponent = 0;
        pInst->currentFrame++;
    }
}


//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
int iusCwcFileWriteFramePolarROI
(
    IusCwcFileInstance * const          pFileInst,
    const float * const * const * const pppFrame,
    int                                 frameNum,
    const IusCwcPolarROI * const        pPolarROI
)
{
    pFileInst->currentFrame = frameNum;

    return iusCwcFileWriteNextFramePolarROI( pFileInst, pppFrame, pPolarROI );
}


//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
int iusCwcFileWriteNextFramePolarROI
(
    IusCwcFileInstance * const          pFileInst,
    const float * const * const * const pppFrame,
    const IusCwcPolarROI * const        pPolarROI
 )
{
    hsize_t memdim[3];
    hsize_t offset[4];
    hsize_t count[4];
    hid_t   memspace;
    hid_t   dataspace;
    herr_t  status;
    int     z, theta, r;  // column and type iterators
    float * pFrame1D;

    memdim[0] = pPolarROI->numThetaGridPointsROI;
    memdim[1] = pPolarROI->numRadiusGridPointsROI;
    memdim[2] = pPolarROI->numComponents;
    memspace  = H5Screate_simple( 3, memdim, NULL );

    pFrame1D = (float *)malloc( memdim[0] * memdim[1] *
                                memdim[2] * sizeof(float) );
    IUS_ASSERT_MEMORY( pFrame1D );

    for ( z = 0; z < memdim[2]; z++ )
    {
        for ( theta = 0; theta < memdim[0]; theta++ )
        {
            for ( r = 0; r < memdim[1]; r++ )
            {
                pFrame1D[z + r*memdim[2] + theta*memdim[1] * memdim[2]] =
                    pppFrame[z][r][theta];
            }
        }
    }

    offset[0] = pPolarROI->startThetaGridPointROI;
    offset[1] = pPolarROI->startRadiusGridPointROI;
    offset[2] = 0;
    offset[3] = pFileInst->currentFrame;

    count[0] = pPolarROI->numThetaGridPointsROI;
    count[1] = pPolarROI->numRadiusGridPointsROI;
    count[2] = pPolarROI->numComponents;
    count[3] = 1;

    dataspace = H5Dget_space( pFileInst->cwcDataset );
    status    = H5Sselect_hyperslab( dataspace, H5S_SELECT_SET, offset, NULL,
                                     count, NULL );
    fprintf( stderr, "writing ROI data to file\n" );
    status    = H5Dwrite( pFileInst->cwcDataset, H5T_NATIVE_FLOAT, memspace,
                          dataspace, H5P_DEFAULT, pFrame1D );

    free( pFrame1D );
    /*
       Close and release memspace but not the file handle to dataspace
     */
    status  = H5Sclose( memspace );
    status |= H5Sclose( dataspace );

    pFileInst->currentFrame++;

    return status;
}


//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
int iusCwcFileClose
(
    IusCwcFileInstance * const pInst
)
{
    int success = -1;

    //iusCwcDestroy( pInst->pCWC4DWDInst );
    if ( pInst->fileChunkConfig )  // Only exists when file has been created
    {
        H5Pclose( pInst->fileChunkConfig );
        pInst->fileChunkConfig = (hid_t)0;
    }
    H5Dclose( pInst->cwcDataset );
    pInst->cwcDataset = (hid_t)0;
    /* Terminate access to the file. */
    success = H5Fclose( pInst->handle );
    pInst->handle = 0;
    free( pInst );

    return success;
}
