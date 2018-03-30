//=============================================================================
//    COPYRIGHT 2016 PHILIPS RESEARCH
//=============================================================================
//
//  Source Name   : iusInputFileNew.c
//                  InputFile (in hdf5 format (aka v7.3 .mat file)).
//  iusVersion    : 255.255.255.255
//
//=============================================================================

#include "iusInputFile.h"
#include "iusInput.h"
#include "iusError.h"
#include <stdlib.h>    // for calloc
#include <include/iusInputFile.h>
#include <include/iusInput.h>


int iusInputFileSave(IusInputFileInstance *pIFI, int verbose)
{
    if (pIFI->pIusInput == NULL)
    {
        fprintf( stderr, "iusInputFileSave: Input arguments can not be NULL \n");
        return IUS_ERR_VALUE;
    }
    int success = iusInputWrite(pIFI->handle, pIFI->pIusInput, verbose);
    return success;
}

IusInputFileInstance *iusInputFileCreate
(
    const char *pFullFileName
)
{
    hsize_t chunkDims[4];
    hsize_t rfDataDims[4];
    hid_t space;
    hid_t dataset;

    IusInputFileInstance *pFileInst;

    if (pFullFileName == NULL)
    {
    fprintf( stderr, "iusInputFileCreate: Input arguments can not be NULL \n");
    return NULL;
    }

    /*------------------------------------------------------------------------*/
    /* alloc instance ; using calloc to clear all state.                      */
    /*------------------------------------------------------------------------*/
    pFileInst = (IusInputFileInstance *)calloc( 1, sizeof(IusInputFileInstance) );
    if( pFileInst == NULL )
    {
    fprintf( stderr, "iusInputFileCreate: calloc of instance failed\n" );
    return NULL;
    }


    pFileInst->handle = H5Fcreate( pFullFileName, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT );
    if ( pFileInst->handle < 0 )
    {
        return NULL;
    }



#ifdef old
  chunkDims[0] = pInst->pTransducer->numElements;
  chunkDims[1] = pInst->pDrivingScheme->numSamplesPerLine;
  chunkDims[2] = 1;
  chunkDims[3] = 1;

  rfDataDims[0] = pInst->pTransducer->numElements;
  rfDataDims[1] = pInst->pDrivingScheme->numSamplesPerLine;
  rfDataDims[2] = pInst->pDrivingScheme->numTransmitPulses;
  rfDataDims[3] = pInst->numFrames;

  hid_t dataChunkConfig = H5Pcreate(H5P_DATASET_CREATE);
  H5Pset_chunk(dataChunkConfig, 4, chunkDims);

  space = H5Screate_simple(4, rfDataDims, NULL);
  dataset = H5Dcreate(pFileInst->handle, "/rfData", H5T_NATIVE_FLOAT, space, H5P_DEFAULT, dataChunkConfig, H5P_DEFAULT);
  H5Sclose(space);

  /* set state variables */
  pFileInst->rfDataset = dataset;
  pFileInst->pIusInput = pInst;
  pFileInst->fileChunkConfig = dataChunkConfig;
  pFileInst->currentFrame=0;
  pFileInst->currentPulse=0;
  
#endif

  return pFileInst;
}

IusInputFileInstance * iusInputFileOpen
(
    const char * const pFullFileName,
    const int verbose
)
{
    hsize_t  chunkDims[4];
    IusInputFileInstance *pFileInst = (IusInputFileInstance *)calloc(1, sizeof(IusInputFileInstance));
    //check calloc
    if( pFileInst == NULL )
    {
        fprintf( stderr, "iusInputFileCreate: calloc of instance failed\n" );
        return NULL;
    }

    /* Open a Hdf5 file using default properties. */
    pFileInst->handle = H5Fopen( pFullFileName, H5F_ACC_RDONLY, H5P_DEFAULT );
    if (pFileInst->handle <=0)
    {
        fprintf( stderr, "iusInputFileCreate: could not create file: %s\n", pFullFileName );
        return NULL;
    }

    //create pInputInst
    pFileInst->pIusInput = iusInputRead(pFileInst->handle, verbose);
#ifdef old
    chunkDims[0] = pFileInst->pIusInput->pTransducer->numElements;
    chunkDims[1] = pFileInst->pIusInput->pDrivingScheme->numSamplesPerLine;
    chunkDims[2] = 1;
    chunkDims[3] = 1;

    hid_t dataChunkConfig = H5Pcreate(H5P_DATASET_CREATE);
    H5Pset_chunk(dataChunkConfig, 4, chunkDims);

    //assign pFileInst properties
    pFileInst->rfDataset = H5Dopen(pFileInst->handle, "/rfData", H5P_DEFAULT);
    pFileInst->fileChunkConfig = dataChunkConfig;  // TODO get the datachunk info from rfDataSet
    pFileInst->currentFrame=0;
    pFileInst->currentPulse=0;
#endif
    return pFileInst;
}

