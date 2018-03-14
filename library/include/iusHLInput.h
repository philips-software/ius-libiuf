//
// Created by Ruijzendaal on 12/03/2018.
//

#ifndef IUSLIBRARY_IUSHLINPUT_H
#define IUSLIBRARY_IUSHLINPUT_H

#include <iusTypes.h>
#include <iusHLExperiment.h>

#define IUS_DEFAULT_NUM_FRAMES  1

#ifndef IUSLIBRARY_IMPLEMENTATION
struct IusInputInstance;
typedef  struct IusInputInstance IusInputInstance;
#endif // IUSLIBRARY_IMPLEMENTATION

// ADT
typedef  IusInputInstance * iuh_t;

iuh_t iusHLCreateInputHeader
(
    void
);

int iusHLGetNumFrames
(
    iuh_t header
);

IUS_BOOL iusHLCompareHeader
(
    iuh_t reference_header,
    iuh_t actual_header
);

int iusHLHeaderSetExperiment
(
    iuh_t pInstance,
    iue_t pExperiment
);

iue_t iusHLHeaderGetExperiment
(
        iuh_t pInstance
);

#endif //IUSLIBRARY_IUSHLINPUT_H
