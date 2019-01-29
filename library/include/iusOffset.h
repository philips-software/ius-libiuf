
// Created by nlv09165 on 05/10/2018.
#ifndef IUSLIBRARY_IUSOFFSET_H
#define IUSLIBRARY_IUSOFFSET_H

// ADT
typedef struct IusOffset_
{
    int x ;
    int y ;
    int z ;
    int t ;
} IusOffset;

typedef IusOffset *iuo_t;
/* Data is stored in rectangular blocks up to 4 dimensions. The offset structure points to 
 * a certain {x,y,z,t} point in this data block
 * */
#define  IUO_INVALID (iuo_t) NULL

/** \brief Create an offset object initialized at {0,0,0,0}
 * \return Returns a zero offset object.
 */
iuo_t iusOffsetCreate
(
    void
);

/** \brief Delete an offset object 
 * \return Returns #IUS_E_OK in when successful or IUS_ERR_VALUE when the offset is NULL 
 */
int iusOffsetDelete
(
    iuo_t offset  ///< the offset that is to be deleted.
);

/** \brief Compare two offsets with each other 
 * \return Returns #IUS_TRUE when the offsets are equal and IUS_FALSE otherwise. 
*/
int iusOffsetCompare
(
    iuo_t reference,  ///< The offset to compare to
    iuo_t actual      ///< The offset to compare with
);


#endif //IUSLIBRARY_IUSOFFSET_H
