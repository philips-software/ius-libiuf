//
// Created by Ruijzendaal on 30/05/2018.
//

#ifndef IUSLIBRARY_IUSHLIQPATTERNLISTDICTIMP_H
#define IUSLIBRARY_IUSHLIQPATTERNLISTDICTIMP_H

#include <hdf5.h>
#include <iusIqPatternListDict.h>

int iusIqPatternListDictSave
(
	iuiqpald_t dict,
	hid_t handle
);

iuiqpald_t iusIqPatternListDictLoad
(
	hid_t handle
);


#endif //IUSLIBRARY_IUSHLIQPATTERNLISTDICTIMP_H
