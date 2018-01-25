//=============================================================================
//    COPYRIGHT 2015 PHILIPS RESEARCH
//=============================================================================
//
//  Source Name   : iusLine2D.h
//                  Basic geometric functions for 2D lines.
//  iusVersion    : 255.255.255.255
//
//=============================================================================

/** \file
  \brief Basic geometric functions for 2D lines.

 This file contains prototypes for the following functions. More explanation
 is given with the function prototypes below.

 Operations on lines
     Intersect

*/

#ifndef _IUSLINE2D_H
#define _IUSLINE2D_H

#ifdef __cplusplus
extern "C" {
#endif

//=============================================================================
// M A C R O S
//=============================================================================

//=============================================================================
// D E F I N E S
//=============================================================================

//=============================================================================
// D A T A   T Y P E S
//=============================================================================
typedef struct /* from adVSR */
{
    float x;
    float y;
} Coord2DFloat;

typedef struct
{
    Coord2DFloat n; /* unity normal vector of line (orthogonal to line) */
    float        d; /* d */
} Line2DFloat;

typedef enum
{
    iusGeoON,   //on
    iusGeoIN,   //inside
    iusGeoOUT   //outside
} iusGeoClass;

//=============================================================================
// F U N C T I O N   P R O T O T Y P E S
//=============================================================================

//-----------------------------------------------------------------------------
// Initializes a line2d structure.
//
// In-place:   n.a.
// pre         true ?
// post        An initialized line struct is returned.
// param[in]   p1    : point on the line (<>p2)
// param[in]   p2    : point on the line (<>p1)
// return      line
//-----------------------------------------------------------------------------
Line2DFloat iusLine_InitPP
(
    const Coord2DFloat p1,
    const Coord2DFloat p2
);

//-----------------------------------------------------------------------------
// Initializes a line2d structure.
//
// In-place:   n.a.
// pre         true ?
// post        An initialized line struct is returned.
// param[in]   n     : normal vector of the line (length > 0)
// param[in]   p     : point on the line
// return      line
//-----------------------------------------------------------------------------
Line2DFloat iusLine_InitNP
(
    const Coord2DFloat n,
    const Coord2DFloat p
);

//-----------------------------------------------------------------------------
// Initializes a line2d structure.
//
// In-place:   n.a.
// pre         true ?
// post        An initialized line struct is returned.
// param[in]   d     : direction of the line (length > 0)
// param[in]   b     : base point of the line
// return      line
//-----------------------------------------------------------------------------
Line2DFloat iusLine_InitDB
(
    const Coord2DFloat d,
    const Coord2DFloat b
);

//-----------------------------------------------------------------------------
// Calculates intersection point of two lines.
//
// In-place:   n.a.
// pre         The lines were initialized.
// post        Point of intersection was calculated.
// param[in]   l    : first line
// param[in]   d    : direction of second line
// param[in]   b    : base of second line
// param[out]  rv   : intersection point
// return      int  : zero if lines not intersecting, or lines overlapping.
//-----------------------------------------------------------------------------
int iusLine_Intersect
(
    const Line2DFloat l,
    const Coord2DFloat d,
    const Coord2DFloat b,
    Coord2DFloat * const rv 
);

//-----------------------------------------------------------------------------
// Calculates distance from point to line.
//
// In-place:   n.a.
// pre         The line was initialized.
// post        Distance from point to line was calculated.
// param[in]   l     : line
// param[in]   p     : point
// return      float : distance
//-----------------------------------------------------------------------------
float iusLine_Distance
(
    const Line2DFloat l,
    const Coord2DFloat p
);

//-----------------------------------------------------------------------------
// Classifies the location of a point with respect to a line.
//
// In-place:   n.a.
// pre         The line was initialized.
// post        Point was classified.
// param[in]   l     : line
// param[in]   p     : point
// return      enum  : class
//-----------------------------------------------------------------------------
iusGeoClass iusLine_Side
(
    const Line2DFloat l,
    const Coord2DFloat p
);

#ifdef __cplusplus
}
#endif

#endif // _IUSLINE2D_H
