//
// Created by Ruijzendaal on 15/03/2018.
//
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>
#include <memory.h>

#include <iusHLTransducer.h>
#include <include/iusTypes.h>
#include <include/iusError.h>
#include <include/iusUtil.h>
#include <include/iusHLPosition.h>
#include <include/iusHL2DTransducerElement.h>
#include <include/iusHL3DTransducerElement.h>
#include <include/iusHL3DTransducerElementList.h>
#include <include/iusHL2DTransducerElementList.h>
#include <include/iusHL3DTransducer.h>
#include <include/iusHL2DTransducer.h>

/** \brief a Transducer object */


iut_t iusHLTransducerAlloc
(
  void
)
{
  IusTransducer *transducer =  (IusTransducer *)calloc(1,sizeof(IusTransducer));
  return transducer;
}


Ius3DAngle *iusHL3DAngleCreate
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

Ius2DSize *iusHL2DSizeCreate
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


Ius3DSize *iusHL3DSizeCreate
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


int iusHLTransducerDelete
(
iut_t transducer
)
{
    if(transducer == NULL) return IUS_ERR_VALUE;
    free(transducer);
    return 0;
}

void IusHLDelete3DElementAngle(Ius3DAngle * iusAngle)
{
    free(iusAngle);
}

void iusHLDelete3DElementSize(Ius3DSize *iusSize)
{
    free(iusSize);
}

void iusHLDelete3DElement(Ius3DTransducerElement *iusElement)
{
    free(iusElement);
}

// operations

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





IUS_BOOL iusCompare2DSize(Ius2DSize *reference,Ius2DSize *actual)
{
    if (IUS_EQUAL_FLOAT(reference->sx, actual->sx) &&
        IUS_EQUAL_FLOAT(reference->sz, actual->sz) )
        return IUS_TRUE;
    else
        return IUS_FALSE;
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
        return iusHL3DTransducerCompare((iu3dt_t) reference, (iu3dt_t) actual);
    if( reference->type == IUS_2D_SHAPE  )
        return iusHL2DTransducerCompare((iu2dt_t)reference, (iu2dt_t)actual);
    return isEqual;
}


static int ius3DTransducerAddElement
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

static int ius2DTransducerAddElement
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

// getters
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



// setters
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

