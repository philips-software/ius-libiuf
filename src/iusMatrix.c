//=============================================================================
//    COPYRIGHT 2015 PHILIPS RESEARCH
//=============================================================================
//
//  Source Name   : iusMatrix.c
//                  Basic functions for matrix memory and operations
//  iusVersion    : 255.255.255.255
//
//=============================================================================

#include "iusMatrix.h"
#include "iusVector.h"
#include "iusError.h"
#include <stdio.h>  // fprintf, stdout
#include <stdlib.h> // malloc..

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
// iusAllocate a matrix
//-----------------------------------------------------------------------------
double ** iusAllocDoubleMatrix
(
    const int n1,
    const int n2
)
{
    int i;
    double **ppDoubleMatrix = (double **)malloc( n1 * sizeof( double * ) );

    IUS_ASSERT_MEMORY( ppDoubleMatrix );

    for ( i = 0; i < n1; i++ )
    {
        ppDoubleMatrix[i] = iusAllocDoubleVector( n2 );
    }
    return ppDoubleMatrix;
}

float ** iusAllocFloatMatrix
(
    const int n1,
    const int n2
)
{
    int i;
    float **ppFloatMatrix = (float **)malloc( n1 * sizeof( float * ) );

	IUS_ASSERT_MEMORY( ppFloatMatrix );

    for ( i = 0; i < n1; i++ )
    {
        ppFloatMatrix[i] = iusAllocFloatVector( n2 );
    }
    return ppFloatMatrix;
}

int ** iusAllocIntMatrix
(
    const int n1,
    const int n2
)
{
    int i;
    int **ppIntMatrix = (int **)malloc( n1 * sizeof( int * ) );

	IUS_ASSERT_MEMORY( ppIntMatrix );

    for ( i = 0; i < n1; i++ )
    {
        ppIntMatrix[i] = iusAllocIntVector( n2 );
    }
    return ppIntMatrix;
}

short ** iusAllocShortMatrix
(
    const int n1,
    const int n2
)
{
    int i;
    short **ppShortMatrix = (short **)malloc( n1 * sizeof( short * ) );

	IUS_ASSERT_MEMORY( ppShortMatrix );

    for ( i = 0; i < n1; i++ )
    {
        ppShortMatrix[i] = iusAllocShortVector( n2 );
    }
    return ppShortMatrix;
}

unsigned char ** iusAllocByteMatrix
(
    const int n1,
    const int n2
)
{
    int i;
    unsigned char **ppByteMatrix = (unsigned char **)malloc( n1 * sizeof( unsigned char * ) );

	IUS_ASSERT_MEMORY( ppByteMatrix );

    for ( i = 0; i < n1; i++ )
    {
        ppByteMatrix[i] = iusAllocByteVector( n2 );
    }
    return ppByteMatrix;
}

unsigned char *** iusAllocByteMatrix3D
(
	const int n1,
	const int n2,
	const int n3
)
{
	int i;
	unsigned char ***pppByteMatrix = (unsigned char ***)malloc( n1 * sizeof( unsigned char ** ) );

	IUS_ASSERT_MEMORY( pppByteMatrix );

	for ( i = 0; i < n1; i++ )
	{
		pppByteMatrix[i] = iusAllocByteMatrix( n2, n3 );
	}
	return pppByteMatrix;
}

float *** iusAllocFloatMatrix3D
(
    const int n1,
    const int n2,
    const int n3
)
{
    int i;
    float *** pppFloatMatrix = (float ***)malloc( n1 * sizeof( float ** ) );

	IUS_ASSERT_MEMORY( pppFloatMatrix );

    for ( i = 0; i < n1; i++ )
    {
        pppFloatMatrix[i] = iusAllocFloatMatrix( n2, n3 );
    }
    return pppFloatMatrix;
}

