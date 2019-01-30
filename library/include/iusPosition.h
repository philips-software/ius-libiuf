//
// Created by Ruijzendaal on 28/03/2018.
//
#ifndef IUSLIBRARY_IUSHLPOSITION_H
#define IUSLIBRARY_IUSHLPOSITION_H

#include <iusTypes.h>

// ADT
struct   Ius3DPosition;
typedef  struct Ius3DPosition Ius3DPosition;
/** a 3D position has an x, y and z, while a 2D position has an x and z only. x is along the transducer, 
 * z is the depth direction, and y is perpendicular to the transducer array */
typedef  Ius3DPosition    * iu3dp_t;
#define  IU3DP_INVALID (iu3dp_t) NULL

typedef  struct Ius2DPosition Ius2DPosition;
typedef  Ius2DPosition    * iu2dp_t;
#define  IU2DP_INVALID (iu2dp_t) NULL

/** \brief Create a 2D position (x,z) 
 * \return Returns (x,z).
 */
iu2dp_t ius2DPositionCreate
(
    float x, ///< along the transducer
    float z  ///< depth direction
);

/** \brief Create a 3D position (x,y,z) 
 * \return Returns (x,y,z).
 */
iu3dp_t ius3DPositionCreate
(
    float x, // along the transducer elements
    float y, // perpendicular to the transducer elements
    float z  // depth direction
);

/** \brief Delete a 3D position */
void ius3DPositionDelete
(
    iu3dp_t pos ///< The position of interest
);

/** \brief Delete a 3D position */
void ius2DPositionDelete
(
    iu2dp_t pos ///< The position of interest
);

/** \brief compare two 2D positions with each other
 * \return Return #IUS_TRUE when the positions are equal and #IUS_FALSE otherwise
*/
IUS_BOOL ius2DPositionCompare
(
    iu2dp_t reference, ///< The position two compare to
    iu2dp_t actual     ///< The position two compare with
);

/** \brief compare two 3D positions with each other
 * \return Return #IUS_TRUE when the positions are equal and #IUS_FALSE otherwise
*/
IUS_BOOL ius3DPositionCompare
(
    iu3dp_t reference, ///< The position two compare to
    iu3dp_t actual     ///< The position two compare with
);

#endif //IUSLIBRARY_IUSHLPOSITION_H
