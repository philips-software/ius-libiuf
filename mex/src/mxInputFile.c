/** ===========================================================================
 *    COPYRIGHT 2018 PHILIPS RESEARCH
 *  ===========================================================================
 *
 * \file mxInputFile.c
 * \brief Mex file for reading and writing a version 3 input file type.
 *
 *  ===========================================================================
*/
#include "ius.h"
#include <iusInputFilePrivate.h>
#include "mxHandleList.h"

#include "mex.h"
#include <string.h> /*strncmp*/

//-----------------------------------------------------------------------------
// M A C R O S       
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// G L O B A L S
//-----------------------------------------------------------------------------
#define MXIF_IDBASE 1.0000;
#define MXIF_IDSTEP 0.0001;

static mxh_t inputFileList = MXH_INVALID;
static double mxif_id = MXIF_IDBASE;

//-----------------------------------------------------------------------------
// L O C A L   F U N C T I O N S
//-----------------------------------------------------------------------------
static void LF_cleanup(void)
{
    iuif_t iusInputFile;
    while (inputFileList != MXH_INVALID)
    {   
        iusInputFile = (iuif_t)mxHandleListRemoveFirst(&inputFileList);        
        
        if (iusInputFile == IUIF_INVALID)
        {
            mexErrMsgIdAndTxt("mxInputFile:mexatexit", "Invalid iusInputFile handle.");
        }

        if (iusInputFileClose(iusInputFile) != IUS_E_OK)
        {
            mexErrMsgIdAndTxt("mxInputFile:mexatexit", "Unable to close file, file already closed?");
        }
        if (iusInputFileDelete(iusInputFile) != IUS_E_OK)
        {
            mexErrMsgIdAndTxt("mxInputFile:mexatexit", "Unable to delete instance.");
        }
    }
    mexPrintf("All resources released!\n");
}

static void LF_Validate_Inputs( char * sMethod, int nlhs, int nrhs, int nlhs_lo, int nlhs_hi, int nrhs_lo, int nrhs_hi )
{
    char * msg = NULL;
    if (nlhs < nlhs_lo)
    {
        msg = "mxInputFile: Too few output arguments.";
    }
    else if( nlhs > nlhs_hi )
    {
        msg = "mxInputFile: Too many output arguments.";
    }
    else if (nrhs < nrhs_lo)
    {
        msg = "mxInputFile: Too few output arguments.";
    }
    else if (nrhs > nrhs_hi)
    {
        msg = "mxInputFile: Too few output arguments.";
    }
    if (msg)
    {
        mexErrMsgIdAndTxt("mxInputFile:%s", msg, sMethod);
    }
}

void LF_create(int nlhs, mxArray * plhs[], int nrhs, const mxArray * prhs[])
{
    mwSize filenameLen;
    char * filename;
    double handleId;
    double * pDouble;
    iuif_t iusInputFile;

    /* Retrieve filename */
    filenameLen = (mwSize)mxGetN(prhs[1]) * sizeof(mxChar) + 1;
    if (filenameLen == 1)
    {
        mexErrMsgIdAndTxt("mxInputFile:create", "Filename must have non-zero length.");
    }

    filename = (char*)mxMalloc(filenameLen);
    if (filename == NULL)
    {
        mexErrMsgIdAndTxt("mxInputFile:create", "Unable to allocate memory.");
    }

    if (mxGetString(prhs[1], filename, filenameLen) != 0)
    {
        mexErrMsgIdAndTxt("mxInputFile:create", "Could not convert method string.");
    }

    /* Create new input file */
    iusInputFile = iusInputFileCreate(filename);
    if (iusInputFile == IUIF_INVALID)
    {
        mexErrMsgIdAndTxt("mxInputFile:create", "Unable to create file, file already open?");
    }
    /* Register a mexAtExit function */
    mexAtExit(LF_cleanup);

    /* Add file handle to a handle list */
    handleId = mxHandleListAppend(&inputFileList, iusInputFile );

    /* Return file identifier */
    plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL);
    pDouble = (double*)mxGetData(plhs[0]);
    pDouble[0] = handleId;

    mexPrintf("iusInputFile '%s' opened.\n", filename);

    /* Cleanup memory */
    mxFree(filename);
}

