#ifndef _IUFTYPES_H
#define _IUFTYPES_H

#ifdef __cplusplus
extern "C" {
#endif

typedef int IUF_BOOL;
#define IUF_TRUE  (1)
#define IUF_FALSE (0)



/** \brief velocity vector in x,y,z dimensions */
typedef struct 
{
    float Vx;
    float Vy;
    float Vz;
} IufVelocity;

//! a line of sampledata is an array of floats
typedef float * IufSampleLine;

//! a page of data is an array of #IufSampleLine
typedef IufSampleLine * IufPage;

//! a transmitWave is an array of #IufPage
typedef IufPage * IufTransmitWave;

//! a frame is an array of #IufTransmitWave
//typedef IufTransmitWave * IufFrame;

//! RFData is an array of #IufFrame
//typedef IufFrame * IufRFData;

//! Different type of interpolation when resampling signals  
enum IufInterpolationMethod
{
    IUF_INTERPOLATION_NONE = 0,
    IUF_INTERPOLATION_LINEAR,
    IUF_INTERPOLATION_PREFILTERED_LINEAR,
    IUF_INTERPOLATION_2DSCALING
};

enum IufCoordinateSystemType
{
    IUF_COORDINATESYSTEM_CARTESIAN = 0,
    IUF_COORDINATESYSTEM_POLAR
};

enum IufGridType
{
    IUF_GRID_POLAR = 0,
    IUF_GRID_CARTESIAN,
    IUF_GRID_SPARSE_POLAR,
    IUF_GRID_SPARSE_CARTESIAN
};

typedef enum 
{
  IUF_IQ_COMPONENT_I = 0,
  IUF_IQ_COMPONENT_Q
} IufComponent;

//! A 3D grid contains a number of points along each dimension plus the
//! position of these points. According to the IUF convention:
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
} IufGrid;

typedef struct
{
    int startIndex0;
    int startIndex1;
    int startIndex2;
    int numSamples0;
    int numSamples1;
    int numSamples2;
} Iuf3dRoi;               // 3D region of interest

typedef struct
{
    int startIndex0;
    int startIndex1;
    int numSamples0;
    int numSamples1;
} Iuf2dRoi;               // 2D region of interest

typedef struct
{
    int startIndex;
    int numSamples;
} IufRange;               // 1D region of interest


typedef enum
{
	IUF_INVALID_TRANSDUCER_SHAPE = 0,
    IUF_LINE,           /**< Todo: See Draft */
    IUF_CIRCLE,
    IUF_PLANE,
    IUF_CYLINDER,
    IUF_SPHERE
} IufTransducerShape;

#define TRANSDUCER_SHAPE_LINE "LINE"
#define TRANSDUCER_SHAPE_CIRCLE "CIRCLE"
#define TRANSDUCER_SHAPE_PLANE "PLANE"
#define TRANSDUCER_SHAPE_CYLINDER "CYLINDER"
#define TRANSDUCER_SHAPE_SPHERE "SPHERE"


typedef enum
{
	IUF_INVALID_SHAPE = 0,
    IUF_2D_SHAPE,
    IUF_3D_SHAPE
} IufShape;


typedef enum 
{
	IUF_DEMODULATION_QUADRATURE = 0,
	IUF_DEMODULATION_FOURX,
	IUF_DEMODULATION_HILBERT
} IufDemodulationMethod;

/** \brief Possible driving patterns */
typedef enum
{
    IUF_INVALID_DRIVING_SCHEME = 0,
    IUF_DIVERGING_WAVES_PARAMETRIZED, /**< focus point has negative z value (parametrized) */
    IUF_DIVERGING_WAVES,                 /**< focus point has negative z value */
    IUF_FOCUSED_WAVES_PARAMETRIZED,      /**< focus point has positive z value */
    IUF_FOCUSED_WAVES,       /**< focus point has positive z value */
    IUF_PLANE_WAVES,         /**< neighbouring elements fire consecutively with constant inter-element delay */
    IUF_SINGLE_ELEMENT,      /**< simplified case where there is only one element */
    IUF_CUSTOM_WAVES         /**< non-specified driving scheme category, the transmit pattern and transmit delays of elements can be anything */
} IufDrivingSchemeType ;

#define DRIVINGSCHEME_DIVERGING_WAVES_PARAMETRIZED   "DIVERGING_WAVES_PARAMETRIZED"
#define DRIVINGSCHEME_DIVERGING_WAVES                "DIVERGING_WAVES"
#define DRIVINGSCHEME_FOCUSED_WAVES_PARAMETRIZED     "FOCUSED_WAVES_PARAMETRIZED"
#define DRIVINGSCHEME_FOCUSED_WAVES                  "FOCUSED_WAVES"
#define DRIVINGSCHEME_PLANE_WAVES                    "PLANE_WAVES"
#define DRIVINGSCHEME_SINGLE_ELEMENT                 "SINGLE_ELEMENT"
#define DRIVINGSCHEME_CUSTOM_WAVES                   "CUSTOM_WAVES"




#ifdef __cplusplus
}
#endif

#endif  // _IUFRFFILE_H
