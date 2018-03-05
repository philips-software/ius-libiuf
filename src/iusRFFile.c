//=============================================================================
//    COPYRIGHT 2015 PHILIPS RESEARCH
//=============================================================================
//
//  Source Name   : iusRFFile.c
//                  RF data file.
//  iusVersion    : 255.255.255.255
//
//=============================================================================

#include "iusRFFile.h"
#include "iusVector.h"
#include "iusMatrix.h"
#include <hdf5_hl.h>          // highlevel hdf functions
#include <stdlib.h>           // for malloc, free
#include <math.h>             // fabs ..
#include <string.h>           // strcmp
#include <assert.h>           //

//=============================================================================
// L O C A L   D E F I N E S   A N D   V A R I A B L E S
//=============================================================================
static int LF_readVector
(
    IUSRFFileInstance * pInst,
    const char * const varpath,
    const int N,
    float ** pOut
);

static int LF_readCellVector
(
    IUSRFFileInstance * pInst,
    const char * const group,
    const char * const var,
    const int N,
    float ** pOut
);

static int LF_readCellMatrix
(
    IUSRFFileInstance * pInst,
    const char * const group,
    const char * const var,
    const int N,
    const int M,
    float *** pOut
);

//=============================================================================
// S T A T I C   F U N C T I O N   P R O T O T Y P E S
//=============================================================================

//=============================================================================
// E X P O R T E D   F U N C T I O N S
//=============================================================================

