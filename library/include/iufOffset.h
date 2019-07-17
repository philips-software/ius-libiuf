
#ifndef IUFLIBRARY_IUFOFFSET_H
#define IUFLIBRARY_IUFOFFSET_H

// ADT
typedef struct IufOffset_
{
    int x ;
    int y ;
    int z ;
    int t ;
} IufOffset;

typedef IufOffset *iuo_t;
/* Data is stored in rectangular blocks up to 4 dimensions. The offset structure points to 
 * a certain {x,y,z,t} point in this data block
 * */
#define  IUO_INVALID (iuo_t) NULL

/** \brief Create an offset object initialized at {0,0,0,0}
 * \return Returns a zero offset object.
 */
iuo_t iufOffsetCreate
(
    void
);

/** \brief Delete an offset object 
 * \return Returns #IUF_E_OK in when successful or IUF_ERR_VALUE when the offset is NULL
 */
int iufOffsetDelete
(
    iuo_t offset  ///< the offset that is to be deleted.
);

/** \brief Compare two offsets with each other 
 * \return Returns #IUF_TRUE when the offsets are equal and IUF_FALSE otherwise.
*/
int iufOffsetCompare
(
    iuo_t reference,  ///< The offset to compare to
    iuo_t actual      ///< The offset to compare with
);


#endif //IUFLIBRARY_IUFOFFSET_H
