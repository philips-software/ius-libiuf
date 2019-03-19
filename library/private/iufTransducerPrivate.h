//
// Created by Ruijzendaal on 24/05/2018.
//
#ifndef IUFLIBRARY_IUFHLTRANSDUCERIMP_H
#define IUFLIBRARY_IUFHLTRANSDUCERIMP_H

#include <iufTransducer.h>
#include <hdf5.h>
#include <hdf5_hl.h>

int iufBaseTransducerDelete
(
	iut_t transducer
);

int iufBaseTransducerCompare
(
    iut_t reference,
    iut_t actual
);

herr_t iufTransducerSave
(
	iut_t transducer,
	hid_t handle
);

iut_t iufTransducerLoad
(
    hid_t handle
);

herr_t iufBaseTransducerSave
(
    iut_t transducer,
    hid_t handle
);

iut_t iufBaseTransducerLoad
(
    hid_t handle
);



#endif //IUFLIBRARY_IUFHLTRANSDUCERIMP_H