//-----------------------------------------------------------------------------
// Creates a RFFile instance with the specified parameters.
//-----------------------------------------------------------------------------
IUSRFFileInstance * iusRFFileOpen
(
    const char * const pFullFileName,
    const int verbose
)
{
    IUSRFFileInstance * pInst;
    hsize_t dims[ 5 ]; // assuming dim of M is 4; +1 for checking
    herr_t  status;
    hid_t   plist_id;
    float * pData;
    int     len;
    int     numfilt;
    int     rv;
    int     c1, c2;
    char    s1[ 128 ];

    /*------------------------------------------------------------------------*/
    /* alloc instance ; using calloc to clear all state.                      */
    /*------------------------------------------------------------------------*/
    pInst = (IUSRFFileInstance*)calloc( 1, sizeof( *pInst ) );
    if( pInst == NULL )
    {
        fprintf( stderr, "iusRFFileOpen: calloc failed\n" );
        return NULL;
    }

    /*------------------------------------------------------------------------*/
    /* init instance variables                                                */
    /*------------------------------------------------------------------------*/
    pInst->waveidx  = 0;
    pInst->frameidx = 0;

    /*------------------------------------------------------------------------*/
    /* open hdf5 file                                                         */
    /*------------------------------------------------------------------------*/
    pInst->file_id = H5Fopen( pFullFileName, H5F_ACC_RDONLY, H5P_DEFAULT );
    if( pInst->file_id < 0 )
    {
        fprintf( stderr, "iusRFFileOpen: H5Fopen failed\n" );
        goto bail;
    }

    /*------------------------------------------------------------------------*/
    /* get RF data dimensions                                                 */
    /*------------------------------------------------------------------------*/
    dims[ 3 ] = 0;
    dims[ 4 ] = 0;
    status = H5LTget_dataset_info( pInst->file_id, "/M", dims, NULL, NULL );
    if( status < 0 )
    {
        fprintf( stderr, "iusRFFileOpen: H5LTget_dataset_info /M failed\n" );
        goto bail;
    }
    if( dims[ 4 ] != 0 )
    {
        fprintf( stderr, "iusRFFileOpen: bad dimensions /M\n" );
        goto bail;
    }
    if( dims[ 3 ] == 0 ) /* nrFrames == 1 ..? */
    {
        pInst->lineLength   = (int) dims[ 2 ];
        pInst->nrRxElements = (int) dims[ 1 ];
        pInst->nrTxEvents   = (int) dims[ 0 ];
        pInst->nrFrames     = (int)1;
    }
    else
    {
        pInst->lineLength   = (int) dims[ 3 ];
        pInst->nrRxElements = (int) dims[ 2 ];
        pInst->nrTxEvents   = (int) dims[ 1 ];
        pInst->nrFrames     = (int) dims[ 0 ];
    }

    if( verbose ) fprintf( stdout, "rffile: lineLength %i\n", pInst->lineLength );
    if( verbose ) fprintf( stdout, "rffile: nrRxElements %i\n", pInst->nrRxElements );
    if( verbose ) fprintf( stdout, "rffile: nrTxEvents %i\n", pInst->nrTxEvents );
    if( verbose ) fprintf( stdout, "rffile: nrFrames %i\n", pInst->nrFrames );

    /*------------------------------------------------------------------------*/
    /* get txd freq                                                           */
    /*------------------------------------------------------------------------*/
    status = H5LTread_dataset_float( pInst->file_id, "/Trans/frequency", &(pInst->Trans.frequency) );
    if( status < 0 )
    {
        fprintf( stderr, "iusRFFileOpen: H5LTread_dataset_float /Trans/frequency failed\n" );
        goto bail;
    }
    if( verbose ) fprintf( stdout, "rffile: Trans.frequency %f\n", pInst->Trans.frequency );

    /*------------------------------------------------------------------------*/
    /* get numelements                                                        */
    /*------------------------------------------------------------------------*/
    status = H5LTread_dataset_int( pInst->file_id, "/Trans/numelements", &(pInst->Trans.numelements) );
    if( status < 0 )
    {
        fprintf( stderr, "iusRFFileOpen: H5LTread_dataset_int /Trans/numelements failed\n" );
        goto bail;
    }
    if( verbose ) fprintf( stdout, "rffile: Trans.numelements %i\n", pInst->Trans.numelements );

    /*------------------------------------------------------------------------*/
    /* get Fs                                                                 */
    /*------------------------------------------------------------------------*/
    status = H5LTread_dataset_float( pInst->file_id, "/par/Fs", &(pInst->par.Fs) );
    if( status < 0 )
    {
        fprintf( stderr, "iusRFFileOpen: H5LTread_dataset_float /par/Fs failed\n" );
        goto bail;
    }
    if( verbose ) fprintf( stdout, "rffile: par.Fs %f\n", pInst->par.Fs );

    /*------------------------------------------------------------------------*/
    /* get c                                                                  */
    /*------------------------------------------------------------------------*/
    status = H5LTread_dataset_float( pInst->file_id, "/par/speedOfSound", &(pInst->par.speedOfSound) );
    if( status < 0 )
    {
        fprintf( stderr, "iusRFFileOpen: H5LTread_dataset_float /par/speedOfSound failed\n" );
        goto bail;
    }
    if( verbose ) fprintf( stdout, "rffile: par.speedOfSound %f\n", pInst->par.speedOfSound );

    /*------------------------------------------------------------------------*/
    /* get waveRate                                                           */
    /*------------------------------------------------------------------------*/
    status = H5LTread_dataset_float( pInst->file_id, "/par/waveRate", &(pInst->par.waveRate) );
    if( status < 0 )
    {
        fprintf( stderr, "iusRFFileOpen: H5LTread_dataset_float /par/waveRate failed\n" );
        goto bail;
    }
    if( verbose ) fprintf( stdout, "rffile: par.waveRate %f\n", pInst->par.waveRate );

    /*------------------------------------------------------------------------*/
    /* get nrAcquisitionsPerDMA (since Oct/06/2015)                           */
    /*------------------------------------------------------------------------*/
    status = H5LTread_dataset_int( pInst->file_id, "/par/nrAcquisitionsPerDMA", &(pInst->par.nrFramesPerDMA) );
    if( status < 0 )
    {
        if (verbose) fprintf( stdout, "iusRFFileOpen: H5LTread_dataset_int /par/nrAcquisitionsPerDMA failed trying nrBModeFramesPerDMA\n" );

        /*--------------------------------------------------------------------*/
        /* get nrBModeFramesPerDMA                                            */
        /*--------------------------------------------------------------------*/
        status = H5LTread_dataset_int( pInst->file_id, "/par/nrBModeFramesPerDMA", &(pInst->par.nrFramesPerDMA) );
        if( status < 0 )
        {
            fprintf( stderr, "iusRFFileOpen: H5LTread_dataset_int /par/nrBModeFramesPerDMA failed\n" );
            goto bail;
        }
    }
    if( verbose ) fprintf( stdout, "rffile: par.nrFramesPerDMA %i\n", pInst->par.nrFramesPerDMA );

    /*------------------------------------------------------------------------*/
    /* get ensembleSize (since Oct/06/2015)                                   */
    /*------------------------------------------------------------------------*/
    status = H5LTread_dataset_int( pInst->file_id, "/par/ensembleSize", &(pInst->par.ensembleSize) );
    if( status < 0 )
    {
        fprintf( stderr, "iusRFFileOpen: H5LTread_dataset_int /par/ensembleSize failed\n" );
        pInst->par.ensembleSize = 1; /* use default */
    }
    if( verbose ) fprintf( stdout, "rffile: par.ensembleSize %i\n", pInst->par.ensembleSize );

    /*------------------------------------------------------------------------*/
    /* get samplesPerWave                                                     */
    /*------------------------------------------------------------------------*/
    status = H5LTread_dataset_int( pInst->file_id, "/par/samplesPerWave", &(pInst->par.samplesPerWave) );
    if( status < 0 )
    {
        fprintf( stderr, "iusRFFileOpen: H5LTread_dataset_int /par/samplesPerWave failed\n" );
        goto bail;
    }
    if( verbose ) fprintf( stdout, "rffile: par.samplesPerWave %i\n", pInst->par.samplesPerWave );

    /*------------------------------------------------------------------------*/
    /* get txFNumber                                                          */
    /*------------------------------------------------------------------------*/
    status = H5LTread_dataset_float( pInst->file_id, "/par/txFNumber", &(pInst->par.txFNumber) );
    if( status < 0 )
    {
        fprintf( stderr, "iusRFFileOpen: H5LTread_dataset_float /par/txFNumber failed\n" );
        goto bail;
    }
    if( verbose ) fprintf( stdout, "rffile: par.txFNumber %f\n", pInst->par.txFNumber);

    /*------------------------------------------------------------------------*/
    /* get tPCHighVoltage                                                     */
    /*------------------------------------------------------------------------*/
    status = H5LTread_dataset_float( pInst->file_id, "/par/tPCHighVoltage", &(pInst->par.tPCHighVoltage) );
    if( status < 0 )
    {
        fprintf( stderr, "iusRFFileOpen: H5LTread_dataset_float /par/tPCHighVoltage failed\n" );
        goto bail;
    }
    if( verbose ) fprintf( stdout, "rffile: par.tPCHighVoltage %f\n", pInst->par.tPCHighVoltage );

    /*------------------------------------------------------------------------*/
    /* get pulseFreqMHz                                                       */
    /*------------------------------------------------------------------------*/
    status = H5LTread_dataset_float( pInst->file_id, "/par/pulseFreqMHz", &(pInst->par.pulseFreqMHz) );
    if( status < 0 )
    {
        if (verbose) fprintf( stdout, "iusRFFileOpen: /par/pulseFreqMHz not found (old file?), using /Trans/frequency instead \n" );
        pInst->par.pulseFreqMHz = pInst->Trans.frequency;
    }
    if( verbose ) fprintf( stdout, "rffile: par.pulseFreqMHz %f\n", pInst->par.pulseFreqMHz );

    /*------------------------------------------------------------------------*/
    /* Get nrPulseCycles                                                      */
    /*------------------------------------------------------------------------*/
    status = H5LTread_dataset_int(pInst->file_id, "/par/nrPulseCycles", &(pInst->par.nrPulseCycles));
    if (status < 0)
    {
        if (verbose) fprintf(stdout, "iusRFFileOpen: H5LTread_dataset_int /par/nrPulseCycles failed, assuming 1\n");
        pInst->par.nrPulseCycles=1;
    }
    if (verbose) fprintf(stdout, "rffile: par.nrPulseCycles %i\n", pInst->par.nrPulseCycles);

    /*------------------------------------------------------------------------*/
    /* get theta                                                              */
    /*------------------------------------------------------------------------*/
    rv = LF_readVector( pInst, "/par/theta", pInst->nrTxEvents, &(pInst->par.theta) );
    if( rv < 0 )
    {
        fprintf( stderr, "iusRFFileOpen: LF_readVector /par/theta failed\n" );
        goto bail;
    }
    if( verbose )
    {
        fprintf( stdout, "rffile: par.theta(deg)\n");
        for( c1 = 0; c1 < pInst->nrTxEvents; c1++ )
            fprintf( stdout, "%+4.1f ", pInst->par.theta[ c1 ] * 180.0f / 3.14159265f );
        fprintf( stdout, "\n" );
    }

    /*------------------------------------------------------------------------*/
    /* get TW.peak                                                            */
    /*------------------------------------------------------------------------*/
    status = H5LTread_dataset_float( pInst->file_id, "/TW/peak", &(pInst->TW.peak) );
    if( status < 0 )
    {
        fprintf( stderr, "iusRFFileOpen: H5LTread_dataset_float /TW/peak failed\n" );
        goto bail;
    }
    if( verbose ) fprintf( stdout, "rffile: TW.peak %f\n", pInst->TW.peak );

    /*------------------------------------------------------------------------*/
    /* get TW.samplesPerWL                                                    */
    /*------------------------------------------------------------------------*/
    status = H5LTread_dataset_float( pInst->file_id, "/TW/samplesPerWL", &(pInst->TW.samplesPerWL) );
    if( status < 0 )
    {
        fprintf( stderr, "iusRFFileOpen: H5LTread_dataset_float /TW/samplesPerWL failed\n" );
        goto bail;
    }
    if( verbose ) fprintf( stdout, "rffile: TW.samplesPerWL %f\n", pInst->TW.samplesPerWL );

    /*------------------------------------------------------------------------*/
    /* get TW.numsamples                                                      */
    /*------------------------------------------------------------------------*/
    status = H5LTread_dataset_int( pInst->file_id, "/TW/numsamples", &(pInst->TW.numsamples) );
    if( status < 0 )
    {
        fprintf( stderr, "iusRFFileOpen: H5LTread_dataset_int /TW/numsamples failed\n" );
        goto bail;
    }
    if( verbose ) fprintf( stdout, "rffile: TW.numsamples %i\n", pInst->TW.numsamples );

    /*------------------------------------------------------------------------*/
    /* get TX{}.focus                                                         */
    /*------------------------------------------------------------------------*/
    if( pInst->nrTxEvents == 1 )
    {
        pInst->TX.focus = (float*)malloc( sizeof( float ) ); /* kludge */
        status = H5LTread_dataset_float( pInst->file_id, "/TX/focus", &(pInst->TX.focus[ 0 ]) );
        if( status < 0 )
        {
            fprintf( stderr, "iusRFFileOpen: H5LTread_dataset_float /TX/focus failed\n" );
            goto bail;
        }
    }
    else
    {
        rv = LF_readCellVector( pInst, "TX", "focus", pInst->nrTxEvents, &(pInst->TX.focus) );
        if( rv < 0 )
        {
            fprintf( stderr, "iusRFFileOpen: LF_readCellVector /TX/focus failed\n" );
            goto bail;
        }
    }
    if( verbose )
    {
        fprintf( stdout, "rffile: TX.focus(-)\n");
        for( c1 = 0; c1 < pInst->nrTxEvents; c1++ )
            fprintf( stdout, "%5.2f ", - pInst->TX.focus[ c1 ] );
        fprintf( stdout, "\n" );
    }

    /*------------------------------------------------------------------------*/
    /* get TX{}.delayOffsetSec                                                */
    /*------------------------------------------------------------------------*/
    if( pInst->nrTxEvents == 1 )
    {
        pInst->TX.delayOffsetSec = (float*)malloc( sizeof( float ) ); /* kludge */
        status = H5LTread_dataset_float( pInst->file_id, "/TX/delayOffsetSec", &(pInst->TX.delayOffsetSec[ 0 ]) );
        if( status < 0 )
        {
            fprintf( stderr, "iusRFFileOpen: H5LTread_dataset_float /TX/delayOffsetSec failed\n" );
            goto bail;
        }
    }
    else
    {
        rv = LF_readCellVector( pInst, "TX", "delayOffsetSec", pInst->nrTxEvents, &(pInst->TX.delayOffsetSec) );
        if( rv < 0 )
        {
            fprintf( stderr, "iusRFFileOpen: LF_readCellVector /TX/delayOffsetSec failed\n" );
            goto bail;
        }
    }
    if( verbose )
    {
        fprintf( stdout, "rffile: TX.delayOffsetSec(usec)\n");
        for( c1 = 0; c1 < pInst->nrTxEvents; c1++ )
            fprintf( stdout, "%5.2f ", 1e6f * pInst->TX.delayOffsetSec[ c1 ] );
        fprintf( stdout, "\n" );
    }

    /*------------------------------------------------------------------------*/
    /* get TX{}.Delay                                                         */
    /*------------------------------------------------------------------------*/
    if( pInst->nrTxEvents == 1 )
    {
        pInst->TX.Delay = (float**)malloc( sizeof( float* ) ); /* kludge */
        rv = LF_readVector( pInst, "/TX/Delay", pInst->Trans.numelements, &(pInst->TX.Delay[ 0 ]) );
        if( rv < 0 )
        {
            fprintf( stderr, "iusRFFileOpen: LF_readVector /TX/Delay failed\n" );
            goto bail;
        }
    }
    else
    {
        rv = LF_readCellMatrix( pInst, "TX", "Delay", pInst->nrTxEvents, pInst->Trans.numelements, &(pInst->TX.Delay) );
        if( rv < 0 )
        {
            fprintf( stderr, "iusRFFileOpen: LF_readCellMatrix /TX/Delay failed\n" );
            goto bail;
        }
    }
    if( verbose )
    {
        fprintf( stdout, "rffile: TX.Delay\n");
        for( c2 = 0; c2 < pInst->Trans.numelements; c2++ )
        {
            for( c1 = 0; c1 < pInst->nrTxEvents; c1++ )
            {
                fprintf( stdout, "%5.2f ", ( iusMatrixRow( pInst->TX.Delay, c1 ) )[ c2 ] );
            }
            fprintf( stdout, "\n" );
        }
    }

    /*------------------------------------------------------------------------*/
    /* get lensCorrection                                                     */
    /*------------------------------------------------------------------------*/
    status = H5LTread_dataset_float( pInst->file_id, "/Trans/lensCorrection", &(pInst->Trans.lensCorrection) );
    if( status < 0 )
    {
        fprintf( stderr, "iusRFFileOpen: H5LTread_dataset_float /Trans/lensCorrection failed\n" );
        goto bail;
    }
    if( verbose ) fprintf( stdout, "rffile: Trans.lensCorrection %f\n", pInst->Trans.lensCorrection );

    /*------------------------------------------------------------------------*/
    /* get units                                                              */
    /*------------------------------------------------------------------------*/
    status = H5LTread_dataset_string( pInst->file_id, "/Trans/units", s1 );
    if( status < 0 )
    {
        fprintf( stderr, "iusRFFileOpen: H5LTread_dataset_string /Trans/units failed\n" );
        goto bail;
    }
    for( c1 = 0; c1 < (int)sizeof( pInst->Trans.units ) - 1; c1++ )
        pInst->Trans.units[ c1 ] = s1[ c1 << 1 ]; //unicode..?
    if( strncmp( pInst->Trans.units, "mm", 2 ) == 0 ) pInst->Trans.units[ 2 ] = 0;
    if( verbose ) fprintf( stdout, "rffile: Trans.units %s\n", pInst->Trans.units );

    /*------------------------------------------------------------------------*/
    /* get elementpos                                                         */
    /*------------------------------------------------------------------------*/
    status = H5LTget_dataset_info( pInst->file_id, "/Trans/ElementPos", dims, NULL, NULL );
    if( status < 0 )
    {
        fprintf( stderr, "iusRFFileOpen: H5LTget_dataset_info /Trans/ElementPos failed\n" );
        goto bail;
    }
    if( dims[ 0 ] != 4 || (int)dims[ 1 ] != pInst->Trans.numelements )
    {
        fprintf( stderr, "iusRFFileOpen: bad dimensions /Trans/ElementPos\n" );
        goto bail;
    }
    len = (int)( dims[ 0 ] * dims[ 1 ] );
    pData = iusAllocFloatVector( len );
    if( pData == NULL )
    {
        fprintf( stderr, "iusRFFileOpen: iusAllocFloatVector failed\n" );
        goto bail;
    }
    status = H5LTread_dataset_float( pInst->file_id, "/Trans/ElementPos", pData );
    if( status < 0 )
    {
        fprintf( stderr, "iusRFFileOpen: H5LTread_dataset_float /Trans/ElementPos failed\n" );
        goto bail;
    }
    pInst->Trans.ElementPos = iusAllocFloatMatrix( 4, pInst->Trans.numelements );
    if( pInst->Trans.ElementPos == NULL )
    {
        fprintf( stderr, "iusRFFileOpen: iusAllocFloatMatrix failed\n" );
        goto bail;
    }
    for( c1 = 0; c1 < 4; c1++ )
    {
        iusCopyFloatVector( iusMatrixRow( pInst->Trans.ElementPos, c1 ),
            pData + c1 * pInst->Trans.numelements,
            pInst->Trans.numelements );
    }
    iusFreeFloatVector( pData );

    if( verbose )
    {
        fprintf( stdout, "rffile: Trans.ElementPos\n");
        for( c2 = 0; c2 < pInst->Trans.numelements; c2++ )
        {
            for( c1 = 0; c1 < 4; c1++ )
            {
                fprintf( stdout, "%+8.7f ", ( iusMatrixRow( pInst->Trans.ElementPos, c1 ) )[ c2 ] );
            }
            fprintf( stdout, "\n" );
        }
    }

    /*------------------------------------------------------------------------*/
    /* calc Fc ; convert MHz to Hz                                            */
    /*------------------------------------------------------------------------*/
    pInst->Fc = pInst->Trans.frequency * 1e6f;
    if( verbose ) fprintf( stdout, "rffile: Fc %f\n", pInst->Fc );

    fprintf(stdout, "Fc\n");

    /*------------------------------------------------------------------------*/
    /* calc lambda                                                            */
    /*------------------------------------------------------------------------*/
    pInst->lambda = pInst->par.speedOfSound / pInst->Fc;
    if( verbose ) fprintf( stdout, "rffile: lambda %f\n", pInst->lambda );

    fprintf(stdout, "opening M\n");
    /*------------------------------------------------------------------------*/
    /* open dataset M                                                         */
    /*------------------------------------------------------------------------*/
    pInst->M_id = H5Dopen2( pInst->file_id, "/M", H5P_DEFAULT );
    if( pInst->M_id < 0 )
    {
        fprintf( stderr, "iusRFFileOpen: H5Dopen2 /M failed\n" );
        goto bail;
    }

    fprintf(stdout, "creating plist \n");
    /*------------------------------------------------------------------------*/
    /* Retrieve filter information.                                           */
    /*------------------------------------------------------------------------*/
    plist_id = H5Dget_create_plist( pInst->M_id );
    if( pInst->M_id < 0 )
    {
        fprintf( stderr, "iusRFFileOpen: H5Dget_create_plist failed\n" );
        goto bail;
    }
    
    fprintf(stderr, "getting nfilters \n");
    numfilt = H5Pget_nfilters( plist_id );
    for( c1 = 0; c1 < numfilt; c1++ )
    {
        size_t   nelmts;
        unsigned flags;
        unsigned filter_info;
        H5Z_filter_t filter_type;

        nelmts = 0;
        filter_type = H5Pget_filter2( plist_id, c1, &flags, &nelmts, NULL, (size_t)0, NULL, &filter_info );

        switch( filter_type )
        {
            case H5Z_FILTER_DEFLATE:
            case H5Z_FILTER_SZIP:
            case H5Z_FILTER_NBIT:
            case H5Z_FILTER_SCALEOFFSET:
                if( verbose ) fprintf( stdout, "iusRFFileOpen: warning: compressed /M data (reading might be slow)\n" );
                break;
            default:
                break;
         }
    }
    
    fprintf(stderr, "closing plist \n");

    status = H5Pclose( plist_id );
    if( status < 0 )
    {
        fprintf( stderr, "iusRFFileOpen: H5Pclose failed\n" );
        goto bail;
    }

    fprintf(stderr, "done \n");
    /*------------------------------------------------------------------------*/
    /* done ; return instance                                                 */
    /*------------------------------------------------------------------------*/
    return pInst;

bail:
    /*------------------------------------------------------------------------*/
    /* error occurred ; return NULL                                           */
    /*------------------------------------------------------------------------*/
    // todo: free all other mem and handles correctly
    free( pInst );
    return NULL;
}

