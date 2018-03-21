//
// Created by Ruijzendaal on 15/03/2018.
//
#define IUSLIBRARY_IMPLEMENTATION
#include <iusInput.h>
#include <iusHLTransducer.h>
#include <stdlib.h>
#include <include/iusInput.h>
#include <memory.h>
#include <include/iusTypes.h>
#include <include/iusError.h>
#include <math.h>
#include <float.h>
#include <include/iusUtil.h>


iut_t iusHLCreateTransducer
(
    char *name,
    IusTransducerShape shape,
    float centerFrequency,
    int numElements
)
{
    IusTransducer *baseTransducer = NULL;
    IusTransducerType type ;

    // Identify transducer type based on shape
    if(shape == IUS_LINE || shape == IUS_CIRCLE)
    {
        type = IUS_2D_TRANSDUCER;
    }
    else
    {
        type = IUS_3D_TRANSDUCER;
    }


    if(type == IUS_3D_TRANSDUCER)
    {
        // Create 3D transducer
        Ius3DTransducer * _3DTransducer = (Ius3DTransducer *)calloc(1,sizeof(Ius3DTransducer));
        _3DTransducer->pElements = (Ius3DTransducerElement *)calloc(numElements,sizeof(Ius3DTransducerElement));
        baseTransducer = (IusTransducer *)_3DTransducer;
    }

    if(type == IUS_2D_TRANSDUCER)
    {
        // Create 3D transducer
        Ius2DTransducer * _2DTransducer = (Ius2DTransducer *)calloc(1,sizeof(Ius3DTransducer));
        _2DTransducer->pElements = (Ius2DTransducerElement *)calloc(numElements,sizeof(Ius2DTransducerElement));
        baseTransducer = (IusTransducer *)_2DTransducer;
    }

    baseTransducer->type = type;
    baseTransducer->shape = shape;
    baseTransducer->centerFrequency = centerFrequency;
    baseTransducer->numElements = numElements;
    if( name == NULL )
        baseTransducer->pTransducerName = "";
    else
        baseTransducer->pTransducerName = strdup(name);

    return baseTransducer;
}

int iusHLDeleteTransducer
(
    iut_t transducer
)
{
    free(transducer);
    return 0;
}

float iusHLTransducerGetCenterFrequency
(
        iut_t transducer
)
{
    return transducer->centerFrequency;
}

int iusHLTransducerGetNumElements
(
    iut_t transducer
)
{
    return transducer->numElements;
}

char *iusHLTransducerGetName
(
    iut_t transducer
)
{
    return transducer->pTransducerName;
}


IusTransducerShape iusHLGetTransducerShape
    (
        iut_t transducer
    )
{
    return transducer->shape;
}

Ius3DPosition *iusHLCreate3DElementPosition
(
    float x,
    float y,
    float z
)
{
    Ius3DPosition *iusPos = calloc(1, sizeof(Ius3DPosition));
    iusPos->x=x;
    iusPos->y=y;
    iusPos->z=z;
    return iusPos;
}


Ius2DPosition *iusHLCreate2DElementPosition
(
    float x,
    float z
)
{
    Ius2DPosition *iusPos = calloc(1, sizeof(Ius2DPosition));
    iusPos->x=x;
    iusPos->z=z;
    return iusPos;
}

void iusHLDelete3DElementPosition(Ius3DPosition *iusPos)
{
    free(iusPos);
}

Ius3DAngle *iusHLCreate3DElementAngle
(
    float theta,
    float phi
)
{
    Ius3DAngle *iusAngle = calloc(1, sizeof(Ius3DAngle));
    iusAngle->theta=theta;
    iusAngle->phi=phi;
    return iusAngle;
}

void IusHLDelete3DElementAngle(Ius3DAngle * iusAngle)
{
    free(iusAngle);
}



Ius2DSize *iusHLCreate2DElementSize
(
    float sx,
    float sz
)
{
    Ius2DSize *iusSize = calloc(1, sizeof(Ius2DSize));
    iusSize->sx=sx;
    iusSize->sx=sz;
    return iusSize;
}


Ius3DSize *iusHLCreate3DElementSize
(
    float sx,
    float sy,
    float sz
)
{
    Ius3DSize *iusSize = calloc(1, sizeof(Ius3DSize));
    iusSize->sx=sx;
    iusSize->sx=sy;
    iusSize->sx=sz;
    return iusSize;
}

void iusHLDelete3DElementSize(Ius3DSize *iusSize)
{
    free(iusSize);
}