IusVelocity ** iusAllocVelocityFloatMatrix
(
    const int n1,
    const int n2
)
{
    int i;
    
    IusVelocity **ppVelocityFloatMatrix =
        ( IusVelocity ** )malloc( n1 * sizeof( IusVelocity * ) ); 
	
    IUS_ASSERT_MEMORY( ppVelocityFloatMatrix );    
    
    for ( i = 0; i < n1; i++ )
    {
        ppVelocityFloatMatrix[i] = (IusVelocity *)calloc( n2 , sizeof(IusVelocity));
        IUS_ASSERT_MEMORY( ppVelocityFloatMatrix[ i ] ); 
    }

    return ppVelocityFloatMatrix;

}



//-----------------------------------------------------------------------------
// Free a float matrix
//-----------------------------------------------------------------------------
void iusFreeDoubleMatrix
(
    double ** ppDoubleMatrix,
    const int n1
)
{
	int i;

	IUS_ASSERT( ppDoubleMatrix );
    for ( i = 0; i < n1; i++ )
    {
        iusFreeDoubleVector( ppDoubleMatrix[i] );
    }
    free( ppDoubleMatrix );
}

void iusFreeFloatMatrix
(
    float **  ppFloatMatrix,
    const int n1
)
{
	int i;

	IUS_ASSERT( ppFloatMatrix );
    for ( i = 0; i < n1; i++ )
    {
        iusFreeFloatVector( ppFloatMatrix[i] );
    }
    free( ppFloatMatrix );
}
 
void iusFreeIntMatrix
(
    int **    ppIntMatrix,
    const int n1
)
{
	int i;

	IUS_ASSERT( ppIntMatrix );
    for ( i = 0; i < n1; i++ )
    {
        iusFreeIntVector( ppIntMatrix[i] );
    }
    free( ppIntMatrix );
}

void iusFreeShortMatrix
(
    short **  ppShortMatrix,
    const int n1
)
{
	IUS_ASSERT( ppShortMatrix );
    int i;
    for ( i = 0; i < n1; i++ )
    {
        iusFreeShortVector( ppShortMatrix[i] );
    }
    free( ppShortMatrix );
}

void iusFreeByteMatrix
(
    unsigned char **ppByteMatrix,
    const int       n1
)
{
	int i;

	IUS_ASSERT( ppByteMatrix );
	for ( i = 0; i < n1; i++ )
    {
        iusFreeByteVector( ppByteMatrix[i] );
    }
    free( ppByteMatrix );
}

void iusFreeByteMatrix3D
(
	unsigned char ***pppByteMatrix,
	const int        n1,
	const int        n2
)
{
	int i;

	IUS_ASSERT( pppByteMatrix );
	for ( i = 0; i < n1; i++ )
	{
		iusFreeByteMatrix( pppByteMatrix[i], n2 );
	}
	free(pppByteMatrix);
}

void iusFreeFloatMatrix3D
(
    float *** pppFloatMatrix,
    const int n1,
    const int n2
)
{
	int i;

	IUS_ASSERT( pppFloatMatrix );
    for ( i = 0; i < n1; i++ )
    {
        iusFreeFloatMatrix( pppFloatMatrix[i], n2 );
    }
    free( pppFloatMatrix );
}

void iusFreeVelocityFloatMatrix
(
    IusVelocity  **ppVelocityFloatMatrix,
    const int              n1
)
{
    int i;
    
    IUS_ASSERT_MEMORY( ppVelocityFloatMatrix );    
    
    for ( i = 0; i < n1; i++ )
    {
        free( ppVelocityFloatMatrix[i] );
    }     
    free( ppVelocityFloatMatrix );
}


//-----------------------------------------------------------------------------
// Set all float matrix elements to the same float value
//-----------------------------------------------------------------------------
void iusSetValue2FloatMatrix
(
    float *const *const ppFloatMatrix,
    const int           n1,
    const int           n2,
    const float         value
)
{
    int i;

	IUS_ASSERT( ppFloatMatrix );

    for ( i = 0; i < n1; i++ )
    {
        iusSetValue2FloatVector( ppFloatMatrix[i], n2, value );
    }
}