//-----------------------------------------------------------------------------
// GetElementSpacing.
//-----------------------------------------------------------------------------
int iusRFFileGetElementSpacing
(
    IUSRFFileInstance * pInst,
    float *             pTdxSpacing
)
{
    const float * x = iusMatrixRow( pInst->Trans.ElementPos, 0 );
    const float * y = iusMatrixRow( pInst->Trans.ElementPos, 1 );
    const float * z = iusMatrixRow( pInst->Trans.ElementPos, 2 );
    const float * a = iusMatrixRow( pInst->Trans.ElementPos, 3 );
    float tx;
    int c1;

    /*------------------------------------------------------------------------*/
    /* calc spacing based on first two tdx x positions                        */
    /*------------------------------------------------------------------------*/
    tx = (float)fabs( x[ 1 ] - x[ 0 ] );

    /*------------------------------------------------------------------------*/
    /* consistency check : uniform array                                      */
    /*------------------------------------------------------------------------*/
    for( c1 = 1; c1 < pInst->nrRxElements; c1++ ) // tbd: Trans.numelements
    {
        if( fabs( ( x[ c1 ] - x[ c1 - 1 ] ) - tx ) > 1e-4f )
        {
            assert( 0 ); // should be rare ; crash on this
            return -1;
        }
    }

    /*------------------------------------------------------------------------*/
    /* consistency check : 1D array                                           */
    /*------------------------------------------------------------------------*/
    for( c1 = 0; c1 < pInst->nrRxElements; c1++ ) // tbd: Trans.numelements
    {
        if( y[ c1 ] != 0.0f || z[ c1 ] != 0.0f || a[ c1 ] != 0.0f )
        {
            assert( 0 ); // should be rare ; crash on this
            return -1;
        }
    }

    /*------------------------------------------------------------------------*/
    /* convert mm to m.                                                       */
    /*------------------------------------------------------------------------*/
    *pTdxSpacing = tx * 1e-3f;

    /*------------------------------------------------------------------------*/
    /* done                                                                   */
    /*------------------------------------------------------------------------*/
    return 0;
}

