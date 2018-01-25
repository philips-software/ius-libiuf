//=============================================================================
//    COPYRIGHT 2016 PHILIPS RESEARCH
//=============================================================================
//
//  Source Name   : iusMatrixUtils.c
//                  Basic linear algebra functions for matrix 
//                  Singular Value Decomposition (SVD)
//                  Pseudo-Inverse (Moore-Penrose Inverse)
//                  The others
//  iusVersion    : 255.255.255.255
//
//=============================================================================

#include "iusMatrixUtils.h"
#include "iusBasicFunctions.h"
#include "iusError.h"
#include <memory.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

//=============================================================================
// L O C A L   D E F I N E S   A N D   V A R I A B L E S
//=============================================================================

//=============================================================================
// S T A T I C   F U N C T I O N   P R O T O T Y P E S
//=============================================================================

//=============================================================================
// E X P O R T E D   F U N C T I O N S
//=============================================================================

//#define MIN(x,y) ( (x) < (y) ? (x) : (y) )
//#define MAX(x,y) ((x)>(y)?(x):(y))
#define SIGN(a, b) ((b) >= 0.0 ? (IUS_ABS(a)) : (-(IUS_ABS(a))))
#define SWAPD(a, b) (temp = (a), (a) = (b), (b) = temp)

static void sort_dsv(size_t m, size_t n, size_t k,
	double **a, double *w, double **v)
{
	size_t i, j;
	double temp;

	for (i = k; (i < n - 1) && (w[i] < w[i + 1]); i++) {
		SWAPD(w[i], w[i + 1]);
		for (j = 0; j < m; j++) SWAPD(a[j][i], a[j][i + 1]);
		for (j = 0; j < n; j++) SWAPD(v[j][i], v[j][i + 1]);
	}
}

static void sort_fsv(size_t m, size_t n, size_t k,
	float **a, float *w, float **v)
{
	size_t i, j;
	float temp;

	for (i = k; (i < n - 1) && (w[i] < w[i + 1]); i++) {
		SWAPD(w[i], w[i + 1]);
		for (j = 0; j < m; j++) SWAPD(a[j][i], a[j][i + 1]);
		for (j = 0; j < n; j++) SWAPD(v[j][i], v[j][i + 1]);
	}
}

static double PYTHAGN(double a, double b)
{
	double at = IUS_ABS(a);
	double bt = IUS_ABS(b);
	double ct, result;

	if (at > bt) { ct = bt / at; result = at * (double)sqrt(1.0 + (double)ct * (double)ct); }
	else if (bt > 0.0) { ct = at / bt; result = bt * (double)sqrt(1.0 + (double)ct * (double)ct); }
	else result = 0.0;
	return(result);
}

static float PYTHAG(float a, float b)
{
	float at = IUS_ABS(a);
	float bt = IUS_ABS(b);
	float ct, result;

	if (at > bt) { ct = bt / at; result = at * (float)sqrt(1.0 + (float)ct * (float)ct); }
	else if (bt > 0.0) { ct = at / bt; result = bt * (float)sqrt(1.0 + (float)ct * (float)ct); }
	else result = 0.0;
	return(result);
}