//-----------------------------------------------------------------------------
// Set all float matrix elements to zero
//-----------------------------------------------------------------------------
void iusZeroFloatMatrix
(
    float *const *const ppFloatMatrix,
    const int           n1,
    const int           n2
)
{
    int i;

	IUS_ASSERT( ppFloatMatrix );

    for (i = 0; i < n1; i++)
    {
        iusZeroFloatVector(ppFloatMatrix[i], n2);
    }
}

//-----------------------------------------------------------------------------
// Set all 3D float matrix elements to zero
//-----------------------------------------------------------------------------
void iusZeroFloatMatrix3D
(
    float *const *const *const pppFloatMatrix,
    const int                  n1,
    const int                  n2,
    const int                  n3
)
{
    int i, j;

	IUS_ASSERT( pppFloatMatrix );

    for (i = 0; i < n1; i++)
    {
        for (j = 0; j < n2; j++)
        {
            iusZeroFloatVector( pppFloatMatrix[i][j], n3 );
        }
    }
}

//-----------------------------------------------------------------------------
// Fill a vector with random values
//-----------------------------------------------------------------------------
void iusSetGaussianFloatMatrix // zero-mean and std=1
(
    float *const *const ppFloatMatrix,
    const int           n1,
    const int           n2
)
{
    int i, j, k;

	IUS_ASSERT( ppFloatMatrix );

    for ( k = 0; k < n1; k++ )
    {
        for ( i = 0; i < n2; i++ )
        {
            ppFloatMatrix[k][i] = 0.0f;
            for ( j = 0; j < 12; j++ )
            {
                ppFloatMatrix[k][i] += (float)rand() / (float)RAND_MAX;
            }
            ppFloatMatrix[k][i] -= 6.0f;
        }
    }
}

//-----------------------------------------------------------------------------
// Fill a matrix with random values
//-----------------------------------------------------------------------------
void iusSetRandomDoubleMatrix
(
	double *const *const ppDoubleMatrix,
	const int            n1,
	const int            n2,
	const int            seed
	)
{
	int r, c;

	IUS_ASSERT( ppDoubleMatrix );

	srand( (unsigned int)seed );
	for ( c = 0; c < n1; c++ )
	{
		for ( r = 0; r < n2; r++ )
		{
			ppDoubleMatrix[c][r] = (double)rand();
		}
	}
}
void iusSetRandomFloatMatrix
(
    float *const *const ppFloatMatrix,
    const int           n1,
    const int           n2,
    const int           seed
)
{
    int r, c;

	IUS_ASSERT( ppFloatMatrix );

    srand( (unsigned int)seed );
    for ( c = 0; c < n1; c++ )
    {
        for ( r = 0; r < n2; r++ )
        {
            ppFloatMatrix[c][r] = (float)rand();
        }
    }
}

void iusSetRandomIntMatrix
(
    int *const *const ppIntMatrix,
    const int         n1,
    const int         n2,
    const int         seed
)
{
    int r, c;

	IUS_ASSERT( ppIntMatrix );

    srand( (unsigned int)seed );
    for ( c = 0; c < n1; c++ )
    {
        for ( r = 0; r < n2; r++ )
        {
            ppIntMatrix[c][r] = (int)rand();
        }
    }
}

void iusSetRandomShortMatrix
(
    short *const *const ppShortMatrix,
    const int           n1,
    const int           n2,
    const int           seed
)
{
    int r, c;

	IUS_ASSERT( ppShortMatrix );

    srand( (unsigned int)seed );
    for ( c = 0; c < n1; c++ )
    {
        for ( r = 0; r < n2; r++ )
        {
            ppShortMatrix[c][r] = (short)rand();
        }
    }
}

//-----------------------------------------------------------------------------
// Copy all data in a matrix to another matrix with the same dimensions
//-----------------------------------------------------------------------------
void iusCopyFloatMatrix
(
          float *const *const ppDest,
    const float *const *const ppSrc,
    const int                 n1,
    const int                 n2
)
{
    int i;

	IUS_ASSERT( ppSrc && ppDest && ( (void*)ppSrc != (void*)ppDest) );

    for ( i = 0; i < n1; i++ )
    {
        iusCopyFloatVector( ppDest[i], ppSrc[i], n2 );
    }
}

