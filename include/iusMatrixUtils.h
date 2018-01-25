//=============================================================================
//    COPYRIGHT 2016 PHILIPS RESEARCH
//=============================================================================
//
//  Source Name   : iusMatrixUtils.h
//                  Basic linear algebra functions for matrix 
//                  Singular Value Decomposition (SVD)
//                  Pseudo-Inverse (Moore-Penrose Inverse)
//                  The others
//  iusVersion    : 255.255.255.255
//
//=============================================================================
/** \file 
 * Basic linear algebra functions for matrix like Singular Value Decomposition 
 (SVD) and Pseudo-Inverse (Moore-Penrose Inverse)
 */ 
 
#include <stdlib.h>
#include <stdio.h>

#ifndef _IUSMATRIXUTILS_H
#define _IUSMATRIXUTILS_H

#ifdef __cplusplus
extern "C" {
#endif

/**
* \brief Singular value decomposition of doubleMatrix ppA of size n1 x n2
*   
* \param[in]  ppA  mxn matrix to be decomposed, gets overwritten with 
* \param[in]  n1   row dimension of ppA, note that n1 >= n2, otherwise use ppA' as input
* \param[in]  n2   column dimension of ppA
* \param[out] pW   1 x n2 vector of singular values of ppA
* \param[put] ppV  n2 x n2 right orthogonal transformation matrix  
*/	
int iusSvdDouble
(
	double * * ppA,
	int n1,
	int n2,
	double * pW,
	double * * ppV
);

/**
* \brief Singular value decomposition of floatMatrix ppA of size n1 x n2
*   
* \param[in]  ppA  mxn matrix to be decomposed, gets overwritten with 
* \param[in]  n1   row dimension of ppA, note that n1 >= n2, otherwise use ppA' as input
* \param[in]  n2   column dimension of ppA
* \param[out] pW   1 x n2 vector of singular values of ppA
* \param[put] ppV  n2 x n2 right orthogonal transformation matrix  
*/	
int iusSvdFloat
(
	float * * ppA,
	int n1,
	int n2,
	float * pW,
	float * * ppV
);
	
#ifdef __cplusplus
}
#endif

#endif // _IUSMATRIXUTILS_H