Ius2DTransducerElement *iusHLCreate2DElement
(
    Ius2DPosition *pos,
    float phi,
    Ius2DSize *siz
)
{
    Ius2DTransducerElement *iusElement = calloc(1, sizeof(Ius2DTransducerElement));
    iusElement->phi = phi;
    iusElement->position = *pos;
    iusElement->size = *siz;
    iusElement->base.type = IUS_2D_TRANSDUCER_ELEMENT;
    return iusElement;
}

Ius3DTransducerElement *iusHLCreate3DElement
(
    Ius3DPosition *pos,
    Ius3DAngle *ang,
    Ius3DSize *siz
)
{
    Ius3DTransducerElement *iusElement = calloc(1, sizeof(Ius3DTransducerElement));
    iusElement->angle = *ang;
    iusElement->position = *pos;
    iusElement->size = *siz;
    iusElement->base.type = IUS_3D_TRANSDUCER_ELEMENT;
    return iusElement;
}

void iusHLDelete3DElement(Ius3DTransducerElement *iusElement)
{
    free(iusElement);
}

int ius3DTransducerAddElement
(
    Ius3DTransducer *transducer,
    int elementIndex,
    Ius3DTransducerElement *element
)
{
    if( transducer->pElements == NULL ||
        elementIndex >= transducer->baseTransducer.numElements )
    {
        return -1;
    }
    transducer->pElements[elementIndex]=*element;
    return 0;
}

int ius2DTransducerAddElement
(
    Ius2DTransducer *transducer,
    int elementIndex,
    Ius2DTransducerElement *element
)
{
    if( transducer->pElements == NULL ||
        elementIndex >= transducer->baseTransducer.numElements )
    {
        return -1;
    }
    transducer->pElements[elementIndex]=*element;
    return 0;
}

int iusTransducerSetElement
(
    IusTransducer *transducer,
    int elementIndex,
    void *element
)
{
    int status = IUS_ERR_VALUE;
    IusBaseTransducerElement *pBaseElement = (IusBaseTransducerElement*) element;
    if( transducer->type == IUS_3D_TRANSDUCER && pBaseElement->type == IUS_3D_TRANSDUCER_ELEMENT )
    {
        Ius3DTransducer *castedTransducer = (Ius3DTransducer *)transducer;
        Ius3DTransducerElement *castedTransducerElement = (Ius3DTransducerElement *)element;
        status = ius3DTransducerAddElement(
        castedTransducer,
        elementIndex,
        castedTransducerElement
        );
    }

    if( transducer->type == IUS_2D_TRANSDUCER && pBaseElement->type == IUS_2D_TRANSDUCER_ELEMENT )
    {
        Ius2DTransducer *castedTransducer = (Ius2DTransducer *)transducer;
        Ius2DTransducerElement *castedTransducerElement = (Ius2DTransducerElement *)element;
        status = ius2DTransducerAddElement(
        castedTransducer,
        elementIndex,
        castedTransducerElement
        );
    }

    return status;
}

IusBaseTransducerElement * iusTransducerGetElement
(
    IusTransducer *transducer,
    int elementIndex
)
{
    if( elementIndex >=  transducer->numElements)
        return NULL;
    if( transducer->type == IUS_3D_TRANSDUCER  )
    {
        Ius3DTransducer *castedTransducer = (Ius3DTransducer *)transducer;
        return (IusBaseTransducerElement *)&castedTransducer->pElements[elementIndex];
    }

    if( transducer->type == IUS_2D_TRANSDUCER  )
    {
        Ius2DTransducer *castedTransducer = (Ius2DTransducer *)transducer;
        return (IusBaseTransducerElement *)&castedTransducer->pElements[elementIndex];
    }
    return NULL;
}


IUS_BOOL iusCompare3DPosition(Ius3DPosition *reference,Ius3DPosition *actual)
{
    if (IUS_EQUAL_FLOAT(reference->x, actual->x) &&
        IUS_EQUAL_FLOAT(reference->y, actual->y) &&
        IUS_EQUAL_FLOAT(reference->z, actual->z) )
        return IUS_TRUE;
    else
        return IUS_FALSE;
}

IUS_BOOL iusCompare3DSize(Ius3DSize *reference,Ius3DSize *actual)
{
    if (IUS_EQUAL_FLOAT(reference->sx, actual->sx) &&
        IUS_EQUAL_FLOAT(reference->sy, actual->sy) &&
        IUS_EQUAL_FLOAT(reference->sz, actual->sz) )
        return IUS_TRUE;
    else
        return IUS_FALSE;
}


IUS_BOOL iusCompare3DAngle(Ius3DAngle *reference,Ius3DAngle *actual)
{
    if (IUS_EQUAL_FLOAT(reference->phi, actual->phi) &&
        IUS_EQUAL_FLOAT(reference->theta, actual->theta) )
        return IUS_TRUE;
    else
        return IUS_FALSE;
}

