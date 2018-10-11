
// Created by nlv09165 on 25/07/2018.
#ifndef IUSLIBRARY_IUSHL3DNONPARAMETRICSOURCE_H
#define IUSLIBRARY_IUSHL3DNONPARAMETRICSOURCE_H

#include "iusPosition.h"

// ADT
typedef struct Ius3DNonParametricSource Ius3DNonParametricSource;
/** The 3D Non-Parametric Sources describes a list of locations from which a pulse originates simulatanously. You can
  * create a source with a predefined number of locations and the get and set these locations with
  * ius3DNonParametricSourceGetPosition() and ius3DNonParametricSourceGetPosition() respectively. */
typedef Ius3DNonParametricSource *iu3dnps_t;
#define  IU3DNPS_INVALID (iu3dnps_t) NULL

/** An invalid 3D Non-Parametric Source */
iu3dnps_t ius3DNonParametricSourceCreate
(
    int numLocations						///< The number of points the source has.
);

/** \brief Delete a 3D non-parametric source.
 * \return Returns #IUS_E_OK on success or #IUS_ERR_VALUE in case of an error.
 */
int ius3DNonParametricSourceDelete
(
    iu3dnps_t ius3DNonParametricSource		///< The source that is deleted
);

/** \brief Compares two 3D non-parametric sources.
 * \return #IUS_TRUE if reference and actual are identical sources, and #IUS_FALSE in all other cases.
 */
int ius3DNonParametricSourceCompare
(
    iu3dnps_t reference,					///< 3D non-parametric source to compare
    iu3dnps_t actual						///< 2D non-parametric source to compare with
);

/** \brief Gets the \p index -th 3D position of a 3D non-parametric source.
 * \return The 3D position or #IU3DP_INVALID if that position does not exist.
 */
iu3dp_t ius3DNonParametricSourceGetPosition
(
    iu3dnps_t ius3DNonParametricSource,		///< 3D non-parametric source with positions
    int index								///< the index of the position list 
);

/** \brief Sets the 3D position \p pos on the \p index -th location for the 3D non-parametric source \p ius3DNonParametricSource.
 * \return Returns #IUS_E_OK on success or #IUS_ERR_VALUE in case of an error.
 */
int ius3DNonParametricSourceSetPosition
(
    iu3dnps_t ius3DNonParametricSource,		///< the 3D non-parametric source
    iu3dp_t  pos,							///< the 3D position to be set 
    int index								///< the index of the position
);

#endif //IUSLIBRARY_IUSHL3DNONPARAMETRICSOURCE_H
