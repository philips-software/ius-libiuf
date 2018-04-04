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
#include <include/iusHLPosition.h>


iut_t iusHLCreateTransducer
(
    char *name,
    IusTransducerShape shape,
    float centerFrequency,
    int numElements
)
{
    IusTransducer *baseTransducer = NULL;
    IusShape type ;

    // Identify transducer type based on shape
    if(shape == IUS_LINE || shape == IUS_CIRCLE)
    {
        type = IUS_2D_SHAPE;
        // Create 2D transducer
        Ius2DTransducer * _2DTransducer = (Ius2DTransducer *)calloc(1,sizeof(Ius2DTransducer));
        _2DTransducer->pElements = (Ius2DTransducerElement *)calloc(numElements,sizeof(Ius2DTransducerElement));
        baseTransducer = (IusTransducer *)_2DTransducer;
    }
    else
    {
        type = IUS_3D_SHAPE;
        // Create 3D transducer
        Ius3DTransducer * _3DTransducer = (Ius3DTransducer *)calloc(1,sizeof(Ius3DTransducer));
        _3DTransducer->pElements = (Ius3DTransducerElement *)calloc(numElements,sizeof(Ius3DTransducerElement));
        baseTransducer = (IusTransducer *)_3DTransducer;
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


IusTransducerShape iusHLTransducerGetShape
    (
        iut_t transducer
    )
{
    return transducer->shape;
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
    iusSize->sz=sz;
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
    iusSize->sy=sy;
    iusSize->sz=sz;
    return iusSize;
}

void iusHLDelete3DElementSize(Ius3DSize *iusSize)
{
    free(iusSize);
}


Ius2DTransducerElement *iusHLCreate2DElement
(
    Ius2DPosition *pos,
    float theta,
    Ius2DSize *siz
)
{
    Ius2DTransducerElement *iusElement = calloc(1, sizeof(Ius2DTransducerElement));
    iusElement->theta = theta;
    iusElement->position = *pos;
    iusElement->size = *siz;
    iusElement->base.shape = IUS_2D_SHAPE;
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
    iusElement->base.shape = IUS_3D_SHAPE;
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

int iusHLTransducerSetElement
    (
        IusTransducer *pTransducer,
        int elementIndex,
        void *element
    )
{
    int status = IUS_ERR_VALUE;
    IusBaseTransducerElement *pBaseElement = (IusBaseTransducerElement*) element;
    if( pTransducer->type == IUS_3D_SHAPE && pBaseElement->shape == IUS_3D_SHAPE )
    {
        Ius3DTransducer *castedTransducer = (Ius3DTransducer *)pTransducer;
        Ius3DTransducerElement *castedTransducerElement = (Ius3DTransducerElement *)element;
        status = ius3DTransducerAddElement(
        castedTransducer,
        elementIndex,
        castedTransducerElement
        );
    }

    if( pTransducer->type == IUS_2D_SHAPE && pBaseElement->shape == IUS_2D_SHAPE )
    {
        Ius2DTransducer *castedTransducer = (Ius2DTransducer *)pTransducer;
        Ius2DTransducerElement *castedTransducerElement = (Ius2DTransducerElement *)element;
        status = ius2DTransducerAddElement(
        castedTransducer,
        elementIndex,
        castedTransducerElement
        );
    }

    return status;
}

IusBaseTransducerElement * iusHLTransducerGetElement
    (
        IusTransducer *transducer,
        int elementIndex
    )
{
    if( elementIndex >=  transducer->numElements)
        return NULL;
    if( transducer->type == IUS_3D_SHAPE  )
    {
        Ius3DTransducer *castedTransducer = (Ius3DTransducer *)transducer;
        return (IusBaseTransducerElement *)&castedTransducer->pElements[elementIndex];
    }

    if( transducer->type == IUS_2D_SHAPE  )
    {
        Ius2DTransducer *castedTransducer = (Ius2DTransducer *)transducer;
        return (IusBaseTransducerElement *)&castedTransducer->pElements[elementIndex];
    }
    return NULL;
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
    if(reference->theta != actual->theta)
        return IUS_FALSE;
    return IUS_TRUE;
}

IUS_BOOL iusHLTransducerCompareElement
    (
        IusBaseTransducerElement *reference,
        IusBaseTransducerElement *actual
    )
{
    if( reference->shape != actual->shape )
        return IUS_FALSE;

    if( reference->shape == IUS_3D_SHAPE  )
    {
        return iusCompare3DElement((Ius3DTransducerElement *)reference,(Ius3DTransducerElement *)actual);
    }

    if( reference->shape == IUS_2D_SHAPE  )
    {
        return iusCompare2DElement((Ius2DTransducerElement *)reference,(Ius2DTransducerElement *)actual);
    }
    return IUS_FALSE;
}

IusShape iusHLTransducerGetShapeType
(
    IusTransducer *baseElement
)
{
    return baseElement->type;
}

IusShape iusHLTransducerGetElementShape
(
    iudte_t baseElement
)
{
    return baseElement->shape;
}

IUS_BOOL iusHLCompare3DTransducer(Ius3DTransducer *pReference, Ius3DTransducer *pActual)
{
    IUS_BOOL isEqual=IUS_TRUE;
    for (int i = 0; i < pReference->baseTransducer.numElements ; ++i)
    {
        isEqual = iusCompare3DElement(&pReference->pElements[i], &pActual->pElements[i]);
        if( isEqual == IUS_FALSE ) break;
    }
    return isEqual;
}

IUS_BOOL iusHLCompare2DTransducer(Ius2DTransducer *pReference, Ius2DTransducer *pActual)
{
    IUS_BOOL isEqual=IUS_TRUE;
    for (int i = 0; i < pReference->baseTransducer.numElements ; ++i)
    {
        isEqual = iusCompare2DElement(&pReference->pElements[i], &pActual->pElements[i]);
        if( isEqual == IUS_FALSE ) break;
    }
    return isEqual;
}

IUS_BOOL iusHLTransducerCompare
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
    if( reference->type == IUS_3D_SHAPE  )
        return iusHLCompare3DTransducer((Ius3DTransducer *)reference, (Ius3DTransducer *)actual);
    if( reference->type == IUS_2D_SHAPE  )
        return iusHLCompare2DTransducer((Ius2DTransducer *)reference, (Ius2DTransducer *)actual);
    return isEqual;
}
