// Created by nlv09165 on 24/05/2018.
#ifndef IUSLIBRARY_IUSHL3DSIZE_H
#define IUSLIBRARY_IUSHL3DSIZE_H

// ADT
typedef struct Ius3DSize
{
	float sx; ///< Size in x dimension in m
	float sy; ///, Size in y dimension in m
	float sz; ///< Size in z dimension in m
} Ius3DSize;

/** \brief 3D elements are considered block shaped and aligned (z-angle rotation=0) and can rotate in theta (y-axis) and phi (over the x-axis)
* angles are in radians and have a position [x,y,z] in meters and a size in [sx, sy, sz] in meters.
*/
typedef Ius3DSize *iu3ds_t;
#define  IU3DS_INVALID (iu3ds_t) NULL

/** \brief create a 3-axis size 3D space
* \return returns an allocated and initialized 3D size.
 */
iu3ds_t ius3DSizeCreate
(
  float sx, ///< size in x direction in m
  float sy, ///< size in y direction in m
  float sz  ///< size in z direction in m
);

/** \brief delete a 3-axis size object
 * \return returns #IUS_ERR_VALUE in case this fails or #IUS_E_OK in case of success
 */
int ius3DSizeDelete
(
    iu3ds_t size ///< the size object to be deleted
);

/** \brief Compare two 3D sizes, argument order doesn't matter.
* \return returns #IUS_TRUE in case the angles are equal or #IUS_FALSE in case they differ.
 */
int ius3DSizeCompare
(
    iu3ds_t reference, ///< a 3D size to compare to
    iu3ds_t actual     ///< a 3D size to compare with
);

/** \brief Get the X component of the 3D size */
float ius3DSizeGetX
(
    iu3ds_t size       ///< the 3D size of interest
);
/** \brief Get the Y component of the 3D size */
float ius3DSizeGetY
(
    iu3ds_t size       ///< the 3D size of interest
);
/** \brief Get the Z component of the 3D size */
float ius3DSizeGetZ
(
    iu3ds_t size       ///< the 3D size of interest
);




#endif //IUSLIBRARY_IUSHL3DSIZE_H
