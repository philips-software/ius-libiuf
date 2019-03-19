//
// Created by Ruijzendaal on 30/05/2018.
//

#ifndef IUFLIBRARY_IUFHLPATTERNLISTDICTIMP_H
#define IUFLIBRARY_IUFHLPATTERNLISTDICTIMP_H

#include <hdf5.h>
#include <iufPatternListDict.h>

int iufPatternListDictSave
(
	iupald_t dict,
	hid_t handle
);

iupald_t iufPatternListDictLoad
(
	hid_t handle
);


#endif //IUFLIBRARY_IUFHLPATTERNLISTDICTIMP_H