int iusInputFileRead
(    
    IusInputFileInstance * pInst,
    float *const *const *const *const pRFout   /* 4 starts :-)  please use filetypes :-)*/
)
{
    int i;

    for (i=0; i<pInst->pIusInput->numFrames; i++)
    {
        iusInputFileReadNextFrame(pInst, *pRFout);
    }
    return 0;
}

int iusInputFileReadFrame
(    
    IusInputFileInstance * pInst,
    float *const *const *const pRFout,
    int frameNum
)
{
   pInst->currentFrame=frameNum;
   return iusInputFileReadNextFrame(pInst, pRFout);
}

int iusInputFileReadNextFrame
(    
    IusInputFileInstance * pInst,
    float *const *const *const pRFout
)
{
    int i;
    //IusPage wave;
    for (i=0; i<pInst->pIusInput->pDrivingScheme->numTransmitPulses; i++)
    {
        iusInputFileReadNextPulse(pInst, pRFout[i]);
    }
    return 0;
}

int iusInputFileWriteNextPulse
(
	IusInputFileInstance * pInst,
	const float *pFrame
)
{
  hid_t memspace;
  hid_t dataspace;
  hsize_t offset[4];
  hsize_t count[4];
  hsize_t memdim[2];
  herr_t  status;

  memdim[0] = pInst->pIusInput->pTransducer->numElements;
  memdim[1] = pInst->pIusInput->pReceiveSettings->numSamplesPerLine;
  memspace = H5Screate_simple(2, memdim, NULL);

  if (pInst->pIusInput->numFrames == 1)
  {
      offset[0] = 0;
      offset[1] = 0;
      offset[2] = pInst->currentPulse;

      count[0] = pInst->pIusInput->pTransducer->numElements;
      count[1] = pInst->pIusInput->pReceiveSettings->numSamplesPerLine;
      count[2] = 1;
  }
  else
  {
      offset[0] = 0;
      offset[1] = 0;
      offset[2] = pInst->currentPulse;
      offset[3] = pInst->currentFrame;

      count[0] = memdim[0];
      count[1] = memdim[1];
      count[2] = 1;
      count[3] = 1;
  }
  dataspace = H5Dget_space(pInst->rfDataset);
  status = H5Sselect_hyperslab(dataspace, H5S_SELECT_SET, offset, NULL, count, NULL);
  /*
   *   Write the data to the dataset.
  */
  status |= H5Dwrite(pInst->rfDataset, H5T_NATIVE_FLOAT, memspace, dataspace, H5P_DEFAULT, pFrame);

  /*
   *   Close and release memspace but not (file)dataspace
  */
  status |= H5Sclose(memspace);
  status |= H5Sclose(dataspace);

  /*------------------------------------------------------------------------*/
  /* increment file read pointers                                           */
  /*------------------------------------------------------------------------*/
  pInst->currentPulse++;
  if (pInst->currentPulse == pInst->pIusInput->pDrivingScheme->numTransmitPulses)
  {
      pInst->currentPulse = 0;
      pInst->currentFrame++;
  }

  return status;

}

int iusInputFileReadNextPulse
(    
    IusInputFileInstance * pInst,
    float *const *const pRFout
)
{
    hsize_t i, j;
    hsize_t offset[4];
    hsize_t count[4];
    hid_t   memspace;
    hid_t   dataspace;
    hsize_t memdim[2];
    herr_t  status;

    if (pInst == NULL || pRFout == NULL)
    {
        fprintf(stderr, "iusInputRedNextPulse: input arguments can not be NULL\n");
        return -1;
    }
    float *pPage = (float *)calloc(pInst->pIusInput->pReceiveSettings->numSamplesPerLine *
            pInst->pIusInput->pTransducer->numElements, sizeof(float)) ;
    if (pPage == NULL)
    {
        fprintf(stderr, "iusInputRedNextPulse: Memory allocation error\n");
        return -1;
    }

    offset[0] = 0;
    offset[1] = 0;
    offset[2] = pInst->currentPulse;
    offset[3] = pInst->currentFrame;

    count[1] = pInst->pIusInput->pReceiveSettings->numSamplesPerLine;
    count[0] = pInst->pIusInput->pTransducer->numElements;
    count[2] = 1;
    count[3] = 1;

    memdim[0] = pInst->pIusInput->pTransducer->numElements;
    memdim[1] = pInst->pIusInput->pReceiveSettings->numSamplesPerLine;
    memspace = H5Screate_simple(2, memdim, NULL);
    dataspace = H5Dget_space(pInst->rfDataset);
    status = H5Sselect_hyperslab(dataspace, H5S_SELECT_SET, offset, NULL, count, NULL);

    status |= H5Dread(pInst->rfDataset, H5T_NATIVE_FLOAT, memspace, dataspace, H5P_DEFAULT, pPage);
    /* read an 1D buffer, copy that to 2D array -- might not be needed anymore */
    for (i=0; i<memdim[0] ; i++) // numElements (# of rows rows)
    {
        for (j=0; j<memdim[1]; j++) // numSamples (on each row)
        {
            pRFout[i][j] = pPage[j+i*memdim[1]];
        }
    }
    free(pPage);

    pInst->currentPulse++;
    if (pInst->currentPulse == pInst->pIusInput->pDrivingScheme->numTransmitPulses)
    {
        pInst->currentPulse = 0;
        pInst->currentFrame++;
    }
    status |= H5Sclose(memspace);
    status |= H5Sclose(dataspace);

    return (int)status;

}

