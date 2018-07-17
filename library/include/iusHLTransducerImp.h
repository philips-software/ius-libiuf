//
// Created by Ruijzendaal on 24/05/2018.
//
#ifndef IUSLIBRARY_IUSHLTRANSDUCERIMP_H
#define IUSLIBRARY_IUSHLTRANSDUCERIMP_H

#include <iusHDF5.h>
#include <iusHL2DTransducer.h>
#include <iusHL3DTransducer.h>

struct IusTransducer
{
    IusShape               type; /**< 2D or 3D transducer? */
    char *                 pTransducerName;   /**< descriptive name of the ultrasound probe */
    float                  centerFrequency;   /**< operating frequency of the transducer */
    IusTransducerShape     shape;
} ;

herr_t ius3DTransducerWrite
(
	iu3dt_t transducer,
	hid_t subgroup_id,
	int verbose
);

herr_t ius2DTransducerWrite
(
	iu2dt_t transducer,
	hid_t subgroup_id,
	int verbose
);

#endif //IUSLIBRARY_IUSHLTRANSDUCERIMP_H