//-----------------------------------------------------------------------------
// GetArrayLength.
//-----------------------------------------------------------------------------
int iusRFFileGetArrayLength
(
    IUSRFFileInstance * pInst,
    float *             pLength
)
{
    const float * x = iusMatrixRow( pInst->Trans.ElementPos, 0 );
    const float * y = iusMatrixRow( pInst->Trans.ElementPos, 1 );
    const float * z = iusMatrixRow( pInst->Trans.ElementPos, 2 );
    const float * a = iusMatrixRow( pInst->Trans.ElementPos, 3 );
    float D;
    int c1;

    /*------------------------------------------------------------------------*/
    /* calc array size based on first and last tdx x positions                */
    /*------------------------------------------------------------------------*/
    D = (float)fabs( x[ pInst->nrRxElements - 1 ] - x[ 0 ] );

    /*------------------------------------------------------------------------*/
    /* consistency check : 1D array                                           */
    /*------------------------------------------------------------------------*/
    for( c1 = 0; c1 < pInst->nrRxElements; c1++ ) // tbd: Trans.numelements
    {
        if( y[ c1 ] != 0.0f || z[ c1 ] != 0.0f || a[ c1 ] != 0.0f )
        {
            assert( 0 ); // should be rare ; crash on this
            return -1;
        }
    }

    /*------------------------------------------------------------------------*/
    /* convert mm to m.                                                       */
    /*------------------------------------------------------------------------*/
    *pLength = D * 1e-3f;

    /*------------------------------------------------------------------------*/
    /* done                                                                   */
    /*------------------------------------------------------------------------*/
    return 0;
}

