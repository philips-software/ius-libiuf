#ifndef IUFLIBRARY_IUFHL2DSIZE_H
#define IUFLIBRARY_IUFHL2DSIZE_H

// ADT
typedef struct Iuf2DSize
{
	float sx;
	float sz;
} Iuf2DSize;

/** \brief A 2D size in meters. First dimension is x (lateral), second dimension is z (depth) */
typedef Iuf2DSize *iu2ds_t;
#define  IU2DS_INVALID (iu2ds_t) NULL

/** \brief Allocate a 2D size */
iu2ds_t iuf2DSizeCreate
(
  float sx, ///< the size in the x-direction in meters 
  float sz  ///< the size in the z-direction in meters
);

/** \brief Delete a 2D size object */
int iuf2DSizeDelete
(
    iu2ds_t size  ///< THe object that will be deleted
);


/** \brief Delete a 2D size object */
int iuf2DSizeCompare
(
    iu2ds_t reference, ///< one of the sizes to compare 
    iu2ds_t actual     ///<the other of the sizes to compare
);

#endif //IUFLIBRARY_IUFHL2DSIZE_H
