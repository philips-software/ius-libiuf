// Created by nlv09165 on 24/07/2018.
#ifndef IUSLIBRARY_IUSHLSOURCE_H
#define IUSLIBRARY_IUSHLSOURCE_H

typedef enum {
  IUS_INVALID_SOURCE_TYPE = 0,
  IUS_2D_NON_PARAMETRIC_SOURCE,
  IUS_3D_NON_PARAMETRIC_SOURCE,
  IUS_2D_PARAMETRIC_SOURCE,
  IUS_3D_PARAMETRIC_SOURCE
} IusSourceType;

// ADT
typedef struct IusSource IusSource;
/** A source consists of points that simultaneously emit. These points can be in 2D or 3D and can have either a 
 * parametric description of their locations or a x,(y),z list. To create a source use the explicit functions 
 * #ius2DParametricSourceCreate(), #ius3DParametricSourceCreate(), #ius2DNonParametricSourceCreate(), #ius3DNonParametricSourceCreate() 
 */
typedef IusSource *ius_t;
#define  IUS_INVALID (ius_t) NULL

/** \brief Delete a source 
 * \return Returns #IUS_E_OK in case of success and #IUS_ERR_VALUE in case of an error.
*/
int iusSourceDelete
(
    ius_t source  ///< The source that is deleted
);

/** \brief Compare two sources 
 * \return Returns #IUS_TRUE in case the sources are equal and #IUS_FALSE otherwise.
*/
int iusSourceCompare
(
    ius_t reference,   ///< The source to compare to 
    ius_t actual       ///< The source to compare with
);

/** \brief Get which type the source has 
 * \return Returns if the source is 2D, 3D and Parametric or non Parametric 
*/
IusSourceType iusSourceGetType
(
    ius_t iusSource ///< The source of interest
);

#endif //IUSLIBRARY_IUSHLSOURCE_H
