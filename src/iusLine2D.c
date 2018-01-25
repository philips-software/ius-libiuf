//=============================================================================
//    COPYRIGHT 2015 PHILIPS RESEARCH
//=============================================================================
//
//  Source Name   : iusLine2D.c
//                  Basic functions for 2D lines
//  iusVersion    : 255.255.255.255
//
//=============================================================================

#include "iusLine2D.h"
#include "iusError.h"
#include <math.h>      // sqrt, fabs, ..

//=============================================================================
// L O C A L   D E F I N E S
//=============================================================================

//=============================================================================
// S T A T I C   F U N C T I O N   P R O T O T Y P E S
//=============================================================================

//=============================================================================
// E X P O R T E D   F U N C T I O N S
//=============================================================================

//-----------------------------------------------------------------------------
// initializes a line
//-----------------------------------------------------------------------------
Line2DFloat iusLine_InitNP
(
    const Coord2DFloat n, /* normal of line */
    const Coord2DFloat p  /* point on line */
)
{
    Line2DFloat l;
    float t;

    /* l.n = unity length n */
    l.n = n;
    t = (float)sqrt( n.x * n.x + n.y * n.y );
    l.n.x /= t;
    l.n.y /= t;

    /* l.d */
    l.d = - ( p.x * l.n.x + p.y * l.n.y );
    return l;
}

//-----------------------------------------------------------------------------
// initializes a line
//-----------------------------------------------------------------------------
Line2DFloat iusLine_InitDB
(
    const Coord2DFloat d, /* direction of line */
    const Coord2DFloat b  /* base of line */
)
{
    Coord2DFloat n;

    n.x = - d.y; /* orthogonal */
    n.y =   d.x; /* orthogonal */
    return iusLine_InitNP( n, b );
}

//-----------------------------------------------------------------------------
// initializes a line
//-----------------------------------------------------------------------------
Line2DFloat iusLine_InitPP
(
    const Coord2DFloat p1,
    const Coord2DFloat p2
)
{
    Coord2DFloat d;

    d.x = p2.x - p1.x;
    d.y = p2.y - p1.y;
    return iusLine_InitDB( d, p1 );
}

//-----------------------------------------------------------------------------
// intersects lines
//-----------------------------------------------------------------------------
int iusLine_Intersect
(
    const Line2DFloat l,
    const Coord2DFloat d, /* direction of line */
    const Coord2DFloat b, /* base */
    Coord2DFloat * const rv 
)
{
    float dn, t;
    Coord2DFloat s;

    dn = d.x * l.n.x + d.y * l.n.y;
    if( fabs( dn ) < 1e-10f )
        return 0;

    t = - ( ( b.x * l.n.x + b.y * l.n.y + l.d ) / dn );
    s.x = b.x + d.x * t;
    s.y = b.y + d.y * t;
    *rv = s;
    return 1;
}

//-----------------------------------------------------------------------------
// distance between line and point
//-----------------------------------------------------------------------------
float iusLine_Distance
(
    const Line2DFloat l,
    const Coord2DFloat p
)
{
    return l.n.x * p.x + l.n.y * p.y + l.d;
}

//-----------------------------------------------------------------------------
// classify location of a point wrt line
//-----------------------------------------------------------------------------
iusGeoClass iusLine_Side
(
    const Line2DFloat l,
    const Coord2DFloat p
)
{
    const float d = iusLine_Distance( l, p );

	if( d > 1e-10f ) return iusGeoOUT;
	else if( d < - 1e-10f ) return iusGeoIN;
	else return iusGeoON;
}