void iusCopyShortMatrix2FloatMatrix
(
          float *const *const ppDest,
    const short *const *const ppSrc,
    const int                 n1,
    const int                 n2
)
{
    int i;

	IUS_ASSERT( ppSrc  && ppDest );

    for ( i = 0; i < n1; i++ )
    {
        iusCopyShortVector2FloatVector( ppDest[i], ppSrc[i], n2 );
    }
}

void iusCopyFloatMatrix2ShortMatrix
(
          short *const *const ppDest,
    const float *const *const ppSrc,
    const int                 n1,
    const int                 n2
)
{
    int i;

	IUS_ASSERT( ppSrc && ppDest );

    for ( i = 0; i < n1; i++ )
    {
        iusCopyFloatVector2ShortVector( ppDest[i], ppSrc[i], n2 );
    }
}

//------------------------------------------------------------------------------
// Copy from a float matrix one float vector walking along the first dimension
// of the matrix (think of copying one matrix column to a vector if the matrix 
// has been defined with the first index as the row index and the second index
// as the column index).
//------------------------------------------------------------------------------
void iusCopyVectorAlongFloatMatrixDim1
(
    float *const              pDest,
    const float *const *const ppSrc,
    const int                 n1,
    const int                 n2,
    const int                 c
)
{
    int i;

	IUS_ASSERT( pDest && ppSrc );
	IUS_ASSERT( (c >= 0) && (c < n2) );

    for ( i = 0; i < n1; i++ )
    {
        pDest[i] = ppSrc[i][c];
    }
}

//------------------------------------------------------------------------------
// Copy from a float matrix one float vector walking along the second dimension
// of the matrix (think of copying one matrix row to a vector if the matrix has
// been defined with the first index as the row index and the second index as
// the column index).
//------------------------------------------------------------------------------
void iusCopyVectorAlongFloatMatrixDim2
(
    float *const              pDest,
    const float *const *const ppSrc,
    const int                 n1,
    const int                 n2,
    const int                 r
)
{
	IUS_ASSERT( pDest  && ppSrc );
	IUS_ASSERT( (r >= 0) && (r < n1) );

    iusCopyFloatVector( pDest, ppSrc[r], n2 );
}

//-----------------------------------------------------------------------------
// Elementwise addition of float matrices
//-----------------------------------------------------------------------------
void iusAddFloatMatrices
(
          float *const *const ppDest,
    const float *const *const ppSrc1,
    const float *const *const ppSrc2,
    const int                 n1,
    const int                 n2
) 
{
    int i, j;

	IUS_ASSERT( ppDest && ppSrc1 && ppSrc2 );

    for ( j = 0; j < n1; j++ )
    {
        for ( i = 0; i < n2; i++ )
        {
            ppDest[j][i] = ppSrc1[j][i] + ppSrc2[j][i];
        }
    }
}

//-----------------------------------------------------------------------------
// Center float matrix columnwise
//-----------------------------------------------------------------------------
void iusCenterDim1FloatMatrix
(
	float *const *const       ppDest,
	const float *const *const ppSrc,
	const int                 n1,
	const int                 n2
)
{
	int i, j;
	float mean;

	IUS_ASSERT( ppDest && ppSrc );

	for ( j = 0; j < n2; j++ )
	{
		mean = 0.0f;
		for ( i = 0; i < n1; i++ )
		{
			mean += ppSrc[i][j]; 
		}
		mean = mean/(float)n1;
		for ( i = 0; i < n1; i++ )
		{
			ppDest[i][j] = ppSrc[i][j]-mean;
		}
	}
}

//-----------------------------------------------------------------------------
// Set float vector to diagonal values of a float square matrix
//-----------------------------------------------------------------------------
void iusSetFloatSquareMatrixDiagonal
(
	float *const *const ppFloatMatrix,
	const float *const  pSrc,
	const int           n
)
{
	int i;

	IUS_ASSERT( ppFloatMatrix && pSrc );

	iusZeroFloatMatrix( ppFloatMatrix, n, n );
	for ( i = 0; i < n; i++ )
	{
		ppFloatMatrix[i][i] = pSrc[i];
	}
}