int iusInputFileReadFrameDepthRange
(
    IusInputFileInstance * pInst,
    const IusRange *const pDepthRange,
    float *const *const *const pPage,
    int frameNum
)
{
    pInst->currentFrame = frameNum;
    return iusInputFileReadNextFrameDepthRange(pInst, pDepthRange, pPage);

}

int iusInputFileReadNextFrameDepthRange
(
    IusInputFileInstance * pInst,
    const IusRange *const pDepthRange,
    float *const *const *const pPage
 )
{
    int i;
    //IusPage wave;
    for (i = 0; i<pInst->pIusInput->pDrivingScheme->numTransmitPulses; i++)
    {
        iusInputFileReadNextPulseDepthRange(pInst, pDepthRange, pPage[i]);
    }
    return 0;
}

int iusInputFileReadNextPulseDepthRange
(
    IusInputFileInstance * pInst,
    const IusRange *const pDepthRange,
    float *const *const ppPageOut
)
{
    hsize_t i, j;
    hsize_t offset[4];
    hsize_t count[4];

    hid_t   memspace;
    hid_t   dataspace;
    hsize_t memdim[2];
    herr_t  status;

    if (pInst == NULL || ppPageOut == NULL)
    {
        fprintf(stderr, "iusInputRedNextPulse: input arguments can not be NULL\n");
        return -1;
    }
    float *pPage = (float *)calloc(pInst->pIusInput->pReceiveSettings->numSamplesPerLine *
            pInst->pIusInput->pTransducer->numElements, sizeof(float));
    if (pPage == NULL)
    {
        fprintf(stderr, "iusInputRedNextPulse: Memory allocation error\n");
        return -1;
    }

    offset[0] = 0;
    offset[1] = pDepthRange->startIndex;
    offset[2] = pInst->currentPulse;
    offset[3] = pInst->currentFrame;

    count[0] = pInst->pIusInput->pTransducer->numElements;
    count[1] = pDepthRange->numSamples;
    count[2] = 1;
    count[3] = 1;

    memdim[0] = pInst->pIusInput->pTransducer->numElements;
    memdim[1] = pDepthRange->numSamples;
    memspace = H5Screate_simple(2, memdim, NULL);
    dataspace = H5Dget_space(pInst->rfDataset);
    status = H5Sselect_hyperslab(dataspace, H5S_SELECT_SET, offset, NULL, count, NULL);

    status |= H5Dread(pInst->rfDataset, H5T_NATIVE_FLOAT, memspace, dataspace, H5P_DEFAULT, pPage);

    /* read an 1D buffer, copy that to 2D array -- might not be needed anymore */
    for (i = 0; i<memdim[0]; i++) // numElements (# of rows rows)
    {
        for (j = 0; j<memdim[1]; j++) // numSamples (on each row)
        {
            ppPageOut[i][j+offset[1]] = pPage[j + i*memdim[1]];
        }
    }
    free(pPage);

    pInst->currentPulse++;
    if (pInst->currentPulse == pInst->pIusInput->pDrivingScheme->numTransmitPulses)
    {
        pInst->currentPulse = 0;
        pInst->currentFrame++;
    }
    status |= H5Sclose(memspace);
    status |= H5Sclose(dataspace);

    return status;
}

int iusInputFileClose
(
    IusInputFileInstance * pFileInst
)
{
    int success=0;
    IUS_ASSERT_MEMORY(pFileInst);

    if( pFileInst->fileChunkConfig )
        success |= H5Pclose(pFileInst->fileChunkConfig);
    if( pFileInst->rfDataset )
        success |= H5Dclose(pFileInst->rfDataset);
    /* Terminate access to the file. */
    if( pFileInst->handle )
        success |= H5Fclose(pFileInst->handle);
    if (success == 0)
    {
      free(pFileInst);
      pFileInst = NULL;
    }
    return success;
}