/*
*Input to iusSvdDouble and iusSvdFloat are as follows :
*   ppA = mxn matrix to be decomposed, gets overwritten with U
*   m   = row dimension of ppA, note that m >= n, otherwise use ppA' as input
*   n   = column dimension of ppA
*   pW  = 1xn vector of singular values of ppA
*   ppV = nxn right orthogonal transformation matrix V 
*/
int iusSvdDouble
(
	double * * ppA,
	int m,
	int n,
	double * pW,
	double * * ppV
)
{
	int flag, i, its, j, jj, k, l, nm;
	double c, f, h, s, x, y, z;
	double anorm = 0.0, g = 0.0, scale = 0.0;
	double *rv1;

	l = 0; 
	nm = 0; // remove warning
	IUS_ASSERT( (ppA != NULL) && (pW != NULL) && (ppV != NULL) );
	IUS_ASSERT( ppV != ppA );

	if (m < n)
	{
		fprintf(stderr, "#rows must be > #cols \n");
		return(0);
	}

	rv1 = (double *)malloc((unsigned int)n*sizeof(double));

	/* Householder reduction to bidiagonal form */
	for (i = 0; i < n; i++)
	{
		/* left-hand reduction */
		l = i + 1;
		rv1[i] = scale * g;
		g = s = scale = 0.0;
		if (i < m)
		{
			for (k = i; k < m; k++)
				scale += IUS_ABS((double)ppA[k][i]);
			if (scale)
			{
				for (k = i; k < m; k++)
				{
					ppA[k][i] = (double)((double)ppA[k][i] / scale);
					s += ((double)ppA[k][i] * (double)ppA[k][i]);
				}
				f = (double)ppA[i][i];
				g = -SIGN((double)sqrt(s), f);
				h = f * g - s;
				ppA[i][i] = (double)(f - g);
				if (i != n - 1)
				{
					for (j = l; j < n; j++)
					{
						for (s = 0.0, k = i; k < m; k++)
							s += ((double)ppA[k][i] * (double)ppA[k][j]);
						f = s / h;
						for (k = i; k < m; k++)
							ppA[k][j] += (double)(f * (double)ppA[k][i]);
					}
				}
				for (k = i; k < m; k++)
					ppA[k][i] = (double)((double)ppA[k][i] * scale);
			}
		}
		pW[i] = (double)(scale * g);

		/* right-hand reduction */
		g = s = scale = 0.0;
		if (i < m && i != n - 1)
		{
			for (k = l; k < n; k++)
				scale += IUS_ABS((double)ppA[i][k]);
			if (scale)
			{
				for (k = l; k < n; k++)
				{
					ppA[i][k] = (double)((double)ppA[i][k] / scale);
					s += ((double)ppA[i][k] * (double)ppA[i][k]);
				}
				f = (double)ppA[i][l];
				g = -SIGN((double)sqrt(s), f);
				h = f * g - s;
				ppA[i][l] = (double)(f - g);
				for (k = l; k < n; k++)
					rv1[k] = (double)ppA[i][k] / h;
				if (i != m - 1)
				{
					for (j = l; j < m; j++)
					{
						for (s = 0.0, k = l; k < n; k++)
							s += ((double)ppA[j][k] * (double)ppA[i][k]);
						for (k = l; k < n; k++)
							ppA[j][k] += (double)(s * rv1[k]);
					}
				}
				for (k = l; k < n; k++)
					ppA[i][k] = (double)((double)ppA[i][k] * scale);
			}
		}
		anorm = IUS_MAX(anorm, (IUS_ABS(pW[i]) + IUS_ABS(rv1[i])));
	}

	/* accumulate the right-hand transformation */
	for (i = n - 1; i >= 0; i--)
	{
		if (i < n - 1)
		{
			if (g)
			{
				for (j = l; j < n; j++)
					ppV[j][i] = (double)(((double)ppA[i][j] / (double)ppA[i][l]) / g);
				/* double division to avoid underflow */
				for (j = l; j < n; j++)
				{
					for (s = 0.0, k = l; k < n; k++)
						s += ((double)ppA[i][k] * (double)ppV[k][j]);
					for (k = l; k < n; k++)
						ppV[k][j] += (double)(s * (double)ppV[k][i]);
				}
			}
			for (j = l; j < n; j++)
				ppV[i][j] = ppV[j][i] = 0.0;
		}
		ppV[i][i] = 1.0;
		g = rv1[i];
		l = i;
	}

	/* accumulate the left-hand transformation */
	for (i = n - 1; i >= 0; i--)
	{
		l = i + 1;
		g = (double)pW[i];
		if (i < n - 1)
			for (j = l; j < n; j++)
				ppA[i][j] = 0.0;
		if (g)
		{
			g = 1.0f / g;
			if (i != n - 1)
			{
				for (j = l; j < n; j++)
				{
					for (s = 0.0, k = l; k < m; k++)
						s += ((double)ppA[k][i] * (double)ppA[k][j]);
					f = (s / (double)ppA[i][i]) * g;
					for (k = i; k < m; k++)
						ppA[k][j] += (double)(f * (double)ppA[k][i]);
				}
			}
			for (j = i; j < m; j++)
				ppA[j][i] = (double)((double)ppA[j][i] * g);
		}
		else
		{
			for (j = i; j < m; j++)
				ppA[j][i] = 0.0;
		}
		++ppA[i][i];
	}

	/* diagonalize the bidiagonal form */
	for (k = n - 1; k >= 0; k--)
	{                             /* loop over singular values */
		for (its = 0; its < 30; its++)
		{                         /* loop over allowed iterations */
			flag = 1;
			for (l = k; l >= 0; l--)
			{                     /* test for splitting */
				nm = l - 1;
				if (fabs(rv1[l]) + anorm == anorm)
				{
					flag = 0;
					break;
				}
				if (fabs((double)pW[nm]) + anorm == anorm)
					break;
			}
			if (flag)
			{
				c = 0.0;
				s = 1.0;
				for (i = l; i <= k; i++)
				{
					f = s * rv1[i];
					if (fabs(f) + anorm != anorm)
					{
						g = (double)pW[i];
						h = PYTHAGN(f, g);
						pW[i] = (double)h;
						h = 1.0f / h;
						c = g * h;
						s = (-f * h);
						for (j = 0; j < m; j++)
						{
							y = (double)ppA[j][nm];
							z = (double)ppA[j][i];
							ppA[j][nm] = (double)(y * c + z * s);
							ppA[j][i] = (double)(z * c - y * s);
						}
					}
				}
			}
			z = (double)pW[k];
			if (l == k)
			{                  /* convergence */
				if (z < 0.0)
				{              /* make singular value nonnegative */
					pW[k] = (double)(-z);
					for (j = 0; j < n; j++)
						ppV[j][k] = (-ppV[j][k]);
				}
				sort_dsv(m, n, k, ppA, pW, ppV);
				break;
			}
			if (its >= 30) {
				free((void*)rv1);
				fprintf(stderr, "No convergence after 30,000! iterations \n");
				return(0);
			}

			/* shift from bottom 2 x 2 minor */
			x = (double)pW[l];
			nm = k - 1;
			y = (double)pW[nm];
			g = rv1[nm];
			h = rv1[k];
			f = ((y - z) * (y + z) + (g - h) * (g + h)) / (2.0f * h * y);
			g = PYTHAGN(f, 1.0);
			f = ((x - z) * (x + z) + h * ((y / (f + SIGN(g, f))) - h)) / x;

			/* next QR transformation */
			c = s = 1.0;
			for (j = l; j <= nm; j++)
			{
				i = j + 1;
				g = rv1[i];
				y = (double)pW[i];
				h = s * g;
				g = c * g;
				z = PYTHAGN(f, h);
				rv1[j] = z;
				c = f / z;
				s = h / z;
				f = x * c + g * s;
				g = g * c - x * s;
				h = y * s;
				y = y * c;
				for (jj = 0; jj < n; jj++)
				{
					x = (double)ppV[jj][j];
					z = (double)ppV[jj][i];
					ppV[jj][j] = (double)(x * c + z * s);
					ppV[jj][i] = (double)(z * c - x * s);
				}
				z = PYTHAGN(f, h);
				pW[j] = (double)z;
				if (z)
				{
					z = 1.0 / z;
					c = f * z;
					s = h * z;
				}
				f = (c * g) + (s * y);
				x = (c * y) - (s * g);
				for (jj = 0; jj < m; jj++)
				{
					y = (double)ppA[jj][j];
					z = (double)ppA[jj][i];
					ppA[jj][j] = (double)(y * c + z * s);
					ppA[jj][i] = (double)(z * c - y * s);
				}
			}
			rv1[l] = 0.0;
			rv1[k] = f;
			pW[k] = (double)x;
		}
	}
	free((double *)rv1);
	return(0);
}

