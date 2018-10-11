//
// Created by Ruijzendaal on 24/05/2018.
//
#ifndef IUSLIBRARY_IUSHLTRANSDUCERIMP_H
#define IUSLIBRARY_IUSHLTRANSDUCERIMP_H

#include <iusHDF5.h>
#include <iusTransducer.h>
#include <ius3DTransducer.h>

struct IusTransducer
{
    IusShape               type;			  /**< 2D or 3D transducer? */
    char *                 pTransducerName;   /**< descriptive name of the ultrasound probe */
    float                  centerFrequency;   /**< operating frequency of the transducer */
    IusTransducerShape     shape;
} ;

int iusBaseTransducerCompare
(
    iut_t reference,
    iut_t actual
);

herr_t iusTransducerSave
(
	iut_t transducer,
	hid_t handle
);

iut_t iusTransducerLoad
(
    hid_t handle
);

herr_t iusBaseTransducerSave
(
    iut_t transducer,
    hid_t handle
);

iut_t iusBaseTransducerLoad
(
    hid_t handle
);



#endif //IUSLIBRARY_IUSHLTRANSDUCERIMP_H