//-----------------------------------------------------------------------------
// Reshape float vector to float matrix
//-----------------------------------------------------------------------------
void iusReshapeFloatVector2FloatMatrix
(
	float *const *const ppFloatMatrix,
	const float *const  pSrc,
	const int           n1,
	const int           n2
)
{
	int i, j;

	IUS_ASSERT( ppFloatMatrix && pSrc );

	for ( i = 0; i < n1; i++ )
	{
		for ( j = 0; j < n2; j++ )
		{
			ppFloatMatrix[i][j] = pSrc[j*n1 + i];
		}		
	}
}

//-----------------------------------------------------------------------------
// Reshape double vector to double matrix
//-----------------------------------------------------------------------------
void iusReshapeDoubleVector2DoubleMatrix
(
	double *const *const ppDoubleMatrix,
	const double *const  pSrc,
	const int            n1,
	const int            n2
)
{
	int i, j;

	IUS_ASSERT( ppDoubleMatrix && pSrc );

	for ( i = 0; i < n1; i++ )
	{
		for ( j = 0; j < n2; j++ )
		{
			ppDoubleMatrix[i][j] = pSrc[j*n1 + i];
		}
	}
}

//-----------------------------------------------------------------------------
// Transpose float matrix
//-----------------------------------------------------------------------------
void iusTransposeFloatMatrix
(
	float *const *const       ppDest,
	const float *const *const ppSrc,
	const int                 n1,
	const int                 n2
)
{
	int i, j;

	IUS_ASSERT( ppDest && ppSrc );
	IUS_ASSERT( ((float **)ppDest != (float **)ppSrc) );

	for ( j = 0; j < n1; j++ )
	{
		for ( i = 0; i < n2; i++ )
		{
				ppDest[i][j] = ppSrc[j][i];
		}
	}
}

//-----------------------------------------------------------------------------
// Inverse square diagonal float matrix
//-----------------------------------------------------------------------------
void iusFloatSquareDiagonalMatrixInverse
(
	      float *const *const ppDest,
	const float *const *const ppSrc,
	const int                 n
)
{
	int i;

	IUS_ASSERT( ppDest && ppSrc );

	for ( i = 0; i < n; i++ )
	{
		if ( ppSrc[i][i] != 0 )
		{
			ppDest[i][i] = 1 / ppSrc[i][i];
		}			
	}	
}

//-----------------------------------------------------------------------------
// Multiply float matrix
//-----------------------------------------------------------------------------
void iusMultiplyFloatMatrices
(
	      float *const *const ppDest,
	const float *const *const ppSrc1,
	const float *const *const ppSrc2,
	const int                 n1,
	const int                 n2,
	const int                 m1,
	const int                 m2
)
{
	int i, j, k;

	IUS_ASSERT( ppDest && ppSrc1 && ppSrc2 && (n2 == m1) );
	IUS_ASSERT( ((float **)ppDest != (float **)ppSrc1) && ((float **)ppDest != (float **)ppSrc2) );

    iusZeroFloatMatrix( ppDest, n1, m2 );
	for ( j = 0; j < n1; j++ )
	{
		for ( i = 0; i < m2; i++ )
		{
			for ( k = 0; k < n2; k++ )
			{
				ppDest[j][i] += ppSrc1[j][k] * ppSrc2[k][i];			
			}
		}
	}
}

//-----------------------------------------------------------------------------
// Elementwise multiplication of float matrices
//-----------------------------------------------------------------------------
void iusMultiplyFloatMatricesElementByElement
(
          float *const *const ppDest,
    const float *const *const ppSrc1,
    const float *const *const ppSrc2,
    const int                 n1,
    const int                 n2
)
{
    int i, j;

	IUS_ASSERT( ppDest && ppSrc1 && ppSrc2 );

    for ( j = 0; j < n1; j++ )
    {
        for ( i = 0; i < n2; i++ )
        {
            ppDest[j][i] = ppSrc1[j][i] * ppSrc2[j][i];
        }
    }
}

