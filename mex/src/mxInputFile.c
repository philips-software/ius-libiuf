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

#include "mex.h"
#include <string.h> /*strncmp*/

//-----------------------------------------------------------------------------
// G L O B A L S
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// L O C A L   F U N C T I O N S
//-----------------------------------------------------------------------------

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

    IusInputFile * spInst = NULL;

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
        
        char * sFilename;
        int filenameLen;
        iuif_t iusInputFile;
        if( nrhs < 2 )
        {
            mexErrMsgTxt( "mxInputFile: No filename provided." );
        }
        filenameLen = (mwSize)mxGetN( prhs[ 1 ] ) * sizeof( mxChar ) + 1;
        sFilename = (char*)mxMalloc( filenameLen );
        if( mxGetString( prhs[ 1 ], sFilename, filenameLen ) != 0 )
        {
            mexErrMsgTxt( "mxInputFile: Could not convert method string." );
        }
        iusInputFile = iusInputFileCreate( sFilename );
        mexPrintf( "Filename = " );
        mexPrintf( sFilename );
        mexPrintf( sMethod );
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
        mexPrintf( sMethod );
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



