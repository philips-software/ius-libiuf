//===========================================================================*/
//     COPYRIGHT 2016 PHILIPS RESEARCH
//===========================================================================*/
//
//  Source Name   : iusPosition.h
//                  Position type
//  iusVersion    : 255.255.255.255
//  author        : Frank van Heesch, Ben Bierens
//
//==============================================================================

#ifndef _IUSPOSITION_H
#define _IUSPOSITION_H

#ifdef __cplusplus
extern "C" {
#endif

#include "base/iusTypes.h"

/**
 * \brief BMode internal data structure
 */
typedef struct
{
	char	  ID[20];			  /**< GUID fixed string identifier            */
	char	  type[20];           /**< Fixed length type of data               */
    int       numberOfParents;    /**< number of parents thta this file has    */

    int 	    numPositions;                     /**< The number of positions (with timestamp and confusion that are in this file */
    IUSPosition *pPosition;               /**< The position array eather in cartesian or polar coordinates depending on coordinateSystemType */
    IUSSize     *pConfusion;              /**< The size of the area of confusion in the same coordinate system as the position */
    float       *pTimeStamp;              /**< timstamps of the position recordings */
    enum IUSCoordinateSystemType coordinateSystemType; /**< are the positions in e.g. polar of cartesian coordinates? */
	
	void      *parents[16];		  /**< array of 16 parent structures? */
} iusPositionInstance;

/** and a create function */


/**
 * \brief Frees all Position resources
 *
 * In-place:   n.a.
 *
 * \param[in]  pInst: pointer to instance
 * \return     void
 */
void iusPositionDestroy
(
    iusPositionInstance * pInst
);

#ifdef __cplusplus
}
#endif

#endif  /* _IUSBMODE_H */