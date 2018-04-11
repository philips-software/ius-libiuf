//=============================================================================
//    COPYRIGHT 2016 PHILIPS RESEARCH
//=============================================================================
//
//  Source Name   : iusTypes.h
//                  various ius types.
//  iusVersion    : 255.255.255.255
//
//=============================================================================

/** \file
 * \brief  various types for ius functions
 */
 
#ifndef _IUSTYPES_H
#define _IUSTYPES_H

#ifdef __cplusplus
extern "C" {
#endif

typedef int IUS_BOOL;
#define IUS_TRUE  (1)
#define IUS_FALSE (0)

/** \brief Position in 3D in meters */    
typedef struct
{
    float x; 
    float y;
    float z;
} Ius3DPosition;


/** \brief Position in 3D in meters */
typedef struct
{
    float x;
    float z;
} Ius2DPosition;

/** \brief 3D Angle of rotation theta and phi over z and x axis */
typedef struct 
{
    float theta; /**< rotation over z axis */
    float phi;   /**< rotation over x axis */
} Ius3DAngle;

/** \brief 3D Size (of element) in x,y,z dimensions */
typedef struct 
{
    float sx;
    float sy;
    float sz;
} Ius3DSize;


/** \brief 2D Size (of element) in x,z dimensions */
typedef struct
{
    float sx;
    float sz;
} Ius2DSize;

/** \brief velocity vector in x,y,z dimensions */
typedef struct 
{
    float Vx;
    float Vy;
    float Vz;
} IusVelocity;

//! a line of sampledata is an array of floats
typedef float * IusSampleLine;                 

//! a page of data is an array of #IusSampleLine            
typedef IusSampleLine * IusPage;                    

//! a transmitWave is an array of #IusPage
typedef IusPage * IusTransmitWave;            

//! a frame is an array of #IusTransmitWave
typedef IusTransmitWave * IusFrame;    

//! RFData is an array of #IusFrame               
typedef IusFrame * IusRFData;                 

//! Different type of interpolation when resampling signals  
enum IusInterpolationMethod
{
    IUS_INTERPOLATION_NONE = 0,
    IUS_INTERPOLATION_LINEAR,
    IUS_INTERPOLATION_PREFILTERED_LINEAR,
    IUS_INTERPOLATION_2DSCALING
};

enum IusCoordinateSystemType
{
    IUS_COORDINATESYSTEM_CARTESIAN = 0,
    IUS_COORDINATESYSTEM_POLAR
};

enum IusGridType
{
    IUS_GRID_POLAR = 0,
    IUS_GRID_CARTESIAN,
    IUS_GRID_SPARSE_POLAR,
    IUS_GRID_SPARSE_CARTESIAN
};

//! A 3D grid contains a number of points along each dimension plus the
//! position of these points. According to the IUS convention:
//! - 1st dimension is elevational position (phi or y)
//! - 2nd dimension is lateral position (theta or x)
//! - 3rd dimension is axial posision (r or z)
typedef struct 
{
  int     numPoints0;   /**< Size in 1st dimension              */
  int     numPoints1;   /**< Size in 2nd dimension              */
  int     numPoints2;   /**< Size in 3rd dimension              */
  float * pPoints0;     /**< Location of the values along 1st dimension */
  float * pPoints1;     /**< Location of the values along 2nd dimension */
  float * pPoints2;     /**< Location of the values along 3rd dimension */
} IusGrid;

typedef struct
{
    int startIndex0;
    int startIndex1;
    int startIndex2;
    int numSamples0;
    int numSamples1;
    int numSamples2;
} Ius3dRoi;               // 3D region of interest

typedef struct
{
    int startIndex0;
    int startIndex1;
    int numSamples0;
    int numSamples1;
} Ius2dRoi;               // 2D region of interest

typedef struct
{
    int startIndex;
    int numSamples;
} IusRange;               // 1D region of interest


typedef enum
{
	IUS_INVALID_SHAPE = 0,
    IUS_LINE,           /**< Todo: See Draft */
    IUS_CIRCLE,
    IUS_PLANE,
    IUS_CYLINDER,
    IUS_SPHERE
} IusTransducerShape;

#define TRANSDUCER_SHAPE_LINE "LINE"
#define TRANSDUCER_SHAPE_CIRCLE "CIRCLE"
#define TRANSDUCER_SHAPE_PLANE "PLANE"
#define TRANSDUCER_SHAPE_CYLINDER "CYLINDER"
#define TRANSDUCER_SHAPE_SPHERE "SPHERE"


typedef enum
{
    IUS_2D_SHAPE = 1,
    IUS_3D_SHAPE
} IusShape;


/** \brief Possible driving patterns */
typedef enum
{
    IUS_INVALID_DRIVING_SCHEME,
    IUS_DIVERGING_WAVES_PARAMETRIZED = 1, /**< focus point has negative z value (parametrized) */
    IUS_DIVERGING_WAVES,                 /**< focus point has negative z value */
    IUS_FOCUSED_WAVES_PARAMETRIZED,      /**< focus point has positive z value */
    IUS_FOCUSED_WAVES,       /**< focus point has positive z value */
    IUS_PLANE_WAVES,         /**< neighbouring elements fire consecutively with constant inter-element delay */
    IUS_SINGLE_ELEMENT,      /**< simplified case where there is only one element */
    IUS_CUSTOM_WAVES         /**< non-specified driving scheme category, the transmit pattern and transmit delays of elements can be anything */
} IusDrivingSchemeType ;

#define DRIVINGSCHEME_DIVERGING_WAVES_PARAMETRIZED   "DIVERGING_WAVES_PARAMETRIZED"
#define DRIVINGSCHEME_DIVERGING_WAVES                "DIVERGING_WAVES"
#define DRIVINGSCHEME_FOCUSED_WAVES_PARAMETRIZED     "FOCUSED_WAVES_PARAMETRIZED"
#define DRIVINGSCHEME_FOCUSED_WAVES                  "FOCUSED_WAVES"
#define DRIVINGSCHEME_PLANE_WAVES                    "PLANE_WAVES"
#define DRIVINGSCHEME_SINGLE_ELEMENT                 "SINGLE_ELEMENT"
#define DRIVINGSCHEME_CUSTOM_WAVES                   "CUSTOM_WAVES"

typedef enum {
    IUS_PARAMETRIC_PULSETYPE = 1,
    IUS_NON_PARAMETRIC_PULSETYPE
} IusTransmitPulseType;


#ifdef __cplusplus
}
#endif

#endif  // _IUSRFFILE_H