//-----------------------------------------------------------------------------
// GetTheta
//-----------------------------------------------------------------------------
void iusRFFileGetTheta
(
    IUSRFFileInstance * pInst,
    float *             pTheta
)
{
    /*------------------------------------------------------------------------*/
    /* get theta ; use -theta due to inversed angle semantics IUS<>Verasonics */
    /*------------------------------------------------------------------------*/
    iusScaleFloatVector( pTheta, pInst->par.theta, -1.0f, pInst->nrTxEvents );
}

//-----------------------------------------------------------------------------
// GetFocus
//-----------------------------------------------------------------------------
void iusRFFileGetFocus
(
    IUSRFFileInstance * pInst,
    float *             pFocus
)
{
    /*------------------------------------------------------------------------*/
    /* convert wavecount to length in m.                                      */
    /*------------------------------------------------------------------------*/
    iusScaleFloatVector( pFocus, pInst->TX.focus, pInst->lambda, pInst->nrTxEvents );
}

//-----------------------------------------------------------------------------
// GetDelay
//-----------------------------------------------------------------------------
void iusRFFileGetDelay
(
    IUSRFFileInstance * pInst,
    float *             pDelay
)
{
    float delcomp;
    int c1;

    /*------------------------------------------------------------------------*/
    /* convert seconds to samples                                             */
    /*------------------------------------------------------------------------*/
    iusScaleFloatVector( pDelay, pInst->TX.delayOffsetSec, pInst->par.Fs, pInst->nrTxEvents );

    /*------------------------------------------------------------------------*/
    /* planewaves have negative delays (?) iso zeroes ; clip to zero..        */
    /*------------------------------------------------------------------------*/
    iusClipLoHiFloatVector( pDelay, pDelay, pInst->nrTxEvents, 0.0f, 10000.0f );

    /*------------------------------------------------------------------------*/
    /* compensate for center vs beginning of pulse.                           */
    /* TW.peak seems from TW.Wvfm2Wy64Fc (ie., 2-way) ?                       */
    /*------------------------------------------------------------------------*/
    delcomp = pInst->TW.peak * (float)pInst->par.samplesPerWave;

    /*------------------------------------------------------------------------*/
    /* compensate for different speed-of-sound in lens                        */
    /*------------------------------------------------------------------------*/
    if( strcmp( pInst->Trans.units, "mm" ) == 0 )
        delcomp += 2.0f * pInst->Trans.lensCorrection * 1e-3f / pInst->par.speedOfSound * pInst->par.Fs;
    else if( strcmp( pInst->Trans.units, "wavelengths" ) == 0 )
        delcomp += 2.0f * pInst->Trans.lensCorrection * (float)pInst->par.samplesPerWave;
    else
    {
        fprintf( stderr, "iusRFFileGetDelay: unsupported units\n" );
        assert(0);
    }

    //fprintf( stdout, "rffile, delay comp %f\n", delcomp );
    
    for( c1 = 0; c1 < pInst->nrTxEvents; c1++ ) pDelay[ c1 ] -= delcomp;
}

