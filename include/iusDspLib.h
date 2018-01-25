
#ifndef DSPLIB_H
#define DSPLIB_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef NTT_EMBEDDED

/**
 * \brief       Fixed point FIR filter with 16 coefficients
 *              (Natural C code from TI) 
 * \pre         All arrays have been allocated, all elements of input arrays
 *              pX[] and pH[] been assigned values in the range [-32768,32767].
 *              For BIBO stability the l1-norm of pH[] must not exceed 32767.
 *              in addition, nr>=4, and nr%4==0. 
 * \post        Output values have been written to pR[] with values in the range
 *              [-32768,32767].
 * \param[in]   pX:  input array having nr+16-1 elements, where the first 16-1
 *                   elements belong to the past with sample index n < 0, the
 *                   16-th element is the sample with index n = 0.
 * \param[in]   pH:  coefficient array having 16 elements with the filter impulse
 *                   response in reverse order
 * \param[out]  pR:  output array having nr elements containing the convolution
 *                   result
 * \param[in]   nr:  number of output samples
 * \return      pointer to the vector data
 */
void DSP_fir_r8_h16
(
    const short *pX,
    const short *pH,
    short       *pR,
    int         nr
);

void DSPF_q15tofl
(
    const short *pX,
    float       *pY,
    const int    n
);

#else

void DSP_fir_r8_h16
(
    const short *restrict x,    /* Input array [nr+16-1 elements] (A4)*/
    const short *restrict h,    /* Coeff array [16 elements]      (B4)*/
    short       *restrict r,    /* Output array [nr elements]     (A6)*/
    int nr                      /* Number of output samples       (A8)*/
);

void DSPF_q15tofl
(
    const short *restrict x,
    float *restrict y,
    const int n
);

#endif

#ifdef __cplusplus
}
#endif

#endif
