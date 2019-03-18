/** ===========================================================================
 *    COPYRIGHT 2018 PHILIPS RESEARCH
 *  ===========================================================================
 *
 * \file mxInputFile.c
 * \brief Mex file for reading and writing a version 3 input file type.
 *
 *  ===========================================================================
*/
#include "iuf.h"
#include <iufInputFilePrivate.h>
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
static iuif_t LF_getHandle(const mxArray * prhs)
{
    double handleId;
    iuif_t iufInputFile;
    
    handleId = mxGetScalar(prhs);
    iufInputFile = (iuif_t)mxHandleListGetById(&inputFileList, handleId);
    if (iufInputFile == IUIF_INVALID)
    {
        mexErrMsgIdAndTxt("mxInputFile:close", "Unable to find file, file already closed?");
    }
    return iufInputFile;
}

static void LF_returnDouble(mxArray * plhs[], double returnValue)
{
    double * pDouble;
    plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL);
    pDouble = (double*)mxGetData(plhs[0]);
    pDouble[0] = returnValue;
}

static void LF_returnDict(mxArray * plhs[], iud_t dict, mwIndex numItems)
{
    mwIndex c1;
    mxArray * cell;
    double * pDouble;

    /*iuta_t ta;
    ta = iufTransmitApodizationDictGet(dict, "bmode")*/



    plhs[0] = mxCreateCellMatrix(numItems, 2);
    for (c1 = 0; c1 < numItems; c1++)
    {
        cell = mxCreateDoubleMatrix(1, 1, mxREAL);
        pDouble = mxGetPr(cell);
        pDouble[0] = (int)c1;
        mxSetCell(plhs[0], 2 * c1, cell);

        cell = mxCreateDoubleMatrix(1, 1, mxREAL);
        pDouble = mxGetPr(cell);
        pDouble[0] = (double)(100 + c1);
        mxSetCell(plhs[0], 2 * c1 + 1, cell);
    }
}

static void LF_cleanup(void)
{
    iuif_t iufInputFile;
    while (inputFileList != MXH_INVALID)
    {   
        iufInputFile = (iuif_t)mxHandleListRemoveFirst(&inputFileList);
        
        if (iufInputFile == IUIF_INVALID)
        {
            mexErrMsgIdAndTxt("mxInputFile:mexatexit", "Invalid iufInputFile handle.");
        }

        if (iufInputFileClose(iufInputFile) != IUF_E_OK)
        {
            mexErrMsgIdAndTxt("mxInputFile:mexatexit", "Unable to close file, file already closed?");
        }
        if (iufInputFileDelete(iufInputFile) != IUF_E_OK)
        {
            mexErrMsgIdAndTxt("mxInputFile:mexatexit", "Unable to delete instance.");
        }
    }
    mexPrintf("All resources released!\n");
}

static void LF_Validate_Num_Arguments( char * sMethod, int nrhs, int numArguments )
{
    if (nrhs != numArguments + 1)
    {
        mexErrMsgIdAndTxt("mxInputFile:%s", "Illegal number of arguments", sMethod);
    }
}

static char * LF_getFileName(const mxArray * prhs)
{
    mwSize filenameLen;
    char * filename;

    filenameLen = (mwSize)mxGetN(prhs) * sizeof(mxChar) + 1;
    if (filenameLen == 1)
    {
        mexErrMsgIdAndTxt("mxInputFile:create", "Filename must have non-zero length.");
    }
    filename = (char*)mxMalloc(filenameLen);
    if (filename == NULL)
    {
        mexErrMsgIdAndTxt("mxInputFile:getFileName", "Unable to allocate memory.");
    }
    if (mxGetString(prhs, filename, filenameLen) != 0)
    {
        mexErrMsgIdAndTxt("mxInputFile:create", "Could not convert method string.");
    }
    return filename;
}

