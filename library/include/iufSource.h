// Created by nlv09165 on 24/07/2018.
#ifndef IUFLIBRARY_IUFHLSOURCE_H
#define IUFLIBRARY_IUFHLSOURCE_H

typedef enum {
  IUF_INVALID_SOURCE_TYPE = 0,
  IUF_2D_NON_PARAMETRIC_SOURCE,
  IUF_3D_NON_PARAMETRIC_SOURCE,
  IUF_2D_PARAMETRIC_SOURCE,
  IUF_3D_PARAMETRIC_SOURCE
} IufSourceType;

// ADT
typedef struct IufSource IufSource;
/** A source consists of points that simultaneously emit. These points can be in 2D or 3D and can have either a 
 * parametric description of their locations or a x,(y),z list. To create a source use the explicit functions 
 * #iuf2DParametricSourceCreate(), #iuf3DParametricSourceCreate(), #iuf2DNonParametricSourceCreate(), #iuf3DNonParametricSourceCreate()
 */
typedef IufSource *ius_t;
#define  IUS_INVALID (ius_t) NULL

/** \brief Delete a source 
 * \return Returns #IUF_E_OK in case of success and #IUF_ERR_VALUE in case of an error.
*/
int iufSourceDelete
(
    ius_t source  ///< The source that is deleted
);

/** \brief Compare two sources 
 * \return Returns #IUF_TRUE in case the sources are equal and #IUF_FALSE otherwise.
*/
int iufSourceCompare
(
    ius_t reference,   ///< The source to compare to
    ius_t actual       ///< The source to compare with
);

/** \brief Get which type the source has 
 * \return Returns if the source is 2D, 3D and Parametric or non Parametric 
*/
IufSourceType iufSourceGetType
(
    ius_t source ///< The source of interest
);

#endif //IUFLIBRARY_IUFHLSOURCE_H
