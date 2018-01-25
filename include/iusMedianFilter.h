//==============================================================================
//    COPYRIGHT 2015 PHILIPS RESEARCH
//==============================================================================
//
//  Source Name   : iusMedianFilter.h
//                  1D median filter
//  iusVersion    : 255.255.255.255
//
//==============================================================================

/**
 * \file iusMedianFilter.h
 * \brief A 1D median filter 
 */

#ifndef _IUSMEDIANFILTER_H
#define _IUSMEDIANFILTER_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief This MedianFilter instance contains an internal work buffer and
 * parameters
 */
typedef struct
{
    float * pWorkBuffer;
    float * pInputData;
    int     inputBufferSize;
    int     windowWidth;
   
} IUSMedianFilterInstance;

/**
 * \brief Creates an instance of the 1D median filter algorithm with the
 * specified parameters, allocates internal memory and initializes internal data.
 *
 * \pre         The instance object is empty.
 * \post        The instance object is initialized and parameters have been set.
 * \param[in]   inputBufferSize     size of the input data array (>0)
 * \param[in]   windowWidth         span of the median operation (>0 and odd)
 * \return      pointer to instance of median filter
 */
IUSMedianFilterInstance * iusMedianFilterCreate
(
    int inputBufferSize,
    int windowWidth
);

/**
 * \brief Runs an instance of the 1D median filter taking input vector and
 * output vector.
 *
 * In-place:    yes (pDest==pSrc is allowed)
 * \pre         The instance object was created. Input and output data vectors
 *              have been allocated. Input array contains data.
 * \post        Data has been processed, internal states have been updated.
 * \param[in]   pInst   : pointer to median filter instance by iusAlgoCreate()
 * \param[out]  pDest   : points to input data
 * \param[in]   pSrc    : points to output data
 * \return      void 
 */
void iusMedianFilterRun
(
    IUSMedianFilterInstance * pInst,
    float * const             pDest,
    const float * const       pSrc,
    int                       length
);

/**
 * \brief Frees all resources of the 1D median filter.
 *
 * In-place:   n.a.
 * \pre        The instance object was created.
 * \post       The contents of MedianFilter is freed, pInst is NULL
 * \param[in]  pInst   : pointer to median filter instance by iusAlgoCreate()
 * \return     void
 */
void iusMedianFilterDestroy
(
    IUSMedianFilterInstance * pInst
);

#ifdef __cplusplus
}
#endif

#endif // _IUSMEDIANFILTER_H
