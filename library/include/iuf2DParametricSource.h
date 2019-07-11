
#ifndef IUFLIBRARY_IUFHL2DPARAMETRICSOURCE_H
#define IUFLIBRARY_IUFHL2DPARAMETRICSOURCE_H

#include "iufPosition.h"

// ADT
typedef struct Iuf2DParametricSource Iuf2DParametricSource;
/** The 2D parametric Sources describes one or multiple point sources from which a pulse originates simulatanously. You can
  * create a source with a predefined number of locations and a paramteric description of where these points are located.
  * The parameters decribe points on a circlular arc, defined by the fNumber (radiuf relative to the aperture), a starting
  * angle and a delta angle (both in radians).
  * iuf2DParametricSourceGetPosition() and iuf2DParametricSourceGetPosition() respectively. */
typedef Iuf2DParametricSource *iu2dps_t;

/** An invalid 2D Non-Parametric Source */
#define  IU2DPS_INVALID (iu2dps_t) NULL

/** \brief Create an ultrasound source for generating a pulseform based on \p numLocations points.
 * The points are described directly by their 2D position (i.e. not parametrically) .
 * \return returns the created 2D non-parameric source.
 */
iu2dps_t iuf2DParametricSourceCreate
(
    int numLocations,   ///< The number of source locations that are triggered simulatenously
    float fNumber,      ///< the fNumber is the Focal depth (radiuf of the circle where the sources are positions) over the aperture diameter.
    float deltaTheta,   ///< The arc length in radians between sources
    float startTheta    ///< the angle of the first source
);

/** \brief Delete a 2D parametric source.
 * \return Returns #IUF_E_OK on success or #IUF_ERR_VALUE in case of an error.
 */
int iuf2DParametricSourceDelete
(
    iu2dps_t source ///< the source that will be deleted
);

/** \brief Compares two 2D parametric sources. It does not matter which is the reference and the actual.
 * \return #IUF_TRUE if reference and actual are identical sources, and #IUF_FALSE in all other cases.
 */
int iuf2DParametricSourceCompare
(
    iu2dps_t reference, ///< 2D non-parametric source to compare
    iu2dps_t actual     ///< 2D non-parametric source to compare with.
);

/** \brief Returns the fNumber of this source, or NAN if the source is invalid
 */
float iuf2DParametricSourceGetFNumber
(
    iu2dps_t source ///< The source of interest
);

/** \brief Returns the angular delta in radians between source points or NAN if the source is invalid.
 */
float iuf2DParametricSourceGetDeltaTheta
(
    iu2dps_t source ///< The source of interest
);

/** \brief Returns the angle of the first source point or NAN if the source is invalid
 */
float iuf2DParametricSourceGetStartTheta
(
    iu2dps_t source ///< The source of interest
);

/** \brief  Gets the number of source locations that are triggered simulatenously.
 * \return  The number of source locations that are triggered simulatenously.
 */
int iuf2DParametricSourceGetNumLocations
(
    iu2dps_t source     ///< The source of interest
);


/** \brief Sets the fNumber of the parametric sources, return #IUF_E_OK when successful or #IUF_ERR_VALUE in case
* the source is incorrect
*/
int iuf2DParametricSourceSetFNumber
(
    iu2dps_t source, ///< The source of interest
    float FNumber                   ///< the value of the fNumber
);

/** \brief Sets the delta between sources, return #IUF_E_OK when successful or #IUF_ERR_VALUE in case
* the source is incorrect
*/
int iuf2DParametricSourceSetDeltaTheta
(
    iu2dps_t source, ///< The source of interest
    float deltaTheta                ///< the value of the delta angle
);

int iuf2DParametricSourceSetStartTheta
(
    iu2dps_t source, ///< The source of interest
    float startTheta                ///< the value of the starting angle
);

#endif //IUFLIBRARY_IUFHL2DPARAMETRICSOURCE_H
