#ifndef IUFLIBRARY_IUFHL3DNONPARAMETRICSOURCE_H
#define IUFLIBRARY_IUFHL3DNONPARAMETRICSOURCE_H

#include "iufPosition.h"

// ADT
typedef struct Iuf3DNonParametricSource Iuf3DNonParametricSource;
/** The 3D Non-Parametric Sources describes a list of locations from which a pulse originates simulatanously. You can
  * create a source with a predefined number of locations and the get and set these locations with
  * iuf3DNonParametricSourceGetPosition() and iuf3DNonParametricSourceGetPosition() respectively. */
typedef Iuf3DNonParametricSource *iu3dnps_t;
#define  IU3DNPS_INVALID (iu3dnps_t) NULL

/** An invalid 3D Non-Parametric Source */
iu3dnps_t iuf3DNonParametricSourceCreate
(
    int numLocations						///< The number of points the source has.
);

/** \brief Delete a 3D non-parametric source.
 * \return Returns #IUF_E_OK on success or #IUF_ERR_VALUE in case of an error.
 */
int iuf3DNonParametricSourceDelete
(
    iu3dnps_t source		///< The source that is deleted
);

/** \brief Compares two 3D non-parametric sources.
 * \return #IUF_TRUE if reference and actual are identical sources, and #IUF_FALSE in all other cases.
 */
int iuf3DNonParametricSourceCompare
(
    iu3dnps_t reference,					///< 3D non-parametric source to compare
    iu3dnps_t actual						///< 2D non-parametric source to compare with
);

/** \brief Gets the \p index -th 3D position of a 3D non-parametric source.
 * \return The 3D position or #IU3DP_INVALID if that position does not exist.
 */
iu3dp_t iuf3DNonParametricSourceGetPosition
(
    iu3dnps_t source,		///< 3D non-parametric source with positions
    int index								///< the index of the position list 
);

/** \brief  Gets he number of points the source has.
 * \return  The number of points the source has.
 */
int iuf3DNonParametricSourceGetNumLocations
(
    iu3dnps_t source     ///< 2D non-parametric source with positions
);

/** \brief Sets the 3D position \p pos on the \p index -th location for the 3D non-parametric source \p iuf3DNonParametricSource.
 * \return Returns #IUF_E_OK on success or #IUF_ERR_VALUE in case of an error.
 */
int iuf3DNonParametricSourceSetPosition
(
    iu3dnps_t source,		///< the 3D non-parametric source
    iu3dp_t  pos,							///< the 3D position to be set 
    int index								///< the index of the position
);

#endif //IUFLIBRARY_IUFHL3DNONPARAMETRICSOURCE_H
