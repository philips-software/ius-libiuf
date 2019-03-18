//
// Created by Ruijzendaal on 28/03/2018.
//
#ifndef IUFLIBRARY_IUFHLPOSITION_H
#define IUFLIBRARY_IUFHLPOSITION_H

#include <iufTypes.h>

// ADT
struct   Iuf3DPosition;
typedef  struct Iuf3DPosition Iuf3DPosition;
/** a 3D position has an x, y and z, while a 2D position has an x and z only. x is along the transducer, 
 * z is the depth direction, and y is perpendicular to the transducer array */
typedef  Iuf3DPosition    * iu3dp_t;
#define  IU3DP_INVALID (iu3dp_t) NULL

typedef  struct Iuf2DPosition Iuf2DPosition;
typedef  Iuf2DPosition    * iu2dp_t;
#define  IU2DP_INVALID (iu2dp_t) NULL

/** \brief Create a 2D position (x,z) 
 * \return Returns (x,z).
 */
iu2dp_t iuf2DPositionCreate
(
    float x, ///< along the transducer
    float z  ///< depth direction
);

/** \brief Create a 3D position (x,y,z) 
 * \return Returns (x,y,z).
 */
iu3dp_t iuf3DPositionCreate
(
    float x, // along the transducer elements
    float y, // perpendicular to the transducer elements
    float z  // depth direction
);

/** \brief Delete a 3D position */
void iuf3DPositionDelete
(
    iu3dp_t pos ///< The position of interest
);

/** \brief Delete a 3D position */
void iuf2DPositionDelete
(
    iu2dp_t pos ///< The position of interest
);

/** \brief compare two 2D positions with each other
 * \return Return #IUF_TRUE when the positions are equal and #IUF_FALSE otherwise
*/
IUF_BOOL iuf2DPositionCompare
(
    iu2dp_t reference, ///< The position two compare to
    iu2dp_t actual     ///< The position two compare with
);

/** \brief compare two 3D positions with each other
 * \return Return #IUF_TRUE when the positions are equal and #IUF_FALSE otherwise
*/
IUF_BOOL iuf3DPositionCompare
(
    iu3dp_t reference, ///< The position two compare to
    iu3dp_t actual     ///< The position two compare with
);

#endif //IUFLIBRARY_IUFHLPOSITION_H
