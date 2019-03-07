//
// Created by Ruijzendaal on 30/05/2018.
//

#ifndef IUSLIBRARY_IUSHLPATTERNLISTDICTIMP_H
#define IUSLIBRARY_IUSHLPATTERNLISTDICTIMP_H

#include <hdf5.h>
#include <iusPatternListDict.h>

int iusPatternListDictSave
(
	iupald_t dict,
	hid_t handle
);

iupald_t iusPatternListDictLoad
(
	hid_t handle
);


#endif //IUSLIBRARY_IUSHLPATTERNLISTDICTIMP_H