//-----------------------------------------------------------------------------
// Reads rfdata from a RFFile instance.
//-----------------------------------------------------------------------------
int iusRFFileRead
(    
    IUSRFFileInstance * pInst,
    float **pRFout
)
{
    const int linelen = pInst->lineLength;
    const int len     = pInst->lineLength * pInst->nrRxElements;
    short * pInShort;
    hid_t dataspace, memspace;
    hsize_t off[ 4 ];
    hsize_t cnt[ 4 ];
    hsize_t dim[ 2 ];
    herr_t  status;
    int c1;

    /*------------------------------------------------------------------------*/
    /* alloc temp mem                                                         */
    /*------------------------------------------------------------------------*/
    pInShort = iusAllocShortVector( len );
    if( pInShort == NULL )
    {
        goto bail;
    }

    /*------------------------------------------------------------------------*/
    /* set up file hyperslab : which region of the dataset M to read.         */
    /*------------------------------------------------------------------------*/
    dataspace = H5Dget_space( pInst->M_id );
    if( dataspace < 0 )
    {
        goto bail;
    }
    if( pInst->nrFrames == 1 )
    {
        off[ 0 ] = pInst->waveidx;
        off[ 1 ] = 0;
        off[ 2 ] = 0;

        cnt[ 0 ] = 1;
        cnt[ 1 ] = pInst->nrRxElements;
        cnt[ 2 ] = pInst->lineLength;
    }
    else
    {
        off[ 0 ] = pInst->frameidx;
        off[ 1 ] = pInst->waveidx;
        off[ 2 ] = 0;
        off[ 3 ] = 0;

        cnt[ 0 ] = 1;
        cnt[ 1 ] = 1;
        cnt[ 2 ] = pInst->nrRxElements;
        cnt[ 3 ] = pInst->lineLength;
    }
    status = H5Sselect_hyperslab( dataspace, H5S_SELECT_SET, off, NULL, cnt, NULL );
    if( status < 0 )
    {
        goto bail;
    }

    /*------------------------------------------------------------------------*/
    /* set up memory hyperslab : layout of memory (contiguous int16 array)    */
    /*------------------------------------------------------------------------*/
    dim[ 0 ] = pInst->nrRxElements;
    dim[ 1 ] = pInst->lineLength;
    memspace = H5Screate_simple( 2, dim, NULL );
    if( memspace < 0 )
    {
        goto bail;
    }

    off[ 0 ] = 0;
    off[ 1 ] = 0;

    cnt[ 0 ] = pInst->nrRxElements;
    cnt[ 1 ] = pInst->lineLength;

    status = H5Sselect_hyperslab( memspace, H5S_SELECT_SET, off, NULL, cnt, NULL );
    if( status < 0 )
    {
        goto bail;
    }

    /*------------------------------------------------------------------------*/
    /* read the data                                                          */
    /*------------------------------------------------------------------------*/
    status = H5Dread( pInst->M_id, H5T_NATIVE_SHORT, memspace, dataspace, H5P_DEFAULT, pInShort );
    if( status < 0 )
    {
        goto bail;
    }

    /*------------------------------------------------------------------------*/
    /* close dataspace                                                        */
    /*------------------------------------------------------------------------*/
    status = H5Sclose( dataspace );
    if( status < 0 )
    {
        goto bail;
    }

    /*------------------------------------------------------------------------*/
    /* close memspace                                                         */
    /*------------------------------------------------------------------------*/
    status = H5Sclose( memspace );
    if( status < 0 )
    {
        goto bail;
    }

    /*------------------------------------------------------------------------*/
    /* increment file read pointers                                           */
    /*------------------------------------------------------------------------*/
    pInst->waveidx++;
    if( pInst->waveidx == pInst->nrTxEvents )
    {
        pInst->waveidx = 0;
        pInst->frameidx++;
    }

    /*------------------------------------------------------------------------*/
    /* convert short to float                                                 */
    /*------------------------------------------------------------------------*/
    for( c1 = 0; c1 < pInst->nrRxElements; c1++ )
    {
        iusCopyShortVector2FloatVector( iusMatrixRow( pRFout, c1 ),
            pInShort + c1 * linelen, linelen );
    }

    /*------------------------------------------------------------------------*/
    /* free temp mem                                                          */
    /*------------------------------------------------------------------------*/
    iusFreeShortVector( pInShort );

    /*------------------------------------------------------------------------*/
    /* done                                                                   */
    /*------------------------------------------------------------------------*/
    return 0;

bail:
    /*------------------------------------------------------------------------*/
    /* error occurred                                                         */
    /*------------------------------------------------------------------------*/
    // todo: close handles, free mem..
    return -1;
}

