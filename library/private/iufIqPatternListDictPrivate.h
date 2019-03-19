//
// Created by Ruijzendaal on 30/05/2018.
//

#ifndef IUFLIBRARY_IUFHLIQPATTERNLISTDICTIMP_H
#define IUFLIBRARY_IUFHLIQPATTERNLISTDICTIMP_H

#include <hdf5.h>
#include <iufIqPatternListDict.h>

int iufIqPatternListDictSave
(
	iuiqpald_t dict,
	hid_t handle
);

iuiqpald_t iufIqPatternListDictLoad
(
	hid_t handle
);


#endif //IUFLIBRARY_IUFHLIQPATTERNLISTDICTIMP_H