static void create(int nlhs, mxArray * plhs[], int nrhs, const mxArray * prhs[])
{
    char * filename;
    double handleId;
    iuif_t iufInputFile;

    filename = LF_getFileName(prhs[1]);

    /* Create new input file */
    iufInputFile = iufInputFileCreate(filename);
    if (iufInputFile == IUIF_INVALID)
    {
        mexErrMsgIdAndTxt("mxInputFile:create", "Unable to create file, file already open?");
    }
    /* Register a mexAtExit function */
    mexAtExit(LF_cleanup);

    /* Add file handle to a handle list */
    handleId = mxHandleListAppend(&inputFileList, iufInputFile );

    /* Return file identifier */
    LF_returnDouble(plhs, handleId);

    mexPrintf("iufInputFile '%s' opened.\n", filename);

    /* Cleanup memory */
    mxFree(filename);
}

static void frameCreate(int nlhs, mxArray * plhs[], int nrhs, const mxArray * prhs[])
{

}

static void responseCreate(int nlhs, mxArray * plhs[], int nrhs, const mxArray * prhs[])
{

}

static void channelCreate(int nlhs, mxArray * plhs[], int nrhs, const mxArray * prhs[])
{

}

static void delete(int nlhs, mxArray * plhs[], int nrhs, const mxArray * prhs[])
{
    double handleId;
    iuif_t iufInputFile;

    handleId = mxGetScalar(prhs[1]);

    iufInputFile = (iuif_t)mxHandleListGetById(&inputFileList, handleId);
    if (iufInputFileDelete(iufInputFile) != IUF_E_OK)
    {
        mexErrMsgIdAndTxt("mxInputFile:close", "Unable to delete instance.");
    }
    iufInputFile = (iuif_t)mxHandleListRemoveById(&inputFileList, handleId);
    mexPrintf("iufInputFile data deleted.\n");
}

static void nodeLoad(int nlhs, mxArray * plhs[], int nrhs, const mxArray * prhs[])
{
    char * filename;
    double handleId;
    iuif_t iufInputFile;

    filename = LF_getFileName(prhs[1]);

    iufInputFile = iufInputFileNodeLoad(filename);
    if (iufInputFile == IUIF_INVALID)
    {
        mexErrMsgIdAndTxt("mxInputFile:nodeload", "Unable to load node");
    }
    /* Register a mexAtExit function */
    mexAtExit(LF_cleanup);

    /* Add file handle to a handle list */
    handleId = mxHandleListAppend(&inputFileList, iufInputFile);

    /* Return file identifier */
    LF_returnDouble(plhs, handleId);

    mexPrintf("iufInputFile '%s' loaded.\n", filename);

    /* Cleanup memory */
    mxFree(filename);
}

static void nodeSave(int nlhs, mxArray * plhs[], int nrhs, const mxArray * prhs[])
{

}

static void close(int nlhs, mxArray * plhs[], int nrhs, const mxArray * prhs[])
{
    iuif_t iufInputFile;
    iufInputFile = LF_getHandle(prhs[1]);
    if (iufInputFileClose(iufInputFile) != IUF_E_OK)
    {
        mexErrMsgIdAndTxt("mxInputFile:close", "Unable to close file, file already closed?");
    }
    mexPrintf("iufInputFile closed.\n");
}

static void compare(int nlhs, mxArray * plhs[], int nrhs, const mxArray * prhs[])
{
    iuif_t reference;
    iuif_t actual;
    int isEqual;

    reference = LF_getHandle(prhs[1]);
    actual = LF_getHandle(prhs[2]);
    isEqual = iufInputFileCompare(reference, actual);
    
    LF_returnDouble(plhs, (double)isEqual);
}

static void getFrameList(int nlhs, mxArray * plhs[], int nrhs, const mxArray * prhs[])
{

}

static void getPatternListDict(int nlhs, mxArray * plhs[], int nrhs, const mxArray * prhs[])
{

}

static void getPulseDict(int nlhs, mxArray * plhs[], int nrhs, const mxArray * prhs[])
{

}

static void getSourceDict(int nlhs, mxArray * plhs[], int nrhs, const mxArray * prhs[])
{

}

static void getReceiveChannelMapDict(int nlhs, mxArray * plhs[], int nrhs, const mxArray * prhs[])
{

}