//-----------------------------------------------------------------------------
// Multiply all elements of a matrix with the same scale factor
//-----------------------------------------------------------------------------
void iusScaleFloatMatrix
(
          float *const *const ppDest,
    const float *const *const ppSrc,
    const float               factor,
    const int                 n1,
    const int                 n2
)
{
    int i, j;

	IUS_ASSERT( ppDest && ppSrc );

    for ( j = 0; j < n1; j++ )
    {
        for ( i = 0; i < n2; i++ )
        {
            ppDest[j][i] = ppSrc[j][i] * factor;
        }
    }
}

//-----------------------------------------------------------------------------
// Compute the mean value across all elements of a matrix
//-----------------------------------------------------------------------------
float iusComputeFloatMatrixMean
(
    const float *const *const ppFloatMatrix,
    const int                 n1,
    const int                 n2
)
{
    int i, j;
    float mean = 0.0f;

	IUS_ASSERT( ppFloatMatrix );

    for ( j = 0; j < n1; j++ )
    {
        for ( i = 0; i < n2; i++ )
        {
            mean += ppFloatMatrix[j][i];
        }
    }
    mean = mean / (float)( n1 * n2 );
    return mean;
}

//-----------------------------------------------------------------------------
// Computes the max value across all elements of a matrix
//-----------------------------------------------------------------------------
float iusMaxFloatMatrix
(
    const float *const *const ppSrc,
    const int                 n1,
    const int                 n2
)
{
    float val;
    int i;

	IUS_ASSERT( ppSrc );
	IUS_ASSERT( n1 > 0 );

    val = iusMaxFloatVector( ppSrc[0], n2 );
    for ( i = 1; i < n1; i++ )
    {
        float mx = iusMaxFloatVector( ppSrc[i], n2 );
        if ( mx > val )
        {
            val = mx;
        }
    }
    return val;
}

//-----------------------------------------------------------------------------
// Computes the min value across all elements of a matrix
//-----------------------------------------------------------------------------
float iusMinFloatMatrix
(
    const float *const *const ppSrc,
    const int                 n1,
    const int                 n2
)
{
    float val;
    int i;

	IUS_ASSERT( ppSrc );
	IUS_ASSERT( n1 > 0 );

    val = iusMinFloatVector( ppSrc[0], n2 );
    for ( i = 1; i < n1; i++ )
    {
        float mn = iusMinFloatVector( ppSrc[i], n2 );
        if ( mn < val )
        {
            val = mn;
        }
    }
    return val;
}

//-----------------------------------------------------------------------------
// Computes the max value across all elements of a matrix and its two indices
//-----------------------------------------------------------------------------
float iusMaxIndicesFloatMatrix
(
    const float *const *const ppSrc,
    const int                 n1,
    const int                 n2,
    int *                     index1,
    int *                     index2
)
{
    float val;
    int x, x1, x2, i;

	IUS_ASSERT( ppSrc );
	IUS_ASSERT( n1 > 0 );

    val = iusMaxIndexFloatVector( ppSrc[0], n2, &x );
    x1  = 0;
    x2  = x;
    for ( i = 1; i < n1; i++ )
    {
        float mx = iusMaxIndexFloatVector( ppSrc[i], n2, &x );
        if ( mx > val )
        {
            x1 = i;
            x2 = x;
            val = mx;
        }
    }
    *index1 = x1;
    *index2 = x2;

    return val;
}

//-----------------------------------------------------------------------------
// Computes the min value across all elements of a matrix and its two indices
//-----------------------------------------------------------------------------
float iusMinIndicesFloatMatrix
(
    const float *const *const ppSrc,
    const int                 n1,
    const int                 n2,
    int *                     index1,
    int *                     index2
)
{
    float val;
    int x, x1, x2, i;

	IUS_ASSERT( ppSrc );
	IUS_ASSERT( n1 > 0 );

    val = iusMinIndexFloatVector( ppSrc[0], n2, &x );
    x1  = 0;
    x2  = x;
    for ( i = 1; i < n1; i++ )
    {
        float mn = iusMinFloatVector( ppSrc[i], n2 );
        if ( mn < val )
        {
            val = mn;
        }
    }
    *index1 = x1;
    *index2 = x2;

    return val;
}

