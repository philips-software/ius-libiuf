// Created by nlv09165 on 24/05/2018.
#ifndef IUSLIBRARY_IUSHL3DANGLE_H
#define IUSLIBRARY_IUSHL3DANGLE_H

// ADT
typedef struct Ius3DAngle
{
	float theta; ///< rotation around y axis */
	float phi;   ///< rotation round x axis */
} Ius3DAngle;

/** \brief 3D elements are considered block shaped and aligned (z-angle rotation=0) and can rotate in theta (y-axis) and phi (over the x-axis) 
* angles are in radians
*/
typedef Ius3DAngle *iu3da_t;
#define  IU3DA_INVALID (iu3da_t) NULL

/** \brief create a 2-axis rotation in 3D space  
 * \return returns an allocated and initialized 3D angle
 */
iu3da_t ius3DAngleCreate
(
    float theta, ///< the rotation over the y-axis in radians
    float phi    ///< the rotation over the x-axis in radians
);

/** \brief Delete a 3D angle
* 
* \return returns #IUS_ERR_VALUE in case this fails or #IUS_E_OK in case of success
 */
int ius3DAngleDelete
(
    iu3da_t angle ///< the angle to be removed
);

/** \brief Compare two 3D angles, the order of arguments does not matter
*
* \return returns #IUS_TRUE in case the angles are equal or #IUS_FALSE in case they differ.
 */
int ius3DAngleCompare
(
    iu3da_t reference,  ///< a 3D angle to compare
    iu3da_t actual      ///< a 3D angle to compare with
);

/** \brief return theta component of 3D angle */
float ius3DAngleGetTheta
(
     iu3da_t angle      ///< The 3D angle of interest
);

/** \brief return theta component of 3D angle */
float ius3DAngleGetPhi
(
     iu3da_t phi        ///< The 3D angle of interest
);

#endif //IUSLIBRARY_IUSHL3DANGLE_H