static void getTransmitApodizationDict(int nlhs, mxArray * plhs[], int nrhs, const mxArray * prhs[])
{

    iuif_t iufInputFile;
    iutad_t tad;
    size_t numItems;

    iufInputFile = LF_getHandle(prhs[1]);
    tad = iufInputFileGetTransmitApodizationDict(iufInputFile);
    numItems = iufTransmitApodizationDictGetSize(tad);
    
    mexPrintf("Number of elements: %d.\n", numItems);

    LF_returnDict(plhs, (iud_t)tad, (mwIndex)numItems);

    
    /*
    double * pDouble;
    plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL);
    pDouble = (double*)mxGetData(plhs[0]);
    pDouble[0] = returnValue;
    */

}

static void getReceiveSettingsDict(int nlhs, mxArray * plhs[], int nrhs, const mxArray * prhs[])
{

}

static void getAcquisition(int nlhs, mxArray * plhs[], int nrhs, const mxArray * prhs[])
{

}

static void getTransducer(int nlhs, mxArray * plhs[], int nrhs, const mxArray * prhs[])
{

}

static void getNumFrames(int nlhs, mxArray * plhs[], int nrhs, const mxArray * prhs[])
{

}

static void getNumResponses(int nlhs, mxArray * plhs[], int nrhs, const mxArray * prhs[])
{

}

static void getNumChannels(int nlhs, mxArray * plhs[], int nrhs, const mxArray * prhs[])
{

}

static void getSamplesPerLine(int nlhs, mxArray * plhs[], int nrhs, const mxArray * prhs[])
{

}

static void setFrameList(int nlhs, mxArray * plhs[], int nrhs, const mxArray * prhs[])
{

}

static void setPatternListDict(int nlhs, mxArray * plhs[], int nrhs, const mxArray * prhs[])
{

}

static void setPulseDict(int nlhs, mxArray * plhs[], int nrhs, const mxArray * prhs[])
{

}

static void setSourceDict(int nlhs, mxArray * plhs[], int nrhs, const mxArray * prhs[])
{

}

static void setReceiveChannelMapDict(int nlhs, mxArray * plhs[], int nrhs, const mxArray * prhs[])
{

}

static void setTransmitApodizationDict(int nlhs, mxArray * plhs[], int nrhs, const mxArray * prhs[])
{

}

static void setReceiveSettingsDict(int nlhs, mxArray * plhs[], int nrhs, const mxArray * prhs[])
{

}

static void setAcquisition(int nlhs, mxArray * plhs[], int nrhs, const mxArray * prhs[])
{

}

static void setTransducer(int nlhs, mxArray * plhs[], int nrhs, const mxArray * prhs[])
{

}

static void channelSave(int nlhs, mxArray * plhs[], int nrhs, const mxArray * prhs[])
{

}

static void channelLoad(int nlhs, mxArray * plhs[], int nrhs, const mxArray * prhs[])
{

}

static void responseSave(int nlhs, mxArray * plhs[], int nrhs, const mxArray * prhs[])
{

}

static void responseLoad(int nlhs, mxArray * plhs[], int nrhs, const mxArray * prhs[])
{

}

static void frameSave(int nlhs, mxArray * plhs[], int nrhs, const mxArray * prhs[])
{

}

