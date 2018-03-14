//
// Created by Ruijzendaal on 14/03/2018.
//

#include <include/iusHDF5.h>
#include <stdlib.h>
#include <math.h>
#include "iusUtil.h"
#define EPSILON 0.0000001

int iusHLDiagDisable()
{
    return iusHdf5DisableMessages();
}