//-----------------------------------------------------------------------------
// Free all RFFile resources.
//-----------------------------------------------------------------------------
void iusRFFileClose
(
    IUSRFFileInstance * pInst
)
{
    herr_t  status;

    assert( pInst != NULL );

    /*------------------------------------------------------------------------*/
    /* close dataset M                                                        */
    /*------------------------------------------------------------------------*/
    status = H5Dclose( pInst->M_id );
    if( status < 0 )
    {
        //..
    }

    /*------------------------------------------------------------------------*/
    /* close file                                                             */
    /*------------------------------------------------------------------------*/
    status = H5Fclose( pInst->file_id );
    if( status < 0 )
    {
        //..
    }

    /*------------------------------------------------------------------------*/
    /* free mem                                                               */
    /*------------------------------------------------------------------------*/
    iusFreeFloatMatrix( pInst->TX.Delay, pInst->nrTxEvents );
    iusFreeFloatVector( pInst->TX.delayOffsetSec );
    iusFreeFloatVector( pInst->TX.focus );
    iusFreeFloatVector( pInst->par.theta );
    iusFreeFloatMatrix( pInst->Trans.ElementPos, 4 );
    free( pInst );
}

//-----------------------------------------------------------------------------
// Read float vector from file
//-----------------------------------------------------------------------------
static int LF_readVector
(
    IUSRFFileInstance * pInst,
    const char * const varpath,
    const int N,
    float ** pOut
)
{
    hsize_t dims[ 3 ];
    herr_t  status;

    /*------------------------------------------------------------------------*/
    /* get vec size                                                           */
    /*------------------------------------------------------------------------*/
    dims[ 0 ] = 0; dims[ 1 ] = 0; dims[ 2 ] = 0;
    status = H5LTget_dataset_info( pInst->file_id, varpath, dims, NULL, NULL );
    if( status < 0 )
    {
        goto bail;
    }

    /*------------------------------------------------------------------------*/
    /* verify vec size                                                        */
    /*------------------------------------------------------------------------*/
    if( (int)dims[ 0 ] != N || (int)dims[ 1 ] != 1 || (int)dims[ 2 ] != 0 )
    {
        goto bail;
    }

    /*------------------------------------------------------------------------*/
    /* alloc mem for vec                                                      */
    /*------------------------------------------------------------------------*/
    (*pOut) = iusAllocFloatVector( N );
    if( (*pOut) == NULL )
    {
        goto bail;
    }

    /*------------------------------------------------------------------------*/
    /* read vec                                                               */
    /*------------------------------------------------------------------------*/
    status = H5LTread_dataset_float( pInst->file_id, varpath, (*pOut) );
    if( status < 0 )
    {
        goto bail;
    }

    /*------------------------------------------------------------------------*/
    /* done                                                                   */
    /*------------------------------------------------------------------------*/
    return 0;

bail:
    /*------------------------------------------------------------------------*/
    /* error                                                                  */
    /*------------------------------------------------------------------------*/
    return -1;
}

//-----------------------------------------------------------------------------
// Read float vector from file
//-----------------------------------------------------------------------------
static int LF_readCellVector
(
    IUSRFFileInstance * pInst,
    const char * const group,
    const char * const var,
    const int N,
    float ** pOut
)
{
    hsize_t dims[ 3 ];
    hid_t   grp, ds, ds2;
    hobj_ref_t ** ref;
    herr_t  status;
    int     c1;

    /*------------------------------------------------------------------------*/
    /* open group (cell array)                                                */
    /*------------------------------------------------------------------------*/
    grp = H5Gopen2( pInst->file_id, group, H5P_DEFAULT );
    if( grp < 0 )
    {
        goto bail;
    }

    /*------------------------------------------------------------------------*/
    /* get vector dim                                                         */
    /*------------------------------------------------------------------------*/
    dims[ 0 ] = 0; dims[ 1 ] = 0; dims[ 2 ] = 0;
    status = H5LTget_dataset_info( grp, var, dims, NULL, NULL );
    if( status < 0 )
    {
        goto bail;
    }

    /*------------------------------------------------------------------------*/
    /* verify vector dim                                                      */
    /*------------------------------------------------------------------------*/
    if( (int)dims[ 0 ] != N || (int)dims[ 1 ] != 1 || (int)dims[ 2 ] != 0 )
    {
        goto bail;
    }

    /*------------------------------------------------------------------------*/
    /* alloc mem for vec                                                      */
    /*------------------------------------------------------------------------*/
    (*pOut) = iusAllocFloatVector( N );
    if( (*pOut) == NULL )
    {
        goto bail;
    }

    /*------------------------------------------------------------------------*/
    /* open group dataset                                                     */
    /*------------------------------------------------------------------------*/
    ds = H5Dopen2( grp, var, H5P_DEFAULT );
    if( ds < 0 )
    {
        goto bail;
    }

    /*------------------------------------------------------------------------*/
    /* alloc temp mem                                                         */
    /*------------------------------------------------------------------------*/
    ref = (hobj_ref_t**)calloc( N, sizeof( hobj_ref_t* ) );
    if( ref == NULL )
    {
        goto bail;
    }

    /*------------------------------------------------------------------------*/
    /* read object refs                                                       */
    /*------------------------------------------------------------------------*/
    status = H5Dread( ds, H5T_STD_REF_OBJ, H5S_ALL, H5S_ALL, H5P_DEFAULT, ref );
    if( status < 0 )
    {
        goto bail;
    }

    /*------------------------------------------------------------------------*/
    /* iterate over cell array                                                */
    /*------------------------------------------------------------------------*/
    for( c1 = 0; c1 < N; c1++ )
    {
        /*--------------------------------------------------------------------*/
        /* dereference ref                                                    */
        /*--------------------------------------------------------------------*/
        ds2 = H5Rdereference( ds, H5R_OBJECT, &(ref[ c1 ]) );
        if( ds2 < 0 )
        {
            goto bail;
        }

        /*--------------------------------------------------------------------*/
        /* init with checkvalue                                               */
        /*--------------------------------------------------------------------*/
        (*pOut)[ c1 ] = -123.456f;

        /*--------------------------------------------------------------------*/
        /* read one float                                                     */
        /*--------------------------------------------------------------------*/
        status = H5Dread( ds2, H5T_NATIVE_FLOAT, H5S_ALL, H5S_ALL, H5P_DEFAULT, &((*pOut)[ c1 ]) );
        if( status < 0 )
        {
            goto bail;
        }

        /*--------------------------------------------------------------------*/
        /* check checkvalue                                                   */
        /*--------------------------------------------------------------------*/
        if( (*pOut)[ c1 ] == -123.456f )
        {
            goto bail;
        }

        /*--------------------------------------------------------------------*/
        /* close                                                              */
        /*--------------------------------------------------------------------*/
        status = H5Dclose( ds2 );
        if( status < 0 )
        {
            goto bail;
        }
    }

    /*------------------------------------------------------------------------*/
    /* free temp mem                                                          */
    /*------------------------------------------------------------------------*/
    free( ref );

    /*------------------------------------------------------------------------*/
    /* close dataset                                                          */
    /*------------------------------------------------------------------------*/
    status = H5Dclose( ds );
    if( status < 0 )
    {
        goto bail;
    }

    /*------------------------------------------------------------------------*/
    /* close group                                                            */
    /*------------------------------------------------------------------------*/
    status = H5Gclose( grp );
    if( status < 0 )
    {
        goto bail;
    }

    /*------------------------------------------------------------------------*/
    /* done                                                                   */
    /*------------------------------------------------------------------------*/
    return 0;

bail:
    return -1;
}