static void frameLoad(int nlhs, mxArray * plhs[], int nrhs, const mxArray * prhs[])
{
    
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
        LF_Validate_Num_Arguments(sMethod, nrhs, 1);
        create(nlhs, plhs, nrhs, prhs);
    }
    else if( strncmp( sMethod, "frameCreate", methodLen ) == 0 )
    {
        LF_Validate_Num_Arguments(sMethod, nrhs, 1);
        frameCreate(nlhs, plhs, nrhs, prhs);
    }
    else if( strncmp( sMethod, "responseCreate", methodLen ) == 0 )
    {
        LF_Validate_Num_Arguments(sMethod, nrhs, 2);
        responseCreate(nlhs, plhs, nrhs, prhs);
    }
    else if( strncmp( sMethod, "channelCreate", methodLen ) == 0 )
    {
        LF_Validate_Num_Arguments(sMethod, nrhs, 2);
        channelCreate(nlhs, plhs, nrhs, prhs);
    }
    else if( strncmp( sMethod, "delete", methodLen ) == 0 )
    {
        LF_Validate_Num_Arguments(sMethod, nrhs, 1);
        delete(nlhs, plhs, nrhs, prhs);
    }
    else if( strncmp( sMethod, "nodeLoad", methodLen ) == 0 )
    {
        LF_Validate_Num_Arguments(sMethod, nrhs, 1);
        nodeLoad(nlhs, plhs, nrhs, prhs);
    }
    else if( strncmp( sMethod, "nodeSave", methodLen ) == 0 )
    {
        LF_Validate_Num_Arguments(sMethod, nrhs, 1);
        nodeSave(nlhs, plhs, nrhs, prhs);
    }
    else if( strncmp( sMethod, "close", methodLen ) == 0 )
    {
        LF_Validate_Num_Arguments(sMethod, nrhs, 1);
        close(nlhs, plhs, nrhs, prhs);
    }
    else if( strncmp( sMethod, "compare", methodLen ) == 0 )
    {
        LF_Validate_Num_Arguments(sMethod, nrhs, 2);
        compare(nlhs, plhs, nrhs, prhs);
    }
    else if( strncmp( sMethod, "getFrameList", methodLen ) == 0 )
    {
        LF_Validate_Num_Arguments(sMethod, nrhs, 1);
        getFrameList(nlhs, plhs, nrhs, prhs);
    }
    else if( strncmp( sMethod, "getPatternListDict", methodLen ) == 0 )
    {
        LF_Validate_Num_Arguments(sMethod, nrhs, 1);
        getPatternListDict(nlhs, plhs, nrhs, prhs);
    }
    else if( strncmp( sMethod, "getPulseDict", methodLen ) == 0 )
    {
        LF_Validate_Num_Arguments(sMethod, nrhs, 1);
        getPulseDict(nlhs, plhs, nrhs, prhs);
    }
    else if( strncmp( sMethod, "getSourceDict", methodLen ) == 0 )
    {
        LF_Validate_Num_Arguments(sMethod, nrhs, 1);
        getSourceDict(nlhs, plhs, nrhs, prhs);
    }
    else if( strncmp( sMethod, "getReceiveChannelMapDict", methodLen ) == 0 )
    {
        LF_Validate_Num_Arguments(sMethod, nrhs, 1);
        getReceiveChannelMapDict(nlhs, plhs, nrhs, prhs);
    }
    else if( strncmp( sMethod, "getTransmitApodizationDict", methodLen ) == 0 )
    {
        LF_Validate_Num_Arguments(sMethod, nrhs, 1);
        getTransmitApodizationDict(nlhs, plhs, nrhs, prhs);
    }
    else if( strncmp( sMethod, "getReceiveSettingsDict", methodLen ) == 0 )
    {
        LF_Validate_Num_Arguments(sMethod, nrhs, 1);
        getReceiveSettingsDict(nlhs, plhs, nrhs, prhs);
    }
    else if( strncmp( sMethod, "getAcquisition", methodLen ) == 0 )
    {
        LF_Validate_Num_Arguments(sMethod, nrhs, 1);
        getAcquisition(nlhs, plhs, nrhs, prhs);
    }
    else if( strncmp( sMethod, "getTransducer", methodLen ) == 0 )
    {
        LF_Validate_Num_Arguments(sMethod, nrhs, 1);
        getTransducer(nlhs, plhs, nrhs, prhs);
    }
    else if( strncmp( sMethod, "getNumFrames", methodLen ) == 0 )
    {
        LF_Validate_Num_Arguments(sMethod, nrhs, 1);
        getNumFrames(nlhs, plhs, nrhs, prhs);
    }
    else if( strncmp( sMethod, "getNumResponses", methodLen ) == 0 )
    {
        LF_Validate_Num_Arguments(sMethod, nrhs, 2);
        getNumResponses(nlhs, plhs, nrhs, prhs);
    }
    else if( strncmp( sMethod, "getNumChannels", methodLen ) == 0 )
    {
        LF_Validate_Num_Arguments(sMethod, nrhs, 2);
        getNumChannels(nlhs, plhs, nrhs, prhs);
    }
    else if( strncmp( sMethod, "getSamplesPerLine", methodLen ) == 0 )
    {
        LF_Validate_Num_Arguments(sMethod, nrhs, 2);
        getSamplesPerLine(nlhs, plhs, nrhs, prhs);
    }
    else if( strncmp( sMethod, "setFrameList", methodLen ) == 0 )
    {
        LF_Validate_Num_Arguments(sMethod, nrhs, 2);
        setFrameList(nlhs, plhs, nrhs, prhs);
    }
    else if( strncmp( sMethod, "setPatternListDict", methodLen ) == 0 )
    {
        LF_Validate_Num_Arguments(sMethod, nrhs, 2);
        setPatternListDict(nlhs, plhs, nrhs, prhs);
    }
    else if( strncmp( sMethod, "setPulseDict", methodLen ) == 0 )
    {
        LF_Validate_Num_Arguments(sMethod, nrhs, 2);
        setPulseDict(nlhs, plhs, nrhs, prhs);
    }
    else if( strncmp( sMethod, "setSourceDict", methodLen ) == 0 )
    {
        LF_Validate_Num_Arguments(sMethod, nrhs, 2);
        setSourceDict(nlhs, plhs, nrhs, prhs);
    }
    else if( strncmp( sMethod, "setReceiveChannelMapDict", methodLen ) == 0 )
    {
        LF_Validate_Num_Arguments(sMethod, nrhs, 2);
        setReceiveChannelMapDict(nlhs, plhs, nrhs, prhs);
    }
    else if( strncmp( sMethod, "setTransmitApodizationDict", methodLen ) == 0 )
    {
        LF_Validate_Num_Arguments(sMethod, nrhs, 2);
        setTransmitApodizationDict(nlhs, plhs, nrhs, prhs);
    }
    else if( strncmp( sMethod, "setReceiveSettingsDict", methodLen ) == 0 )
    {
        LF_Validate_Num_Arguments(sMethod, nrhs, 2);
        setReceiveSettingsDict(nlhs, plhs, nrhs, prhs);
    }
    else if( strncmp( sMethod, "setAcquisition", methodLen ) == 0 )
    {
        LF_Validate_Num_Arguments(sMethod, nrhs, 2);
        setAcquisition(nlhs, plhs, nrhs, prhs);
    }
    else if( strncmp( sMethod, "setTransducer", methodLen ) == 0 )
    {
        LF_Validate_Num_Arguments(sMethod, nrhs, 2);
        setTransducer(nlhs, plhs, nrhs, prhs);
    }
    else if( strncmp( sMethod, "channelSave", methodLen ) == 0 )
    {
        LF_Validate_Num_Arguments(sMethod, nrhs, 4);
        channelSave(nlhs, plhs, nrhs, prhs);
    }
    else if( strncmp( sMethod, "channelLoad", methodLen ) == 0 )
    {
        LF_Validate_Num_Arguments(sMethod, nrhs, 4);
        channelLoad(nlhs, plhs, nrhs, prhs);
    }
    else if( strncmp( sMethod, "responseSave", methodLen ) == 0 )
    {
        LF_Validate_Num_Arguments(sMethod, nrhs, 4);
        responseSave(nlhs, plhs, nrhs, prhs);
    }
    else if( strncmp( sMethod, "responseLoad", methodLen ) == 0 )
    {
        LF_Validate_Num_Arguments(sMethod, nrhs, 4);
        responseLoad(nlhs, plhs, nrhs, prhs);
    }
    else if( strncmp( sMethod, "frameSave", methodLen ) == 0 )
    {
        LF_Validate_Num_Arguments(sMethod, nrhs, 4);
        frameSave(nlhs, plhs, nrhs, prhs);
    }
    else if( strncmp( sMethod, "frameLoad", methodLen ) == 0 )
    {
        LF_Validate_Num_Arguments(sMethod, nrhs, 4);
        frameLoad(nlhs, plhs, nrhs, prhs);
    }
    else 
    {
        mexErrMsgTxt( "mxInputFile: Method not supported." );
    }
    mexPrintf("\n");
}



