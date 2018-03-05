//=============================================================================
//    COPYRIGHT 2015 PHILIPS RESEARCH
//=============================================================================
//
//  Source Name   : iusRFFile.h
//                  Read-only RF file (in hdf5 format (aka v7.3 .mat file)).
//                  See iusSavePlaneWaves.m
//  iusVersion    : 255.255.255.255
//
//=============================================================================

#ifndef _IUSRFFILE_H
#define _IUSRFFILE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <hdf5.h>

//-----------------------------------------------------------------------------
// RF File (internal) data structure
//
// The .mat file has these top level variables:
//   'M', 'timeStampDMA', 'Trans', 'TW', 'TX', 'TGC', 'Receive', 'par'
//-----------------------------------------------------------------------------
typedef struct 
{
    //
    // The parameters as obtained from the rffile ; public (read-only) access
    //
    int             lineLength;     // size(M,1) ; length of rfdata (line length)
    int             nrRxElements;   // size(M,2) ; width of rfdata (nr of lines)
    int             nrTxEvents;     // size(M,3) ; nr of transmits
    int             nrFrames;       // size(M,4) ; nr of frames

    struct
    {
        int         numelements;    // Trans.numelements
        float * *   ElementPos;     // Trans.ElementPos ; tdx xyzw positions, Ntdx x 4
        float       frequency;      // Trans.frequency  ; center freq in MHz
        float       lensCorrection; // Trans.lensCorrection ;
        char        units[ 16 ];    // Trans.units ; 'mm' or '.....'
    } Trans;
    
    struct
    {
        float       speedOfSound;   // par.speedOfSound ; in m/s
        float       Fs;             // par.Fs           ; sampling frequency in Hz
        float *     theta;          // par.theta        ; wave angles in rad
        float       waveRate;       // par.waveRate     ; nr waves per s
        int         nrFramesPerDMA; // par.nrBModeFramesPerDMA
        int         samplesPerWave; // par.samplesPerWave
        int         ensembleSize;   // par.ensembleSize
        float       txFNumber;      // par.txFNumber    ; FNumber (source focus distance)
        float       pulseFreqMHz;   // par.pulseFreqMHz ; The frequency [Hz] of the transmit pulse
        int         nrPulseCycles;  // par.nrPulseCycles;
        float       tPCHighVoltage; // par.tPCHighVoltage ; The amplitude [V] of the transmit pulse
    } par;

    struct
    {
        float *     focus;          // TX{}.focus       ; diverging waves, focus in #waves ?
        float *     delayOffsetSec; // TX{}.delayOffsetSec
        float * *   Delay;          // TX{}.Delay
    } TX;

    struct
    {
        float       peak;           // TW.peak
        float       samplesPerWL;   // TW.samplesPerWL
        int         numsamples;     // TW.numsamples
    } TW;

    float           Fc;             // transducer center freq (in Hz) (for convenience)
    float           lambda;         // wavelength (in m) (for convenience)

    //
    // internal variables
    //
    int             waveidx;        // wave index counter
    int             frameidx;       // frame index counter

    //
    // handles
    //
    hid_t           file_id;        // file
    hid_t           M_id;           // M dataset

} IUSRFFileInstance;

//-----------------------------------------------------------------------------
// Creates a File instance with the specified parameters, allocates
// internal memory and initializes internal data.
//
// In-place:   n.a.
// pre         The instance object is empty.
// post        The instance object is initialized and parameters have been set.
// param[in]   pFullFileName : full name of hdf5 input file (path + name + ext).
// param[in]   verbose       : boolean, print file information to stdout.
// return      File instance pointer (or NULL)
//-----------------------------------------------------------------------------
IUSRFFileInstance * iusRFFileOpen
(
    const char * const pFullFileName,
    const int verbose
);

//-----------------------------------------------------------------------------
// Retrieves the amount of spacing between the centers of the transducers.
//
// In-place:   n.a.
// pre         The instance object was created. The array is 1D and uniform.
// post        The distance was assigned to *pTdxSpacing.
// param[in]   pInst       : File instance
// param[out]  pTdxSpacing : spacing (in m)
// return      int         : 0 on success
//-----------------------------------------------------------------------------
int iusRFFileGetElementSpacing
(
    IUSRFFileInstance * pInst,
    float *             pTdxSpacing
);

//-----------------------------------------------------------------------------
// Retrieves the length of the array, measured between the centers of the outer elements.
//
// In-place:   n.a.
// pre         The instance object was created. The transducer-array is 1D.
// post        The length was assigned to *pLength.
// param[in]   pInst   : File instance
// param[out]  pLength : length (in m)
// return      int     : 0 on success
//-----------------------------------------------------------------------------
int iusRFFileGetArrayLength
(
    IUSRFFileInstance * pInst,
    float *             pLength
);

//-----------------------------------------------------------------------------
// Retrieves wave theta, in IUS conventions.
//
// In-place:   n.a.
// pre         The instance object was created.
// post        Theta was assigned to *pTheta.
// param[in]   pInst   : File instance
// param[out]  pTheta  : angle (in rad) per transmit event
// return      void    :
//-----------------------------------------------------------------------------
void iusRFFileGetTheta
(
    IUSRFFileInstance * pInst,
    float *             pTheta
);

//-----------------------------------------------------------------------------
// Retrieves wave focus.
//
// In-place:   n.a.
// pre         The instance object was created.
// post        Focus was assigned to *pFocus.
// param[in]   pInst   : File instance
// param[out]  pFocus  : focus distance (in m) per transmit event
// return      void    :
//-----------------------------------------------------------------------------
void iusRFFileGetFocus
(
    IUSRFFileInstance * pInst,
    float *             pFocus
);

//-----------------------------------------------------------------------------
// Retrieves wave delay offset.
//
// In-place:   n.a.
// pre         The instance object was created.
// post        Delay was assigned to *pDelay.
// param[in]   pInst   : File instance
// param[out]  pDelay  : delay offset (in samples) per transmit event
// return      void    :
//-----------------------------------------------------------------------------
void iusRFFileGetDelay
(
    IUSRFFileInstance * pInst,
    float *             pDelay
);

//-----------------------------------------------------------------------------
// Reads RF data from a File instance, giving a single matrix of data.
//
// In-place:   n/a
// pre         The instance object was created. Output data lines
//             have been allocated.
// post        Data was read, internal states have been updated.
// param[in]   pInst   : File instance
// param[out]  pRFout  : RF data, matrix of lineLength x nrRxElements
// return      int     : 0 on success
//-----------------------------------------------------------------------------
int iusRFFileRead
(    
    IUSRFFileInstance * pInst,
    float * * pRFout
);

//-----------------------------------------------------------------------------
// Frees all File resources.
//
// In-place:   n.a.
// @pre        The instance object was created.
// @post       The contents of File is freed.
// @param[in]  pInst:  pointer to File instance
// return      void
//-----------------------------------------------------------------------------
void iusRFFileClose
(
    IUSRFFileInstance * pInst
);

#ifdef __cplusplus
}
#endif

#endif  // _IUSRFFILE_H
