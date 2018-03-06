//=============================================================================
//    COPYRIGHT 2015 PHILIPS RESEARCH
//=============================================================================
//
//  Source Name   : iusRFFileTest.c
//                  tests the rffile module
//  iusVersion    : 255.255.255.255
//
//=============================================================================

#include "iusRFFile.h"
#include "iusVector.h"
#include "iusMatrix.h"
#include <stdio.h>
#include <assert.h>

#ifdef _WIN32
//#define LD_FILEPATH  "\\\\code1\\storage\\2016_ius_ux\\INPUT\\regression\\CWC4DWD\\20160201_172319_divergingBeams_short.mat"
#define LD_FILEPATH  "\\\\code1\\storage\\2016_ius_ux\\INPUT\\regression\\CWC4DWD\\20160201_173335_divergingBeams_short.mat"
#else
// #define LD_FILEPATH  "/home/2014-0083_satellitedata/Test/Verasonics/synth_plate/synth_plate_divergingWaves.mat"
#define LD_FILEPATH  "/Users/nlv09165/proj/PPF/ius/INPUT/regression/Vera2Input/example.mat"
#endif

int main( int argc, char * argv[] )
{
    float * * pData;
    IUSRFFileInstance * pInst;
    FILE * f;
    int    rflength;
    int    rfwidth;
    int    Nwaves, Nframes;
    float  D;
    float  tx;
    int    rv;
    int    c1, c2, c3;

    if( argc <= 1 )
    {
        fprintf( stdout, "Running self-test (%s).\n", argv[ 0 ] );
    }
    else
    {
        fprintf( stderr, "Usage: %s\n", argv[ 0 ] );
        return 1;
    }

    f = fopen( "rffiledump.dat", "wt" );
    assert( f != NULL );
    
    // initialize module
    pInst = iusRFFileOpen( LD_FILEPATH, 1 );
    assert( pInst != NULL );

    rflength = pInst->lineLength;
    rfwidth  = pInst->nrRxElements;
    Nwaves   = pInst->nrTxEvents;
    Nframes  = pInst->nrFrames;

    fprintf( stdout, "M is %i x %i x %i x %i\n", rflength, rfwidth, Nwaves, Nframes );

    rv = iusRFFileGetElementSpacing( pInst, &tx );
    if( rv != 0 )
    {
        fprintf( stderr, "error in iusRFFileGetElementSpacing\n" );
        return 1;
    }
    fprintf( stdout, "elementspacing is %f m\n", tx );

    rv = iusRFFileGetArrayLength( pInst, &D );
    if( rv != 0 )
    {
        fprintf( stderr, "error in iusRFFileGetArrayLength\n" );
        return 1;
    }
    fprintf( stdout, "the size of the array is %f m\n", D );

    // Allocate some memory
    pData = iusAllocFloatMatrix( rfwidth, rflength );
    assert( pData != NULL );

    // iterate over waves x frames
    fprintf( stderr, "iterate over %d waves x %d frames\n", Nwaves, Nframes );
    for( c1 = 0; c1 < Nwaves * Nframes; c1++ )
    {
        // Perform reading
        rv = iusRFFileRead( pInst, pData );
        if( rv != 0 )
        {
            fprintf( stderr, "error in iusRFFileRead\n" );
            return 1;
        }

        for( c2 = 0; c2 < rfwidth; c2++ )
        {
            const float * const r = iusMatrixRow( pData, c2 );
            float s = 0.0f;
            for( c3 = 0; c3 < rflength; c3++ )
            {
                s += r[ c3 ];
            }
            fprintf( f, "%e\t", s );
        }
        fprintf( f, "\n" );
    }

    // Free
    iusFreeFloatMatrix( pData, rfwidth );

    iusRFFileClose( pInst );

    fclose( f );

    return 0;
}
