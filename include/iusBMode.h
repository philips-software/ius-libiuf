//===========================================================================*/
//     COPYRIGHT 2016 PHILIPS RESEARCH
//===========================================================================*/
//
//  Source Name   : iusBModeInstance.h
//                  BMode frame construction from polar (compounded) image
//  iusVersion    : 255.255.255.255
//  author        : Frank van Heesch, Ben Bierens, Harm Belt
//
//==============================================================================

#ifndef _IUSBMODE_H
#define _IUSBMODE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "iusTypes.h"
#include "iusNode.h"

#define IUS_BMODE_TYPE "IusBMODEFILETYPE_V3"

/**
 * \brief BMode internal data structure
 * 
 * Properties that fully describe BMode data
 *
 * \param[in] pPixelGrid :          Cartesian coordinates of the data (in [m]) output 
 * \param[in] pixelsPerMeter :      Resolution in pixels per meter     
 * \param[in] interpolationMethod : The way samples are interpolated          
 * \param[in] IusVersion :          The IUS version compatibility number      
 */
typedef struct
{
    IusNode   iusNode;

    IusGrid * pPixelGrid;
    float     pixelsPerMeter;
    int       interpolationMethod; 
    int       IusVersion;          
} IusBModeInstance;

/*
 * \brief Creates a BMode instance with the specified parameters,
 * allocates internal memory and initializes internal data.
 *
 * In-place:   n.a.
 * \pre        The instance object is empty
 * \post       The instance object is initialized and pars have been set
 * \param[in]  pPixelGrid          : Cartesian coordinates describing the data in [m] 
 * \param[in]  pixelsPerMeter      : Resolution in pixels per meter
 * \param[in]  interpolationMethod : The way samples are interpolated  
 * \return     the BMode instance 
 */
IusBModeInstance * iusBModeCreate
(
    const IusGrid * const pPixelGrid,
    float                 pixelsPerMeter,
    int                   interpolationMethod
);

/**
 * Reads a BMode instance and populates it with the data from the HDF5 handle.
 *
 * \brief reads the BMode data structure from a (file) handle
 * \param[in]   handle           : Handle to the HDF file or group which represents the root node for an input data structure.
 * \return      IusBModeInstance : Instance pointer (or NULL)
 */
IusBModeInstance * iusBModeRead
(
    hid_t handle,
    int   verbose
);

/**
 * \brief Writes an input instance to a handle
 *
 * \brief parameters for writing the data
 * \param[in]   handle           : Handle to write the IUSInputInstance data to
 * \param[in]   IUSInputInstance : Instance pointer
 * \return      error code;
 */
int iusBModeWrite
(
    hid_t                    handle,
    IusBModeInstance * const pIusBMode,
    int                      verbose
);


/**
 * \brief Frees all BMode resources
 *
 * In-place:   n.a.
 * \pre        The instance object was created
 * \post       The contents of CWC4DWD is freed
 * \param[in]  pInst : pointer to CWC4DWD instance
 * \return     void
 */
void iusBModeDestroy
(
    IusBModeInstance * pInst
);

#ifdef __cplusplus
}
#endif

#endif  /* _IUSBMODE_H */
