/**  
 * \file iuf2DNonParametricSource.h
 * \brief File containing iuf2DNonParametricSource related declarations.
 */
#ifndef IUFLIBRARY_IUFHL2DNONPARAMETRICSOURCE_H
#define IUFLIBRARY_IUFHL2DNONPARAMETRICSOURCE_H

#include "iufPosition.h"

typedef struct Iuf2DNonParametricSource Iuf2DNonParametricSource;
/** The 2D Non-Parametric Sources describes a list of locations from which a pulse originates simulatanously. You can 
  * create a source with a predefined number of locations and then get and set these locations with 
  * iuf2DNonParametricSourceGetPosition() and iuf2DNonParametricSourceGetPosition() respectively. */
typedef Iuf2DNonParametricSource *iu2dnps_t;

/** An invalid 2D Non-Parametric Source */
#define  IU2DNPS_INVALID (iu2dnps_t) NULL

/** \brief Create an ultrasound source for generating a pulseform based on \p numLocations points. 
 * The points are described directly by their 2D position (i.e. not parametrically) .
 * \return returns the created 2D non-parameric source.
 */
iu2dnps_t iuf2DNonParametricSourceCreate
(
    int numLocations						///< The number of points the source has.
);

/** \brief Delete a 2D non-parametric source.
 * \return Returns #IUF_E_OK on success or #IUF_ERR_VALUE in case of an error.
 */
int iuf2DNonParametricSourceDelete
(
    iu2dnps_t source      ///< The source that is deleted
);

/** \brief Compares two 2D non-parametric sources.
 * \return #IUF_TRUE if \p reference and \p actual are identical sources, and #IUF_FALSE in all other cases.
 */
int iuf2DNonParametricSourceCompare
(
    iu2dnps_t reference,                    ///< 2D non-parametric source to compare
    iu2dnps_t actual                        ///< 2D non-parametric source to compare with
);

/** \brief Gets the \p index -th 2D position of a 2D non-parametric source.
 * \return The 2D position or #IU2DP_INVALID if that position does not exist.
 */
iu2dp_t iuf2DNonParametricSourceGetPosition
(
    iu2dnps_t source,     ///< 2D non-parametric source with positions
    int index							    ///< the index of the position list 
);

/** \brief  Gets he number of points the source has.
 * \return  The number of points the source has.
 */
int iuf2DNonParametricSourceGetNumLocations
(
    iu2dnps_t source     ///< 2D non-parametric source with positions
);


/** \brief sets the 2D position \p pos on the \p index -th location for the 2D non-parametric source \p iuf2DNonParametricSource.
 * \return Returns #IUF_E_OK on success or #IUF_ERR_VALUE in case of an error.
 */
int iuf2DNonParametricSourceSetPosition
(
    iu2dnps_t source,		///< the 2D non-parametric source
    iu2dp_t  pos,                           ///< the 2D position to be set 
    int index                               ///< the index of the position
);

#endif //IUFLIBRARY_IUFHL2DNONPARAMETRICSOURCE_H

