//
// Created by Ruijzendaal on 24/05/2018.
//
#include <stddef.h>

#include <iusHLTransducerFactory.h>
#include <iusHL2DTransducer.h>
#include <iusHL3DTransducer.h>


// Todo:
// nlv09165 - Are we going to use factory methods?
//            if so, also introduce in other ADTs
//            for the sake of consistency
iut_t iusHLTransducerFactoryCreate
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
    iu2dt_t _2DTransducer = iusHL2DTransducerCreate(name,shape,centerFrequency,numElements);
    baseTransducer = (IusTransducer *)_2DTransducer;
  }
  else
  {
    type = IUS_3D_SHAPE;
    // Create 3D transducer
    iu3dt_t _3DTransducer = iusHL3DTransducerCreate(name,shape,centerFrequency,numElements);
    baseTransducer = (IusTransducer *)_3DTransducer;
  }

  return baseTransducer;
}
