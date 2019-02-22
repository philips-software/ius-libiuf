
// Created by nlv09165 on 25/07/2018.
#ifndef IUSLIBRARY_IUSHL3DPARAMETRICSOURCE_H
#define IUSLIBRARY_IUSHL3DPARAMETRICSOURCE_H

#include "iusPosition.h"
#include <iusSourcePrivate.h>

#if 0
// ADT
typedef struct Ius3DParametricSource Ius3DParametricSource;
/** The 3D parametric sources describes source locations from which a pulse originates simulatanously with a set of parameters. 
* A 2D array of sources on a spherical 
*/
typedef Ius3DParametricSource *iu3dps_t;
#define  IU3DPS_INVALID (iu3dps_t) NULL
#endif

/** \brief Construct a (number of) source point(s) using a parametric description of their locations 
 * 
 * the sources are points in 3D space on a spherical surface, the radius of this sphere is defined by the fNumber
 * the locations are on a grid described by a start, delta and numLocations in the specific dimension. 
 * The total number of locations 
 *
*/
ius_t ius3DParametricSourceCreate
(
    int numThetaLocations,  ///< The number of sources in theta dimension
	int numPhiLocations,    ///< The number of sources int phi dimension
    float fNumber,          ///< The apertuseSize-source dustance ratio
    float deltaTheta,       ///< the distance between sources along the Theta dimension
    float startTheta,       ///< the first angle in the Theta dimension
    float deltaPhi,         ///< the distance between sources along the Phi dimension
    float startPhi          ///< the first angle in the Phi dimension
);

/** \brief Deletes the 3D source
*  
* \return When the source is deleted it return IUS_E_OK, otherwise IUS_ERR_VALUE
*/
int ius3DParametricSourceDelete
(
    ius_t source ///< The 3D source to be deleted
);


/** \brief Compares 2 3D parametric sources
* checks if the contents of the two 3D sources are identical. 
* the order of the arguments don't matter. 
* \return The comparison returns IUS_FALSE if the parametric sources differ, IUS_TRUE if their content is equal
*/
int ius3DParametricSourceCompare
(
    ius_t reference, ///< The 3D parametric source to compare with
    ius_t actual     ///< The 3D parametric source to compare
);

/** \brief Return the FNumber of the 3D parametric source
* 
* \return the fNumber (dimensionless of this source)
*/
float ius3DParametricSourceGetFNumber
(
    ius_t source ///< The 3D parametric source that holds the fNumber
);

/** \brief Return the delta angle between two thetas of the 3D parametric source
*
* \return the anglular step size in radians (negative values are allowed) along the theta dimension
*/
float ius3DParametricSourceGetDeltaTheta
(
    ius_t source ///< The 3D parametric source that holds the delta theta
);

/** \brief Return the starting angle theta
*
* \return the offset angle in radians along the theta axis
*/
float ius3DParametricSourceGetStartTheta
(
    ius_t source ///< The 3D parametric source that has the start theta
);

/** \brief Return the delta angle between two phis of the 3D parametric source
*
* \return the anglular step size in radians (negative values are allowed) along the phi dimension
*/
float ius3DParametricSourceGetDeltaPhi
(
    ius_t source ///< The 3D parametric source that holds the dleta phi
);

/** \brief Return the number of sources in theta dimension
*
* \return The number of sources in theta dimension
*/
int ius3DParametricSourceGetNumThetaLocations
(
    ius_t source  ///< The source of interest
);

/** \brief Return the number of sources in phi dimension
*
* \return The number of sources in phi dimension
*/
int ius3DParametricSourceGetNumPhiLocations
(
    ius_t source  ///< The source of interest
);

/** \brief Return the starting angle phi
*
* \return the offset angle in radians along the phi axis
*/
float ius3DParametricSourceGetStartPhi
(
    ius_t source ///< The 3D parametric source that holds the sarting phi
);

/** Set the fNumber of the provided 3D parametric source 
*
* \return Returns IUS_E_OK when the fNumber has been set or IUS_ERR_VALUE when the 3D source is invalid   
*/
int ius3DParametricSourceSetFNumber
(
    ius_t source,  ///< The 3D parametric source of which the fNumber is set
    float FNumber                    ///< The fNumber is the ratio between source distance and the transducer's aperture width
);

/** Set the angular distance between sources
*
* \return Returns IUS_E_OK when the delta has been set or IUS_ERR_VALUE when the 3D source is invalid
*/
int ius3DParametricSourceSetDeltaTheta
(
    ius_t source,  ///< The 3D parametric source of which the deltaTheta is set in theta dimension
    float deltaTheta                 ///< The angular delta is the arc-distance in rad between sources  
);

/** Set the angular offset of the source in theta direction
*
* \return Returns IUS_E_OK when the start value has been set or IUS_ERR_VALUE when the 3D source is invalid
*/
int ius3DParametricSourceSetStartTheta
(
    ius_t source,  ///< The 3D parametric source of which the start angle is set in theta dimension
    float startTheta                 ///< The start angle is the arc-offset in rad of the sources, i.e. the angle of the first source 
);

/** Set the angular offset of sources
*
* \return Returns IUS_E_OK when the delta has been set or IUS_ERR_VALUE when the 3D source is invalid
*/
int ius3DParametricSourceSetDeltaPhi
(
    ius_t source,  ///< The 3D parametric source of which the deltaPhi is set
    float deltaPhi					 ///< The angular delta is the arc-distance in rad between sources 
);

/** Set the angular offset of the source in phi direction
*
* \return Returns IUS_E_OK when the start value has been set or IUS_ERR_VALUE when the 3D source is invalid
*/
int ius3DParametricSourceSetStartPhi
(
    ius_t source,  ///< The 3D parametric source of which the startPhi is set
    float startPhi					 ///< The start angle is the offset in rad of the first source 
);


#endif //IUSLIBRARY_IUSHL3DPARAMETRICSOURCE_H