//-----------------------------------------------------------------------------
// Read float matrix from file
//-----------------------------------------------------------------------------
static int LF_readCellMatrix
(
    IUSRFFileInstance * pInst,
    const char * const group,
    const char * const var,
    const int N,
    const int M,
    float *** pOut
)
{
    hsize_t dims[ 3 ];
    hid_t   grp, ds, ds2;
    hobj_ref_t ** ref;
    herr_t  status;
    int     c1, c2;

    /*------------------------------------------------------------------------*/
    /* open group (cell array)                                                */
    /*------------------------------------------------------------------------*/
    grp = H5Gopen2( pInst->file_id, group, H5P_DEFAULT );
    if( grp < 0 )
    {
        goto bail;
    }

    /*------------------------------------------------------------------------*/
    /* get matrix dim                                                         */
    /*------------------------------------------------------------------------*/
    dims[ 0 ] = 0; dims[ 1 ] = 0; dims[ 2 ] = 0;
    status = H5LTget_dataset_info( grp, var, dims, NULL, NULL );
    if( status < 0 )
    {
        goto bail;
    }

    /*------------------------------------------------------------------------*/
    /* verify list dim                                                        */
    /*------------------------------------------------------------------------*/
    if( (int)dims[ 0 ] != N || (int)dims[ 1 ] != 1 || (int)dims[ 2 ] != 0 )
    {
        goto bail;
    }

    /*------------------------------------------------------------------------*/
    /* alloc mem for mat                                                      */
    /*------------------------------------------------------------------------*/
    (*pOut) = iusAllocFloatMatrix( N, M );
    if( (*pOut) == NULL )
    {
        goto bail;
    }

    /*------------------------------------------------------------------------*/
    /* open group dataset                                                     */
    /*------------------------------------------------------------------------*/
    ds = H5Dopen2( grp, var, H5P_DEFAULT );
    if( ds < 0 )
    {
        goto bail;
    }

    /*------------------------------------------------------------------------*/
    /* alloc temp mem                                                         */
    /*------------------------------------------------------------------------*/
    ref = (hobj_ref_t**)calloc( N, sizeof( hobj_ref_t* ) );
    if( ref == NULL )
    {
        goto bail;
    }

    /*------------------------------------------------------------------------*/
    /* read object refs                                                       */
    /*------------------------------------------------------------------------*/
    status = H5Dread( ds, H5T_STD_REF_OBJ, H5S_ALL, H5S_ALL, H5P_DEFAULT, ref );
    if( status < 0 )
    {
        goto bail;
    }

    /*------------------------------------------------------------------------*/
    /* iterate over cell array                                                */
    /*------------------------------------------------------------------------*/
    for( c1 = 0; c1 < N; c1++ )
    {
        /*--------------------------------------------------------------------*/
        /* dereference ref                                                    */
        /*--------------------------------------------------------------------*/
        ds2 = H5Rdereference( ds, H5R_OBJECT, &(ref[ c1 ]) );
        if( ds2 < 0 )
        {
            goto bail;
        }

        /*--------------------------------------------------------------------*/
        /* init with checkvalue                                               */
        /*--------------------------------------------------------------------*/
        iusSetValue2FloatVector( iusMatrixRow( (*pOut), c1 ), M, -123.456f );

        /*--------------------------------------------------------------------*/
        /* read float vec                                                     */
        /*--------------------------------------------------------------------*/
        status = H5Dread( ds2, H5T_NATIVE_FLOAT, H5S_ALL, H5S_ALL, H5P_DEFAULT, iusMatrixRow( (*pOut), c1 ) );
        if( status < 0 )
        {
            goto bail;
        }

        /*--------------------------------------------------------------------*/
        /* check checkvalue                                                   */
        /*--------------------------------------------------------------------*/
        for( c2 = 0; c2 < M; c2++ )
        {
            if( ( iusMatrixRow( (*pOut), c1 ) )[ c2 ] == -123.456f )
            {
                goto bail;
            }
        }

        /*--------------------------------------------------------------------*/
        /* close                                                              */
        /*--------------------------------------------------------------------*/
        status = H5Dclose( ds2 );
        if( status < 0 )
        {
            goto bail;
        }
    }

    /*------------------------------------------------------------------------*/
    /* free temp mem                                                          */
    /*------------------------------------------------------------------------*/
    free( ref );

    /*------------------------------------------------------------------------*/
    /* close dataset                                                          */
    /*------------------------------------------------------------------------*/
    status = H5Dclose( ds );
    if( status < 0 )
    {
        goto bail;
    }

    /*------------------------------------------------------------------------*/
    /* close group                                                            */
    /*------------------------------------------------------------------------*/
    status = H5Gclose( grp );
    if( status < 0 )
    {
        goto bail;
    }

    /*------------------------------------------------------------------------*/
    /* done                                                                   */
    /*------------------------------------------------------------------------*/
    return 0;

bail:
    return -1;
}
