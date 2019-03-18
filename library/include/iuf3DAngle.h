// Created by nlv09165 on 24/05/2018.
#ifndef IUFLIBRARY_IUFHL3DANGLE_H
#define IUFLIBRARY_IUFHL3DANGLE_H

// ADT
typedef struct Iuf3DAngle
{
	float theta; ///< rotation around y axis */
	float phi;   ///< rotation round x axis */
} Iuf3DAngle;

/** \brief 3D elements are considered block shaped and aligned (z-angle rotation=0) and can rotate in theta (y-axis) and phi (over the x-axis) 
* angles are in radians
*/
typedef Iuf3DAngle *iu3da_t;
#define  IU3DA_INVALID (iu3da_t) NULL

/** \brief create a 2-axis rotation in 3D space  
 * \return returns an allocated and initialized 3D angle
 */
iu3da_t iuf3DAngleCreate
(
    float theta, ///< the rotation over the y-axis in radians
    float phi    ///< the rotation over the x-axis in radians
);

/** \brief Delete a 3D angle
* 
* \return returns #IUF_ERR_VALUE in case this fails or #IUF_E_OK in case of success
 */
int iuf3DAngleDelete
(
    iu3da_t angle ///< the angle to be removed
);

/** \brief Compare two 3D angles, the order of arguments does not matter
*
* \return returns #IUF_TRUE in case the angles are equal or #IUF_FALSE in case they differ.
 */
int iuf3DAngleCompare
(
    iu3da_t reference,  ///< a 3D angle to compare
    iu3da_t actual      ///< a 3D angle to compare with
);

#endif //IUFLIBRARY_IUFHL3DANGLE_H
