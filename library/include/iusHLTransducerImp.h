//
// Created by Ruijzendaal on 24/05/2018.
//
#ifndef IUSLIBRARY_IUSHLTRANSDUCERIMP_H
#define IUSLIBRARY_IUSHLTRANSDUCERIMP_H

#include <iusHDF5.h>
#include <iusHLTransducer.h>
#include <iusHL3DTransducer.h>

struct IusTransducer
{
    IusShape               type; /**< 2D or 3D transducer? */
    char *                 pTransducerName;   /**< descriptive name of the ultrasound probe */
    float                  centerFrequency;   /**< operating frequency of the transducer */
    IusTransducerShape     shape;
} ;

int iusHLBaseTransducerCompare
(
    iut_t reference,
    iut_t actual
);

herr_t iusHLTransducerSave
(
	iut_t transducer,
	hid_t handle
);

iut_t iusHLTransducerLoad
(
    hid_t handle
);

herr_t iusHLBaseTransducerSave
(
    iut_t transducer,
    hid_t handle
);

iut_t iusHLBaseTransducerLoad
(
    hid_t handle
);



#endif //IUSLIBRARY_IUSHLTRANSDUCERIMP_H
