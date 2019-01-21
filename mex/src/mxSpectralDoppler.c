//=============================================================================
//    COPYRIGHT 2016 PHILIPS RESEARCH
//=============================================================================
//
//  Source Name   : mxSpectralDoppler.c
//                  MatLab mex calling iusSpectralDoppler.c
//  iusVersion    : 255.255.255.255
//
//=============================================================================

#include "iusSpectralDoppler.h"
#include "base/iusVector.h"
#include "base/iusMatrix.h"
#include "base/iusBasicFunctions.h" /* iusIsPowerOfTwo */
#include "mex.h"
#include <string.h> /* strcmp */

//-----------------------------------------------------------------------------
// G L O B A L S
//-----------------------------------------------------------------------------
static IUSSpectralDopplerInstance *		pInst				= NULL;
//---------------------------------------------------------------------
// Intermediate arrays for passing RF & IQ data
// These are initated upon calling "Create"
//---------------------------------------------------------------------
static int *							pPacketStartIndex	= NULL;
static float **							ppRfLinesIn			= NULL;
// IQ Lines in can make use of the array within the iusSpectralDoppler class
static float **							ppPowerSpectrumOut	= NULL;
static float *							pVOut				= NULL;
static float *							pSigmaVOut			= NULL;

//-----------------------------------------------------------------------------
// M E X   G A T E W A Y   F U N C T I O N
//-----------------------------------------------------------------------------
void mexFunction
(
    int             nlhs,   // number of left-hand-side arguments
    mxArray *       plhs[], // the left-hand-side arguments
    int             nrhs,   // number of right-hand-side arguments
    const mxArray * prhs[]  // the right-hand-side arguments
)
{
    char * sMethod;
    mwSize methodLen;

    //---------------------------------------------------------------------
    // Retrieve method from the first input argument (i.e. from prhs[0])
    //---------------------------------------------------------------------
    if (nrhs == 0)
    {
        mexErrMsgTxt("mxSpectralDoppler: No input arguments provided.");
    }
    methodLen = (mwSize)mxGetN(prhs[0]) * sizeof(mxChar) + 1;
    sMethod = (char*)mxMalloc(methodLen);
    if (mxGetString(prhs[0], sMethod, methodLen) != 0)
    {
        mexErrMsgTxt("mxSpectralDoppler: Could not convert method string.");
    }

    //---------------------------------------------------------------------
    // Call the C-algorithm function that belongs to the retrieved method  
    //---------------------------------------------------------------------
    if ((strcmp(sMethod, "?") == 0) || (strcmp(sMethod, "help") == 0) || (strcmp(sMethod, "Help") == 0))
    {
        mexPrintf("========================== mxSpectralDoppler ==========================\n");
        mexPrintf("  mxSpectralDoppler( 'Version' );                                      \n");
        mexPrintf("  mxSpectralDoppler( 'Example' );                                      \n");
        mexPrintf("  mxSpectralDoppler( 'Create', numSamplesPerRfLine, ensembleSize, ...  \n");
        mexPrintf("            packetStartIndex, numPackets, packetSize, samplesPerWave); \n");
        mexPrintf("    numSamplesPerRfLine : number of samples per input RF line          \n");
        mexPrintf("    ensembleSize        : amount of consecutive RF lines from          \n");
        mexPrintf("                          which one output vector will be              \n");
        mexPrintf("                          computed                                     \n");
        mexPrintf("    packetStartIndex    : vector with start indices (in input          \n");
        mexPrintf("                          IQ line) of each packet; a velocity,         \n");
        mexPrintf("                          velocity standard deviation, and a           \n");
        mexPrintf("                          power spectrum are computed for              \n");
        mexPrintf("                          each packet                                  \n");
        mexPrintf("    packetSize          : amount of subsequent samples in an           \n");
        mexPrintf("                          IQ line from which one output                \n");
        mexPrintf("                          vector element will be computed              \n");
        mexPrintf("    samplesPerWave      : RF sample rate divided by ultrasound         \n");
        mexPrintf("                          frequency; must equal 4                      \n");
        mexPrintf("  [S, V, sigmaV] = mxSpectralDoppler( 'RunOnRfData', inMatrix );       \n");
        mexPrintf("    inMatrix            : one RF line per matrix column with           \n");
        mexPrintf("                          numSamplesPerRfLine elements per             \n");
        mexPrintf("                          row                                          \n");
        mexPrintf("    S                   : matrix with power spectrum of                \n");
        mexPrintf("                          Doppler signal for each packet               \n");
        mexPrintf("                          per row                                      \n");
        mexPrintf("    V                   : vector with mean velocity per packet         \n");
        mexPrintf("    sigmaV              : vector with std velocity per packet          \n");
        mexPrintf("  [S, V, sigmaV] = mxSpectralDoppler( 'RunOnIqData', inMatrix );       \n");
        mexPrintf("    inMatrix            : one IQ line per matrix column with           \n");
        mexPrintf("                          at least max(packetStartIndex) +             \n");
        mexPrintf("                          packetSize elements per row                  \n");
        mexPrintf("    S                   : matrix with power spectrum of                \n");
        mexPrintf("                          Doppler signal for each packet               \n");
        mexPrintf("                          per row                                      \n");
        mexPrintf("    V                   : vector with mean velocity per packet         \n");
        mexPrintf("    sigmaV              : vector with std velocity per packet          \n");
        mexPrintf("  mxSpectralDoppler( 'Destroy' );                                      \n");
		mexPrintf("  [valid] = mxSpectralDoppler( 'Exist' );                              \n");
		mexPrintf("    valid               : boolean to indicate whether an               \n");
		mexPrintf("                          instance has been created                    \n");
		mexPrintf("=======================================================================\n\n");
    }
    else if (strcmp(sMethod, "Example") == 0)
    {
		mexPrintf("  numSamplesPerRfLine = 16;											  \n");
		mexPrintf("  ensembleSize = 256;      											  \n");
		mexPrintf("  packetStartIndex = [1,3];											  \n");
		mexPrintf("  packetSize = 3;          											  \n");
		mexPrintf("  samplesPerWave = 4;      											  \n");
		mexPrintf("  phaseShift = pi/5; %%[pi/5, -3*pi/5];					     		  \n");
		mexPrintf("  inMatrix = zeros(numSamplesPerRfLine, ensembleSize);      			  \n");
		mexPrintf("  for n = 1 : ensembleSize      		  								  \n");
		mexPrintf("      for m = 1 : length(phaseShift)      							  \n");
		mexPrintf("          inMatrix(:, n) = inMatrix(:, n) + sin((pi/2) * (0:numSamplesPerRfLine-1)' - n*phaseShift(m) );			  \n");
		mexPrintf("      end      														  \n");
		mexPrintf("  end        														  \n");
		mexPrintf("  figure, plot(inMatrix)        										  \n");
		mexPrintf("  mxSpectralDoppler('Create', numSamplesPerRfLine, ensembleSize, packetStartIndex, packetSize, samplesPerWave);	  \n");  
		mexPrintf("  [S, V, sigmaV] = mxSpectralDoppler('RunOnRfData', inMatrix);		  \n");
		mexPrintf("  mxSpectralDoppler('Destroy');										  \n");
		mexPrintf("  figure, plot(V),													  \n");
		mexPrintf("  title(['expected value for elements of V : ', num2str(phaseShift)]); \n");
		mexPrintf("  xlabel('element index')											  \n");
		mexPrintf("  ylabel('V: estimated phase shift between input lines')				  \n");
		mexPrintf("  Sc = circshift(S, [0, ensembleSize/2]);							  \n");
		mexPrintf("  fax = ((0 : (ensembleSize-1)) * 2*pi/ensembleSize) - pi;			  \n");
		mexPrintf("  for packetIdx = 1 : length(packetStartIndex)						  \n");
		mexPrintf("      figure;														  \n");
		mexPrintf("      plot(fax,Sc(packetIdx,:));										  \n");
		mexPrintf("      xlabel('Frequency');											  \n");
		mexPrintf("      ylabel('S: Doppler power spectrum');							  \n");
		mexPrintf("  end																  \n");
    }
    else if (strcmp(sMethod, "Version") == 0)
    {
        char versionString[] = "mxSpectralDoppler version: 255.255.255.255";
        plhs[0] = mxCreateString(versionString);
    }
    else if (strcmp(sMethod, "Exist") == 0)
	{
		plhs[0] = mxCreateLogicalScalar(pInst != NULL);
	}
	else if (strcmp(sMethod, "Create") == 0)
    {
        int i;
        double * pDoubleMexMatrixInPacketStartIndex;
        //---------------------------------------------------------------------
        // mxSpectralDopplerCreate( int numSamplesPerRfLine, int ensembleSize, int * pPacketStartIndex, int packetSize, int samplesPerWave ); 
        //---------------------------------------------------------------------

        if (pInst != NULL)
        {
            mexErrMsgTxt("mxSpectralDoppler Create: you may only Create one instance, call method Destroy first");
        }

        // verify input arguments (i.e. prhs[n], n=1,2,3,...)
        if (nrhs != 1 + 5)
        {
            mexErrMsgTxt("mxSpectralDoppler Create: method requires 5 input arguments");
        }
        for (i = 1; i < nrhs; i++)
        {
            if (!mxIsDouble(prhs[i]) ||
                mxIsComplex(prhs[i]) || 
                (i != 3 && mxGetNumberOfElements(prhs[i]) != 1))
            {
                mexErrMsgTxt("mxSpectralDoppler Create: inputs must be real and - except from packetStartIndex - scalar");
            }
        }
        // call the C-algorithm Create
        {
            int numSamplesPerRfLine = (int)mxGetScalar(prhs[1]);
            int ensembleSize = (int)mxGetScalar(prhs[2]);
            int packetSize = (int)mxGetScalar(prhs[4]);
            int samplesPerWave = (int)mxGetScalar(prhs[5]);
            // using mex API to get a double pointer to the packetStartIndex vector in the MatLab workspace
            int numPackets = (int)mxGetNumberOfElements(prhs[3]);
            // get int* to packetStartIndex array
            pDoubleMexMatrixInPacketStartIndex = mxGetPr(prhs[3]); 
            // allocate memory
            pPacketStartIndex = iusAllocIntVector( numPackets );
            // copy the values
            iusCopyDoubleVector2IntVector( pPacketStartIndex, pDoubleMexMatrixInPacketStartIndex, numPackets );


            if (ensembleSize <= 1)
            {
                mexErrMsgTxt("mxSpectralDoppler Create: ensembleSize must be larger than 1");
            }
            if (samplesPerWave != 4)
            {
                mexErrMsgTxt("mxSpectralDoppler Create: samplesPerWave must euqal 4");
            }
            if (packetSize > numSamplesPerRfLine)
            {
                mexErrMsgTxt("mxSpectralDoppler Create: packetSize larger than numSamplesPerRfLine makes no sense");
            }

            pInst = iusSpectralDopplerCreate( numSamplesPerRfLine, ensembleSize, pPacketStartIndex, numPackets, packetSize, samplesPerWave );
            // initialize the input and output arrays
            ppRfLinesIn         = iusAllocFloatMatrix( ensembleSize, numSamplesPerRfLine );
            ppPowerSpectrumOut  = iusAllocFloatMatrix( numPackets, ensembleSize );
            pVOut               = iusAllocFloatVector( numPackets );
            pSigmaVOut          = iusAllocFloatVector( numPackets );
        }
        
    }
    else if (strcmp(sMethod, "RunOnRfData") == 0)
    {
        //---------------------------------------------------------------------
        // [S, V, sigmaV] = mxSpectralDoppler( 'RunOnRfData', inMatrix );
        //---------------------------------------------------------------------
        int         numLines, numSamplesPerRfLine;
        double *    pDblMexMatrixPowerSpectrumOut;
        double *    pDblMexMatrixVOut;
        double *    pDblMexMatrixSigmaVOut;
        int			i, j;

        if (pInst == NULL)
        {
            mexErrMsgTxt("mxSpectralDoppler RunOnRfData: you try to call RunOnRfData method before calling Create.");
        }
        IUS_ASSERT_MEMORY( ppRfLinesIn != NULL );
        IUS_ASSERT_MEMORY( ppPowerSpectrumOut != NULL );
        IUS_ASSERT_MEMORY( pVOut != NULL );
        IUS_ASSERT_MEMORY( pSigmaVOut != NULL );


        // verify input arguments (i.e. prhs[n], n=1,2,3,...)
        if (nrhs != 1 + 1)
        {
            mexErrMsgTxt("mxSpectralDoppler RunOnRfData: method requires 1 input argument");
        }
        if (!(mxIsDouble(prhs[1]) || mxIsSingle(prhs[1])) ||
            mxIsComplex(prhs[1]) ||
            mxGetNumberOfElements(prhs[1]) == 1)
        {
            mexErrMsgTxt("mxSpectralDoppler RunOnRfData: input must be a real double or single matrix");
        }
        if (nlhs != 3)
        {
            mexErrMsgTxt("mxSpectralDoppler RunOnRfData: method requires 3 output arguments");
        }

        // Get input array dimensions provided from the MatLab workspace
        numLines            = (int)mxGetN(prhs[1]);     // nr of rows of inputMatrix
        numSamplesPerRfLine = (int)mxGetM(prhs[1]);     // nr of cols of inputMatrix

        if (numSamplesPerRfLine != pInst->numSamplesPerRfLine)
        {
            mexErrMsgTxt("mxSpectralDoppler RunOnRfData: nr samples per RF line different from Create");
        }
        if (numLines != pInst->ensembleSize)
        {
            mexErrMsgTxt("mxSpectralDoppler RunOnRfData: nr lines (ensemble size) different from Create");
        }

		if (mxIsDouble(prhs[1]))
		{
			// using mex API to get a double pointer to the input/output matrix in the MatLab workspace
			double *    pDblMexMatrixRFDataIn = mxGetPr(prhs[1]); // get double* to input array
			for (i = 0; i < numLines; i++)
			{
				// cast double input vector coming from matlab workspace to a float vector
				iusCopyDoubleVector2FloatVector(iusMatrixRow(ppRfLinesIn, i),
					pDblMexMatrixRFDataIn + i * numSamplesPerRfLine, numSamplesPerRfLine);
			}
		}
		else // Must be float/single (see earlier check on class)
		{
			float*      pFltMexMatrixRfDataIn = (float*)mxGetData(prhs[1]);
			for (i = 0; i < numLines; i++)
			{
				iusCopyFloatVector(ppRfLinesIn[i], pFltMexMatrixRfDataIn + i * numSamplesPerRfLine, numSamplesPerRfLine);
			}
		}


		plhs[0] = mxCreateDoubleMatrix(pInst->numPackets, pInst->ensembleSize, mxREAL);
		pDblMexMatrixPowerSpectrumOut = mxGetPr(plhs[0]); // get double* to output array
		plhs[1] = mxCreateDoubleMatrix(pInst->numPackets, 1, mxREAL);
		pDblMexMatrixVOut = mxGetPr(plhs[1]); // get double* to output array
		plhs[2] = mxCreateDoubleMatrix(pInst->numPackets, 1, mxREAL);
		pDblMexMatrixSigmaVOut = mxGetPr(plhs[2]); // get double* to output array

        // Run spectral Doppler on RF data
        iusSpectralDopplerRunOnRfData( pInst, ppRfLinesIn, numLines, numSamplesPerRfLine, ppPowerSpectrumOut, pVOut, pSigmaVOut, pInst->numPackets );

        // cast float output vector to a double vector going to matlab workspace as a column of outputmatrix
		for (i = 0; i < pInst->ensembleSize; i++)
		{
			for (j = 0; j < pInst->numPackets; j++)
			{
				*(pDblMexMatrixPowerSpectrumOut + i*pInst->numPackets + j) = (double)ppPowerSpectrumOut[j][i];
			}
		}
		iusCopyFloatVector2DoubleVector(pDblMexMatrixVOut, pVOut, pInst->numPackets);
        iusCopyFloatVector2DoubleVector( pDblMexMatrixSigmaVOut, pSigmaVOut, pInst->numPackets );

	}
    else if (strcmp(sMethod, "RunOnIqData") == 0)
    {
        //---------------------------------------------------------------------
        // [S, V, sigmaV] = mxSpectralDoppler( 'RunOnIqData', inMatrix );
        //---------------------------------------------------------------------
        int         numLines, numSamplesPerIqLine;
		double *    pDblMexMatrixPowerSpectrumOut;
        double *    pDblMexMatrixVOut;
        double *    pDblMexMatrixSigmaVOut;
        int			i, j;

        if (pInst == NULL)
        {
            mexErrMsgTxt("mxSpectralDoppler RunOnIqData: you try to call RunOnIqData method before calling Create.");
        }
        IUS_ASSERT_MEMORY( pInst->pIqData != NULL ); // We won't need a new array
        IUS_ASSERT_MEMORY( ppPowerSpectrumOut != NULL );
        IUS_ASSERT_MEMORY( pVOut != NULL );
        IUS_ASSERT_MEMORY( pSigmaVOut != NULL );


        // verify input arguments (i.e. prhs[n], n=1,2,3,...)
        if (nrhs != 1 + 1)
        {
            mexErrMsgTxt("mxSpectralDoppler RunOnIqData: method requires 1 input argument");
        }
        if  (!(mxIsDouble(prhs[1]) || mxIsSingle(prhs[1])) ||
			 !mxIsComplex(prhs[1]) ||
             mxGetNumberOfElements(prhs[1]) == 1)
        {
            mexErrMsgTxt("mxSpectralDoppler RunOnIqData: input must be a complex double or complex single matrix");
        }
        if (nlhs != 3)
        {
            mexErrMsgTxt("mxSpectralDoppler RunOnIqData: method requires 3 output arguments");
        }

        // Get input array dimensions provided from the MatLab workspace
        numLines            = (int)mxGetN(prhs[1]);     // nr of cols of inputMatrix
		numSamplesPerIqLine = (int)mxGetM(prhs[1]);     // nr of rows of inputMatrix

        if (numSamplesPerIqLine != pInst->numSamplesPerIqLine)
        {
            mexErrMsgTxt("mxSpectralDoppler RunOnIqData: nr samples per IQ line different from Create");
        }
        if (numLines != pInst->ensembleSize)
        {
            mexErrMsgTxt("mxSpectralDoppler RunOnIqData: nr lines (ensemble size) different from Create");
        }

		if (mxIsDouble(prhs[1]))
		{
			// using mex API to get a double pointer to the input/output matrix in the MatLab workspace
			double *    pDblMexMatrixIDataIn = mxGetPr(prhs[1]); // get double* to input array
			double *    pDblMexMatrixQDataIn = mxGetPi(prhs[1]); // get double* to input array
			for (i = 0; i < numLines; i++)
			{
				// cast double input vector coming from matlab workspace to a float vector
				iusCopyDoubleVector2FloatVector( iusMatrixRow(pInst->pIqData->ppRe, i),
					pDblMexMatrixIDataIn + i * numSamplesPerIqLine, numSamplesPerIqLine );
				// cast double input vector coming from matlab workspace to a float vector
				iusCopyDoubleVector2FloatVector( iusMatrixRow(pInst->pIqData->ppIm, i),
					pDblMexMatrixQDataIn + i * numSamplesPerIqLine, numSamplesPerIqLine );
			}
		}
		else // Must be float/single (see earlier check on class)
		{
			float*      pFltMexMatrixIDataIn = (float*)mxGetData(prhs[1]);
			float*      pFltMexMatrixQDataIn = (float*)mxGetImagData(prhs[1]);
			for (i = 0; i < numLines; i++)
			{
				// No type casting needed; just copy the data to pInst->pIqData
				iusCopyFloatVector(pInst->pIqData->ppRe[i], pFltMexMatrixIDataIn + i * numSamplesPerIqLine, numSamplesPerIqLine);
				iusCopyFloatVector(pInst->pIqData->ppIm[i], pFltMexMatrixQDataIn + i * numSamplesPerIqLine, numSamplesPerIqLine);
			}
		}

        plhs[0] = mxCreateDoubleMatrix(pInst->numPackets, pInst->ensembleSize, mxREAL);
        pDblMexMatrixPowerSpectrumOut = mxGetPr(plhs[0]); // get double* to output array
        plhs[1] = mxCreateDoubleMatrix(pInst->numPackets, 1, mxREAL);
        pDblMexMatrixVOut = mxGetPr(plhs[1]); // get double* to output array
        plhs[2] = mxCreateDoubleMatrix(pInst->numPackets, 1, mxREAL);
        pDblMexMatrixSigmaVOut = mxGetPr(plhs[2]); // get double* to output array

		// Run spectral Doppler on IQ data
		iusSpectralDopplerRunOnIqData( pInst, pInst->pIqData, numLines, numSamplesPerIqLine, ppPowerSpectrumOut, pVOut, pSigmaVOut, pInst->numPackets );

		// cast float output vector to a double vector going to matlab workspace as a column of outputmatrix
		for (i = 0; i < pInst->ensembleSize; i++)
		{
			for (j = 0; j < pInst->numPackets; j++)
			{
				*(pDblMexMatrixPowerSpectrumOut + i*pInst->numPackets + j) = (double)ppPowerSpectrumOut[j][i];
			}
		}
		iusCopyFloatVector2DoubleVector(pDblMexMatrixVOut, pVOut, pInst->numPackets);
		iusCopyFloatVector2DoubleVector( pDblMexMatrixSigmaVOut, pSigmaVOut, pInst->numPackets );

    }
    else if (strcmp(sMethod, "Destroy") == 0)
    {
        // Free the algorithm
        if (pInst != NULL)
        {

            // Free memory that was allocated
            iusFreeIntVector( pPacketStartIndex );
            iusFreeFloatMatrix( ppRfLinesIn, pInst->ensembleSize );
            iusFreeFloatMatrix( ppPowerSpectrumOut, pInst->numPackets );
            iusFreeFloatVector( pVOut );
            iusFreeFloatVector( pSigmaVOut );

            iusSpectralDopplerDestroy( pInst );
            pInst = NULL;

        }
        else
        {
            mexWarnMsgTxt("mxSpectralDoppler: Destroy: nothing to do.");
        }
    }
	else if (strcmp(sMethod, "SetPacket") == 0)
	{
		double * pDoubleMexMatrixInPacketStartIndex;
		//---------------------------------------------------------------------
		// mxSpectralDoppler( 'SetPacket', int * pPacketStartIndex, int packetSize ); 
		//---------------------------------------------------------------------

		if (pInst == NULL)
		{
			mexErrMsgTxt("mxSpectralDoppler SetPacket: you try to call SetPacket method before calling Create.");
		}

		// verify input arguments (i.e. prhs[n], n=1,2,3,...)
		if (nrhs != 1 + 2)
		{
			mexErrMsgTxt("mxSpectralDoppler SetPacket: method requires 2 input arguments");
		}
		if (!mxIsDouble(prhs[1]) || mxIsComplex(prhs[1]) || mxGetNumberOfElements(prhs[1]) != pInst->numPackets)
		{
			mexErrMsgTxt("mxSpectralDoppler SetPacket: packetStartIndex should be real and have numPackets values");
		}
		if (!mxIsDouble(prhs[2]) || mxIsComplex(prhs[2]) || mxGetNumberOfElements(prhs[2]) != 1)
		{
			mexErrMsgTxt("mxSpectralDoppler SetPacket: packetSize should be real and scalar");
		}
		// call the C-algorithm Create
		{
			// using mex API to get a double pointer to the packetStartIndex vector in the MatLab workspace
			int numPackets = (int)mxGetNumberOfElements(prhs[1]);
			// get int* to packetStartIndex array
			pDoubleMexMatrixInPacketStartIndex = mxGetPr(prhs[1]); 
			int packetSize = (int)mxGetScalar(prhs[2]);
			// allocate memory
			pPacketStartIndex = iusAllocIntVector( numPackets );
			// copy the values
			iusCopyDoubleVector2IntVector( pPacketStartIndex, pDoubleMexMatrixInPacketStartIndex, numPackets );

			// call setPacket function
			iusSpectralDopplerSetPacket( pInst, pPacketStartIndex, numPackets, packetSize );
		}

	}
	else
    {
        mexErrMsgTxt("mxSpectralDoppler: Invalid method string.");
    }

    mxFree(sMethod);
}