//-----------------------------------------------------------------------------
// Computes a vector of length n2 with the largest matrix elements along the
// first matrix dimension
//-----------------------------------------------------------------------------
void iusMaxVectorAlongFloatMatrixDim1
(
    float *const              pDest,
    const float *const *const ppSrc,
    const int                 n1,
    const int                 n2
)
{
    int i, j;

	IUS_ASSERT( ppSrc && pDest );
	IUS_ASSERT( (n1 > 0) && (n2 > 0) );

    for ( j = 0; j < n2; j++ )
    {
        float maxVal = ppSrc[0][j];
        for ( i = 1; i < n1; i++ )
        {
            if ( ppSrc[i][j] > maxVal )
            {
                maxVal = ppSrc[i][j];
            }
        }
        pDest[j] = maxVal;
    }
}

//-----------------------------------------------------------------------------
// Computes a vector of length n1 with the largest matrix elements along the
// second matrix dimension
//-----------------------------------------------------------------------------
void iusMaxVectorAlongFloatMatrixDim2
(
    float *const              pDest,
    const float *const *const ppSrc,
    const int                 n1,
    const int                 n2
)
{
    int i;

	IUS_ASSERT( ppSrc && pDest );
	IUS_ASSERT( n1 > 0 );

    for ( i = 0; i < n1; i++ )
    {
        pDest[i] = iusMaxFloatVector( ppSrc[i], n2 );
    }
}

//-----------------------------------------------------------------------------
// Matrix projection sum along 1st dimension leading to a vector of length n2
//-----------------------------------------------------------------------------
void iusSumFloatMatrixAlongDim1
(
    float *const              pDest,
    const float *const *const ppSrc,
    const int                 n1,
    const int                 n2
    )
{
    int i, j;

	IUS_ASSERT( ppSrc && pDest );
	IUS_ASSERT( (n1 > 0) && (n2 > 0) );

    for (j = 0; j < n2; j++)
    {
        pDest[j] = 0.0f;
        for (i = 0; i < n1; i++)
        {
            pDest[j] += ppSrc[i][j];
        }
    }
}

//-----------------------------------------------------------------------------
// Matrix projection sum-squared along 1st dim leading to a vector of length n2
//-----------------------------------------------------------------------------
void iusSumSquaredFloatMatrixAlongDim1
(
    float *const              pDest,
    const float *const *const ppSrc,
    const int                 n1,
    const int                 n2
    )
{
    int i, j;

	IUS_ASSERT( ppSrc && pDest );
	IUS_ASSERT( (n1 > 0) && (n2 > 0) );

    for ( j = 0; j < n2; j++ )
    {
        pDest[j] = 0.0f;
        for ( i = 0; i < n1; i++ )
        {
            pDest[j] += ppSrc[i][j] * ppSrc[i][j];
        }
    }
}

//-----------------------------------------------------------------------------
// Matrix projection sum along 2nd dimension leading to a vector of length n1
//-----------------------------------------------------------------------------
void iusSumFloatMatrixAlongDim2
(
    float *const              pDest,
    const float *const *const ppSrc,
    const int                 n1,
    const int                 n2
    )
{
    int i;

	IUS_ASSERT( ppSrc && pDest );
	IUS_ASSERT( n1 > 0 );

    for (i = 0; i < n1; i++)
    {
        pDest[i] = iusSumElementsFloatVector( ppSrc[i], n2 );
    }
}

//-----------------------------------------------------------------------------
// Matrix projection sum-squared along 2nd dim leading to a vector of length n1
//-----------------------------------------------------------------------------
void iusSumSquaredFloatMatrixAlongDim2
(
    float *const              pDest,
    const float *const *const ppSrc,
    const int                 n1,
    const int                 n2
    )
{
    int i;

	IUS_ASSERT( ppSrc && pDest );
	IUS_ASSERT( n1 > 0 );

    for (i = 0; i < n1; i++)
    {
        pDest[i] = iusSumSquaredElementsFloatVector( ppSrc[i], n2 );
    }
}

