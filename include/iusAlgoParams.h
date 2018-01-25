//=============================================================================
//    COPYRIGHT 2018 PHILIPS RESEARCH
//=============================================================================
//
//  Source Name   : iusAlgoParams.h
//                  IUS algorithm parameter access
//  iusVersion    : 255.255.255.255
//
//=============================================================================

#ifndef IUS_ALGOPARAMS_H
#define IUS_ALGOPARAMS_H

#include <hdf5.h>

#define MAX_ALGO_PARAMETERS 12

enum IusAlgorithmParameterType
{
	IUS_ALGOPARAMT_NONE = 0,
	IUS_ALGOPARAMT_INT,
	IUS_ALGOPARAMT_FLOAT,
	IUS_ALGOPARAMT_STRING
};

typedef struct _algo_param_type
{
	char *                         pName;
	enum IusAlgorithmParameterType type;
	int                            length;
	void *                         pValue;
} IusAlgorithmParameter;

typedef struct _algo_params_type
{
	char *                  pId;
	char *                  pName;
	char *                  pDescription;

	int                     numberOfParameters;
	IusAlgorithmParameter * pParameters[MAX_ALGO_PARAMETERS];
} IusAlgorithmParameters; 

// Only ever use these methods to interact with IusAlgorithmParameters objects.

// <Create/Destroy>
IusAlgorithmParameters * iusAlgorithmParametersCreate
(
    const char * const pId,
    const char * const pName,
    const char * const pDescription
);

void iusAlgorithmParametersDestroy
(
    IusAlgorithmParameters * pInst
);

// <Write>
void iusAlgorithmParametersAddInt
(
    IusAlgorithmParameters * const pInst,
    const char * const             pName,
    int                            value
);

void iusAlgorithmParametersAddFloat
(
    IusAlgorithmParameters * const pInst,
    const char * const             pName,
    float                          value
);

void iusAlgorithmParametersAddString
(
    IusAlgorithmParameters * const pInst,
    const char * const             pName,
    const char * const             pValue
);

// <Read>
int iusAlgorithmParametersReadInt
(
    const IusAlgorithmParameters * const pInst,
    const char * const                   pName
);

float iusAlgorithmParametersReadFloat
(
    const IusAlgorithmParameters * const pInst,
    const char * const                   pName
);

void iusAlgorithmParametersReadString
(
    const IusAlgorithmParameters * const pInst,
    const char * const                   pName,
    char * const                         pBuffer
);

// <Serialization>
IusAlgorithmParameters * iusAlgorithmParametersRead
(
    hid_t handle,
    int   verbose
);

void iusAlgorithmParametersWrite
(
    const IusAlgorithmParameters * const pInst,
    hid_t                                handle
);

#endif