int iusSvdFloat
(
	float * * ppA,
	int m,
	int n,
	float * pW,
	float * * ppV
	)
{
	int flag, i, its, j, jj, k, l, nm;
	float c, f, h, s, x, y, z;
	float anorm = 0.0, g = 0.0, scale = 0.0;
	float *rv1;

	IUS_ASSERT( (ppA != NULL) && (pW != NULL) && (ppV != NULL) );
	IUS_ASSERT( ppV != ppA );
	l  = 0; 
	nm = 0; // remove warning
	if (m < n)
	{
		fprintf(stderr, "#rows must be > #cols \n");
		return(0);
	}

	rv1 = (float *)malloc((unsigned int)n*sizeof(float));

	/* Householder reduction to bidiagonal form */
	for (i = 0; i < n; i++)
	{
		/* left-hand reduction */
		l = i + 1;
		rv1[i] = scale * g;
		g = s = scale = 0.0;
		if (i < m)
		{
			for (k = i; k < m; k++)
				scale += IUS_ABS((float)ppA[k][i]);
			if (scale)
			{
				for (k = i; k < m; k++)
				{
					ppA[k][i] = (float)((float)ppA[k][i] / scale);
					s += ((float)ppA[k][i] * (float)ppA[k][i]);
				}
				f = (float)ppA[i][i];
				g = -SIGN((float)sqrt(s), f);
				h = f * g - s;
				ppA[i][i] = (float)(f - g);
				if (i != n - 1)
				{
					for (j = l; j < n; j++)
					{
						for (s = 0.0, k = i; k < m; k++)
							s += ((float)ppA[k][i] * (float)ppA[k][j]);
						f = s / h;
						for (k = i; k < m; k++)
							ppA[k][j] += (float)(f * (float)ppA[k][i]);
					}
				}
				for (k = i; k < m; k++)
					ppA[k][i] = (float)((float)ppA[k][i] * scale);
			}
		}
		pW[i] = (float)(scale * g);

		/* right-hand reduction */
		g = s = scale = 0.0;
		if (i < m && i != n - 1)
		{
			for (k = l; k < n; k++)
				scale += IUS_ABS((float)ppA[i][k]);
			if (scale)
			{
				for (k = l; k < n; k++)
				{
					ppA[i][k] = (float)((float)ppA[i][k] / scale);
					s += ((float)ppA[i][k] * (float)ppA[i][k]);
				}
				f = (float)ppA[i][l];
				g = -SIGN((float)sqrt(s), f);
				h = f * g - s;
				ppA[i][l] = (float)(f - g);
				for (k = l; k < n; k++)
					rv1[k] = (float)ppA[i][k] / h;
				if (i != m - 1)
				{
					for (j = l; j < m; j++)
					{
						for (s = 0.0, k = l; k < n; k++)
							s += ((float)ppA[j][k] * (float)ppA[i][k]);
						for (k = l; k < n; k++)
							ppA[j][k] += (float)(s * rv1[k]);
					}
				}
				for (k = l; k < n; k++)
					ppA[i][k] = (float)((float)ppA[i][k] * scale);
			}
		}
		anorm = IUS_MAX(anorm, (IUS_ABS(pW[i]) + IUS_ABS(rv1[i])));
	}

	/* accumulate the right-hand transformation */
	for (i = n - 1; i >= 0; i--)
	{
		if (i < n - 1)
		{
			if (g)
			{
				for (j = l; j < n; j++)
					ppV[j][i] = (float)(((float)ppA[i][j] / (float)ppA[i][l]) / g);
				/* float division to avoid underflow */
				for (j = l; j < n; j++)
				{
					for (s = 0.0, k = l; k < n; k++)
						s += ((float)ppA[i][k] * (float)ppV[k][j]);
					for (k = l; k < n; k++)
						ppV[k][j] += (float)(s * (float)ppV[k][i]);
				}
			}
			for (j = l; j < n; j++)
				ppV[i][j] = ppV[j][i] = 0.0;
		}
		ppV[i][i] = 1.0;
		g = rv1[i];
		l = i;
	}

	/* accumulate the left-hand transformation */
	for (i = n - 1; i >= 0; i--)
	{
		l = i + 1;
		g = (float)pW[i];
		if (i < n - 1)
			for (j = l; j < n; j++)
				ppA[i][j] = 0.0;
		if (g)
		{
			g = 1.0f / g;
			if (i != n - 1)
			{
				for (j = l; j < n; j++)
				{
					for (s = 0.0, k = l; k < m; k++)
						s += ((float)ppA[k][i] * (float)ppA[k][j]);
					f = (s / (float)ppA[i][i]) * g;
					for (k = i; k < m; k++)
						ppA[k][j] += (float)(f * (float)ppA[k][i]);
				}
			}
			for (j = i; j < m; j++)
				ppA[j][i] = (float)((float)ppA[j][i] * g);
		}
		else
		{
			for (j = i; j < m; j++)
				ppA[j][i] = 0.0;
		}
		++ppA[i][i];
	}

	/* diagonalize the bidiagonal form */
	for (k = n - 1; k >= 0; k--)
	{                             /* loop over singular values */
		for (its = 0; its < 30; its++)
		{                         /* loop over allowed iterations */
			flag = 1;
			for (l = k; l >= 0; l--)
			{                     /* test for splitting */
				nm = l - 1;
				if (fabs(rv1[l]) + anorm == anorm)
				{
					flag = 0;
					break;
				}
				if (fabs((float)pW[nm]) + anorm == anorm)
					break;
			}
			if (flag)
			{
				c = 0.0;
				s = 1.0;
				for (i = l; i <= k; i++)
				{
					f = s * rv1[i];
					if (fabs(f) + anorm != anorm)
					{
						g = (float)pW[i];
						h = PYTHAG(f, g);
						pW[i] = (float)h;
						h = 1.0f / h;
						c = g * h;
						s = (-f * h);
						for (j = 0; j < m; j++)
						{
							y = (float)ppA[j][nm];
							z = (float)ppA[j][i];
							ppA[j][nm] = (float)(y * c + z * s);
							ppA[j][i] = (float)(z * c - y * s);
						}
					}
				}
			}
			z = (float)pW[k];
			if (l == k)
			{                  /* convergence */
				if (z < 0.0)
				{              /* make singular value nonnegative */
					pW[k] = (float)(-z);
					for (j = 0; j < n; j++)
						ppV[j][k] = (-ppV[j][k]);
				}
				sort_fsv(m, n, k, ppA, pW, ppV);
				break;
			}
			if (its >= 30) {
				free((void*)rv1);
				fprintf(stderr, "No convergence after 30,000! iterations \n");
				return(0);
			}

			/* shift from bottom 2 x 2 minor */
			x = (float)pW[l];
			nm = k - 1;
			y = (float)pW[nm];
			g = rv1[nm];
			h = rv1[k];
			f = ((y - z) * (y + z) + (g - h) * (g + h)) / (2.0f * h * y);
			g = PYTHAG(f, 1.0);
			f = ((x - z) * (x + z) + h * ((y / (f + SIGN(g, f))) - h)) / x;

			/* next QR transformation */
			c = s = 1.0;
			for (j = l; j <= nm; j++)
			{
				i = j + 1;
				g = rv1[i];
				y = (float)pW[i];
				h = s * g;
				g = c * g;
				z = PYTHAG(f, h);
				rv1[j] = z;
				c = f / z;
				s = h / z;
				f = x * c + g * s;
				g = g * c - x * s;
				h = y * s;
				y = y * c;
				for (jj = 0; jj < n; jj++)
				{
					x = (float)ppV[jj][j];
					z = (float)ppV[jj][i];
					ppV[jj][j] = (float)(x * c + z * s);
					ppV[jj][i] = (float)(z * c - x * s);
				}
				z = PYTHAG(f, h);
				pW[j] = (float)z;
				if (z)
				{
					z = 1.0f / z;
					c = f * z;
					s = h * z;
				}
				f = (c * g) + (s * y);
				x = (c * y) - (s * g);
				for (jj = 0; jj < m; jj++)
				{
					y = (float)ppA[jj][j];
					z = (float)ppA[jj][i];
					ppA[jj][j] = (float)(y * c + z * s);
					ppA[jj][i] = (float)(z * c - y * s);
				}
			}
			rv1[l] = 0.0;
			rv1[k] = f;
			pW[k] = (float)x;
		}
	}
	free((float *)rv1);
	return(0);
}


