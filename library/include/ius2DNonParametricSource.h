/** ==============================================================================
 *    COPYRIGHT 2018 PHILIPS RESEARCH
 *  ==============================================================================
 *
 * \file ius2DNonParametricSource.h
 * \brief File containing ius2DNonParametricSource related declarations.
 *
 *  ==============================================================================
 */
#ifndef IUSLIBRARY_IUSHL2DNONPARAMETRICSOURCE_H
#define IUSLIBRARY_IUSHL2DNONPARAMETRICSOURCE_H

#include "iusPosition.h"

typedef struct Ius2DNonParametricSource Ius2DNonParametricSource;
/** The 2D Non-Parametric Sources describes a list of locations from which a pulse originates simulatanously. You can 
  * create a source with a predefined number of locations and then get and set these locations with 
  * ius2DNonParametricSourceGetPosition() and ius2DNonParametricSourceGetPosition() respectively. */
typedef Ius2DNonParametricSource *iu2dnps_t;

/** An invalid 2D Non-Parametric Source */
#define  IU2DNPS_INVALID (iu2dnps_t) NULL

/** \brief Create an ultrasound source for generating a pulseform based on \p numLocations points. 
 * The points are described directly by their 2D position (i.e. not parametrically) .
 * \return returns the created 2D non-parameric source.
 */
iu2dnps_t ius2DNonParametricSourceCreate
(
    int numLocations						///< The number of points the source has.
);

/** \brief Delete a 2D non-parametric source.
 * \return Returns #IUS_E_OK on success or #IUS_ERR_VALUE in case of an error.
 */
int ius2DNonParametricSourceDelete
(
    iu2dnps_t source      ///< The source that is deleted
);

/** \brief Compares two 2D non-parametric sources.
 * \return #IUS_TRUE if \p reference and \p actual are identical sources, and #IUS_FALSE in all other cases.
 */
int ius2DNonParametricSourceCompare
(
    iu2dnps_t reference,                    ///< 2D non-parametric source to compare
    iu2dnps_t actual                        ///< 2D non-parametric source to compare with
);

/** \brief Gets the \p index -th 2D position of a 2D non-parametric source.
 * \return The 2D position or #IU2DP_INVALID if that position does not exist.
 */
iu2dp_t ius2DNonParametricSourceGetPosition
(
    iu2dnps_t source,     ///< 2D non-parametric source with positions
    int index							    ///< the index of the position list 
);

/** \brief sets the 2D position \p pos on the \p index -th location for the 2D non-parametric source \p ius2DNonParametricSource. 
 * \return Returns #IUS_E_OK on success or #IUS_ERR_VALUE in case of an error.
 */
int ius2DNonParametricSourceSetPosition
(
    iu2dnps_t source,		///< the 2D non-parametric source
    iu2dp_t  pos,                           ///< the 2D position to be set 
    int index                               ///< the index of the position
);

#endif //IUSLIBRARY_IUSHL2DNONPARAMETRICSOURCE_H