IUS_BOOL iusCompare3DElement
(
    Ius3DTransducerElement *reference,
    Ius3DTransducerElement *actual
)
{
    if(iusCompare3DPosition(&(reference->position),&(actual->position)) != IUS_TRUE)
        return IUS_FALSE;
    if(iusCompare3DSize(&reference->size,&actual->size) != IUS_TRUE)
        return IUS_FALSE;
    if(iusCompare3DAngle(&reference->angle,&actual->angle) != IUS_TRUE)
        return IUS_FALSE;
    return IUS_TRUE;
}



IUS_BOOL iusCompare2DPosition(Ius2DPosition *reference,Ius2DPosition *actual)
{
    if (IUS_EQUAL_FLOAT(reference->x, actual->x) &&
        IUS_EQUAL_FLOAT(reference->z, actual->z) )
        return IUS_TRUE;
    else
        return IUS_FALSE;
}

IUS_BOOL iusCompare2DSize(Ius2DSize *reference,Ius2DSize *actual)
{
    if (IUS_EQUAL_FLOAT(reference->sx, actual->sx) &&
        IUS_EQUAL_FLOAT(reference->sz, actual->sz) )
        return IUS_TRUE;
    else
        return IUS_FALSE;
}

IUS_BOOL iusCompare2DElement
    (
        Ius2DTransducerElement *reference,
        Ius2DTransducerElement *actual
    )
{
    if(iusCompare2DPosition(&reference->position,&actual->position) != IUS_TRUE)
        return IUS_FALSE;
    if(iusCompare2DSize(&reference->size,&actual->size) != IUS_TRUE)
        return IUS_FALSE;
    if(reference->phi != actual->phi)
        return IUS_FALSE;
    return IUS_TRUE;
}

IUS_BOOL iusTransducerCompareElement
(
    IusBaseTransducerElement *reference,
    IusBaseTransducerElement *actual
)
{
    if( reference->type != actual->type )
        return IUS_FALSE;

    if( reference->type == IUS_3D_TRANSDUCER_ELEMENT  )
    {
        return iusCompare3DElement((Ius3DTransducerElement *)reference,(Ius3DTransducerElement *)actual);
    }

    if( reference->type == IUS_2D_TRANSDUCER_ELEMENT  )
    {
        return iusCompare2DElement((Ius2DTransducerElement *)reference,(Ius2DTransducerElement *)actual);
    }
    return IUS_FALSE;
}

IusTransducerElementType iusHLTransducerGetElementType
(
    iudte_t baseElement
)
{
    return baseElement->type;
}

IUS_BOOL iusHLCompare3DTransducer(Ius3DTransducer *pReference, Ius3DTransducer *pActual)
{
    IUS_BOOL isEqual=IUS_TRUE;
    for (int i = 0; i < pReference->baseTransducer.numElements ; ++i)
    {
        IUS_BOOL isEqual = iusCompare3DElement(&pReference->pElements[i], &pActual->pElements[i]);
        if( isEqual == IUS_FALSE ) break;
    }
    return isEqual;
}

IUS_BOOL iusHLCompare2DTransducer(Ius2DTransducer *pReference, Ius2DTransducer *pActual)
{
    IUS_BOOL isEqual=IUS_TRUE;
    for (int i = 0; i < pReference->baseTransducer.numElements ; ++i)
    {
        IUS_BOOL isEqual = iusCompare2DElement(&pReference->pElements[i], &pActual->pElements[i]);
        if( isEqual == IUS_FALSE ) break;
    }
    return isEqual;
}

IUS_BOOL iusHLCompareTransducer
    (
        iut_t reference,
        iut_t actual
    )
{
    IUS_BOOL isEqual = IUS_FALSE;
    if( reference == actual ) return IUS_TRUE;
    if( reference == NULL || actual == NULL )
        return IUS_FALSE;

    if( reference->type != actual->type ) return IUS_FALSE;
    if( reference->shape != actual->shape ) return IUS_FALSE;
    if( IUS_EQUAL_FLOAT(reference->centerFrequency, actual->centerFrequency) == IUS_FALSE ) return IUS_FALSE;
    if( reference->numElements != actual->numElements)  return IUS_FALSE;
    if( strcmp(reference->pTransducerName, actual->pTransducerName) != 0)  return IUS_FALSE;
    if( reference->type == IUS_3D_TRANSDUCER  )
        return iusHLCompare3DTransducer((Ius3DTransducer *)reference, (Ius3DTransducer *)actual);
    if( reference->type == IUS_2D_TRANSDUCER  )
        return iusHLCompare2DTransducer((Ius2DTransducer *)reference, (Ius2DTransducer *)actual);
    return isEqual;
}
