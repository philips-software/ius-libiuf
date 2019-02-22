// Created by nlv09165 on 24/05/2018.
#ifndef IUSLIBRARY_IUSHL2DSIZE_H
#define IUSLIBRARY_IUSHL2DSIZE_H

// ADT
typedef struct Ius2DSize
{
	float sx;
	float sz;
} Ius2DSize;

/** \brief A 2D size in meters. First dimension is x (lateral), second dimension is z (depth) */
typedef Ius2DSize *iu2ds_t;
#define  IU2DS_INVALID (iu2ds_t) NULL

/** \brief Allocate a 2D size */
iu2ds_t ius2DSizeCreate
(
  float sx, ///< the size in the x-direction in meters 
  float sz  ///< the size in the z-direction in meters
);

/** \brief get the X component of the 2D Size */
float ius2DSizeGetX
(
   iu2ds_t size
);

/** \brief get the Z component of the 2D Size */
float ius2DSizeGetZ
(
   iu2ds_t size
);

/** \brief Delete a 2D size object */
int ius2DSizeDelete
(
    iu2ds_t size  ///< THe object that will be deleted
);


/** \brief Delete a 2D size object */
int ius2DSizeCompare
(
    iu2ds_t reference, ///< one of the sizes to compare 
    iu2ds_t actual     ///<the other of the sizes to compare
);

#endif //IUSLIBRARY_IUSHL2DSIZE_H