//-----------------------------------------------------------------------------
// Fetching a value from a float matrix at a position with non-integer indices
// using a bilinear interpolation.
//-----------------------------------------------------------------------------
float iusFetchBilinearFloatMatrix
(
    const float *const *const ppIn,
    int                       n1,
    int                       n2,
    float                     l1,
    float                     l2
)
{
    int i1, i2, i1p1, i2p1;
    float f1, f2, nw, ne, sw, se, outVal;

	IUS_ASSERT( ppIn );
	IUS_ASSERT( (l1 <= (float)(n1 - 1)) && (l1 >= 0.0f) );
	IUS_ASSERT( (l2 <= (float)(n2 - 1)) && (l2 >= 0.0f) );

    /* Get integer and fractional part of the two non-integer indices */
    i1 = (int)l1;
    i1p1 = (i1 < n1 - 1) ? i1 + 1 : i1;        /* protect array bound */
    i2 = (int)l2;
    i2p1 = (i2 < n2 - 1) ? i2 + 1 : i2;        /* protect array bound */
    f1 = l1 - (float)i1;
    f2 = l2 - (float)i2;

    /* Fetch four surrounding values */
    nw = ppIn[i1][i2];
    ne = ppIn[i1p1][i2];
    sw = ppIn[i1][i2p1];
    se = ppIn[i1p1][i2p1];

    /* Interpolate output value */
    outVal = nw * (1.0f - f1) * (1.0f - f2) + ne * f1 * (1.0f - f2) +
        sw * (1.0f - f1) * f2 + se * f1 * f2;

    return outVal;
}

//-----------------------------------------------------------------------------
// Rotate the column pointers of a matrix to right (steps>0) left (steps<0).
//-----------------------------------------------------------------------------
void iusRotateColumnsFloatMatrix
(
    float **  ppIO,
    const int n1,
    const int steps
)
{
    int i, j;
    float *pTmp;

	IUS_ASSERT( ppIO );

    if ( steps >= 0 )
    {
        // rotate right "steps" times
        for ( j = 0; j < steps; j++ )
        {
            // rotate right 1 time
            pTmp = ppIO[n1 - 1];
            for ( i = n1 - 1; i > 0; i-- )
            {
                ppIO[i] = ppIO[i-1];
            }
            ppIO[0] = pTmp;
        }
    }
    else
    {
        // rotate left "-steps" times
        for ( j = 0; j < -steps; j++ )
        {
            // rotate left 1 time
            pTmp = ppIO[0];
            for ( i = 0; i < n1 - 1; i++ )
            {
                ppIO[i] = ppIO[i + 1];
            }
            ppIO[n1 - 1] = pTmp;
        }
    }
}

//------------------------------------------------------------------------------
// Print all elements of a matrix to stdout
//------------------------------------------------------------------------------
void iusPrintFloatMatrix
(
    const float *const *const ppMatrix,
    int                       n1,
    int                       n2
)
{
    int i, j;

	IUS_ASSERT( ppMatrix );

    for (i = 0; i < n1; i++)
    {
        for (j = 0; j < n2; j++)
        {
            fprintf( stdout, "\t%.4f", ppMatrix[i][j] );
        }
        fprintf( stdout, "\n" );
    }
}

void iusPrintDoubleMatrix
(
	const double *const *const ppMatrix,
	int                        n1,
	int                        n2
	)
{
	int i, j;

	IUS_ASSERT( ppMatrix );

	for (i = 0; i < n1; i++)
	{
		for (j = 0; j < n2; j++)
		{
			fprintf( stdout, "\t%.4lf", ppMatrix[i][j] );
		}
		fprintf( stdout, "\n" );
	}
}


//=============================================================================
// S T A T I C   F U N C T I O N S
//=============================================================================
