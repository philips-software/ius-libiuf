// Created by nlv09165 on 24/05/2018.
#ifndef IUFLIBRARY_IUFHL3DSIZE_H
#define IUFLIBRARY_IUFHL3DSIZE_H

// ADT
typedef struct Iuf3DSize
{
	float sx; ///< Size in x dimension in m
	float sy; ///, Size in y dimension in m
	float sz; ///< Size in z dimension in m
} Iuf3DSize;

/** \brief 3D elements are considered block shaped and aligned (z-angle rotation=0) and can rotate in theta (y-axis) and phi (over the x-axis)
* angles are in radians and have a position [x,y,z] in meters and a size in [sx, sy, sz] in meters.
*/
typedef Iuf3DSize *iu3ds_t;
#define  IU3DS_INVALID (iu3ds_t) NULL

/** \brief create a 3-axis size 3D space
* \return returns an allocated and initialized 3D size.
 */
iu3ds_t iuf3DSizeCreate
(
  float sx, ///< size in x direction in m
  float sy, ///< size in y direction in m
  float sz  ///< size in z direction in m
);

/** \brief delete a 3-axis size object
 * \return returns #IUF_ERR_VALUE in case this fails or #IUF_E_OK in case of success
 */
int iuf3DSizeDelete
(
    iu3ds_t size ///< the size object to be deleted
);

/** \brief Compare two 3D sizes, argument order doesn't matter.
* \return returns #IUF_TRUE in case the angles are equal or #IUF_FALSE in case they differ.
 */
int iuf3DSizeCompare
(
    iu3ds_t reference, ///< a 3D size to compare to
    iu3ds_t actual     ///< a 3D size to compare with
);
#endif //IUFLIBRARY_IUFHL3DSIZE_H