void LF_close(int nlhs, mxArray * plhs[], int nrhs, const mxArray * prhs[])
{
    double handleId;
    iuif_t iusInputFile;

    handleId = mxGetScalar(prhs[1]);
    iusInputFile = (iuif_t)mxHandleListRemoveById(&inputFileList, handleId);

    if (iusInputFile == IUIF_INVALID)
    {
        mexErrMsgIdAndTxt("mxInputFile:close", "Unable to find file, file already closed?");
    }
    if (iusInputFileClose(iusInputFile) != IUS_E_OK)
    {
        mexErrMsgIdAndTxt("mxInputFile:close", "Unable to close file, file already closed?");
    }
    if (iusInputFileDelete(iusInputFile) != IUS_E_OK)
    {
        mexErrMsgIdAndTxt("mxInputFile:close", "Unable to delete instance.");
    }
    mexPrintf("iusInputFile closed.\n");
}

//-----------------------------------------------------------------------------
// M E X   G A T E W A Y   F U N C T I O N
//-----------------------------------------------------------------------------
void mexFunction
(
    int             nlhs,   // number of left-hand-side arguments
    mxArray *       plhs[], // the left-hand-side arguments
    int             nrhs,   // number of right-hand-side arguments
    const mxArray * prhs[]  // the right-hand-side arguments
)
{
    char * sMethod;
    mwSize methodLen;

    //-------------------------------------------------------------------------
    // Retrieve method from the first input argument (i.e. from prhs[0])
    //-------------------------------------------------------------------------
    if( nrhs == 0 )
    {
        mexErrMsgTxt( "mxInputFile: No input arguments provided." );
    }
    methodLen = (mwSize)mxGetN( prhs[ 0 ] ) * sizeof( mxChar ) + 1;
    sMethod   = (char*)mxMalloc( methodLen );
    if( mxGetString( prhs[ 0 ], sMethod, methodLen ) != 0 )
    {
        mexErrMsgTxt( "mxInputFile: Could not convert method string." );
    }

    //---------------------------------------------------------------------
    // Call the C-algorithm function that belongs to the retrieved method  
    //---------------------------------------------------------------------
    if( strncmp( sMethod, "create", methodLen ) == 0 )
    {        
        LF_Validate_Inputs(sMethod, nlhs, nrhs, 0, 1, 2, 2);
        LF_create(nlhs, plhs, nrhs, prhs);
    }
    else if( strncmp( sMethod, "frameCreate", methodLen ) == 0 )
    {
        mexPrintf( sMethod );
    }
    else if( strncmp( sMethod, "responseCreate", methodLen ) == 0 )
    {
        mexPrintf( sMethod );
    }
    else if( strncmp( sMethod, "channelCreate", methodLen ) == 0 )
    {
        mexPrintf( sMethod );
    }
    else if( strncmp( sMethod, "delete", methodLen ) == 0 )
    {
        mexPrintf( sMethod );
    }
    else if( strncmp( sMethod, "nodeLoad", methodLen ) == 0 )
    {
        mexPrintf( sMethod );
    }
    else if( strncmp( sMethod, "nodeSave", methodLen ) == 0 )
    {
        mexPrintf( sMethod );
    }
    else if( strncmp( sMethod, "close", methodLen ) == 0 )
    {
        LF_Validate_Inputs(sMethod, nlhs, nrhs, 0, 0, 2, 2);
        LF_close(nlhs, plhs, nrhs, prhs);

#if( 0 )
        double * pData;
        iuif_t iusInputFile;
        mwSize mrows = mxGetM(prhs[1]);
        mwSize mcols = mxGetN(prhs[1]);

        if (nrhs < 1)
        {
            mexErrMsgTxt("mxInputFile: No handle provided.");
        }
        if (nrhs > 2)
        {
            mexErrMsgTxt("mxInputFile: Too many input arguments.");
        }
        if (nlhs > 0)
        {
            mexErrMsgTxt("mxInputFile: Too many output arguments.");
        }
        if (!mxIsDouble(prhs[1]) || mxIsComplex(prhs[1]) || mrows != 1 || mcols != 1)
        {
            mexPrintf("%d %d", mrows, mcols);
            mexErrMsgTxt("mxInputFile: Input must be a real valued scalar double.");
        }

        /* Retrieve file handle */
        pData = mxGetPr(plhs[1]);
        iusInputFile = (iuif_t)(pData);
        mexPrintf("Output double: %d\n", *(double *)iusInputFile);
#endif        
    }
    else if( strncmp( sMethod, "compare", methodLen ) == 0 )
    {
        mexPrintf( sMethod );
    }
    else if( strncmp( sMethod, "getFrameList", methodLen ) == 0 )
    {
        mexPrintf( sMethod );
    }
    else if( strncmp( sMethod, "getPatternListDict", methodLen ) == 0 )
    {
        mexPrintf( sMethod );
    }
    else if( strncmp( sMethod, "getPulseDict", methodLen ) == 0 )
    {
        mexPrintf( sMethod );
    }
    else if( strncmp( sMethod, "getSourceDict", methodLen ) == 0 )
    {
        mexPrintf( sMethod );
    }
    else if( strncmp( sMethod, "getReceiveChannelMapDict", methodLen ) == 0 )
    {
        mexPrintf( sMethod );
    }
    else if( strncmp( sMethod, "getTransmitApodizationDict", methodLen ) == 0 )
    {
        mexPrintf( sMethod );
    }
    else if( strncmp( sMethod, "getReceiveSettingsDict", methodLen ) == 0 )
    {
        mexPrintf( sMethod );
    }
    else if( strncmp( sMethod, "getAcquisition", methodLen ) == 0 )
    {
        mexPrintf( sMethod );
    }
    else if( strncmp( sMethod, "getTransducer", methodLen ) == 0 )
    {
        mexPrintf( sMethod );
    }
    else if( strncmp( sMethod, "getNumFrames", methodLen ) == 0 )
    {
        mexPrintf( sMethod );
    }
    else if( strncmp( sMethod, "getNumResponses", methodLen ) == 0 )
    {
        mexPrintf( sMethod );
    }
    else if( strncmp( sMethod, "getNumChannels", methodLen ) == 0 )
    {
        mexPrintf( sMethod );
    }
    else if( strncmp( sMethod, "getSamplesPerLine", methodLen ) == 0 )
    {
        mexPrintf( sMethod );
    }
    else if( strncmp( sMethod, "setFrameList", methodLen ) == 0 )
    {
        mexPrintf( sMethod );
    }
    else if( strncmp( sMethod, "setPatternListDict", methodLen ) == 0 )
    {
        mexPrintf( sMethod );
    }
    else if( strncmp( sMethod, "setPulseDict", methodLen ) == 0 )
    {
        mexPrintf( sMethod );
    }
    else if( strncmp( sMethod, "setSourceDict", methodLen ) == 0 )
    {
        mexPrintf( sMethod );
    }
    else if( strncmp( sMethod, "setReceiveChannelMapDict", methodLen ) == 0 )
    {
        mexPrintf( sMethod );
    }
    else if( strncmp( sMethod, "setTransmitApodizationDict", methodLen ) == 0 )
    {
        mexPrintf( sMethod );
    }
    else if( strncmp( sMethod, "setReceiveSettingsDict", methodLen ) == 0 )
    {
        mexPrintf( sMethod );
    }
    else if( strncmp( sMethod, "setAcquisition", methodLen ) == 0 )
    {
        mexPrintf( sMethod );
    }
    else if( strncmp( sMethod, "setTransducer", methodLen ) == 0 )
    {
        mexPrintf( sMethod );
    }
    else if( strncmp( sMethod, "channelSave", methodLen ) == 0 )
    {
        mexPrintf( sMethod );
    }
    else if( strncmp( sMethod, "channelLoad", methodLen ) == 0 )
    {
        mexPrintf( sMethod );
    }
    else if( strncmp( sMethod, "responseSave", methodLen ) == 0 )
    {
        mexPrintf( sMethod );
    }
    else if( strncmp( sMethod, "responseLoad", methodLen ) == 0 )
    {
        mexPrintf( sMethod );
    }
    else if( strncmp( sMethod, "frameSave", methodLen ) == 0 )
    {
        mexPrintf( sMethod );
    }
    else if( strncmp( sMethod, "frameLoad", methodLen ) == 0 )
    {
        mexPrintf( sMethod );
    }
    else 
    {
        mexErrMsgTxt( "mxInputFile: Method not supported." );
    }
    mexPrintf("\n");
}



