//
// Created by Ruijzendaal on 26/07/2018.
//

#ifndef IUSLIBRARY_IUSHLSOURCEIMP_H
#define IUSLIBRARY_IUSHLSOURCEIMP_H

#include <iusTypes.h>
#include <iusSource.h>
#include <hdf5.h>
#include <hdf5_hl.h>


struct IusSource
{
    IusSourceType type;
    int numLocations;      // for 2D and non-parametric 3D

    int numThetaLocations; //for parametric 3D
    int numPhiLocations;   //for parametric 3D

    //for Non parametric
    struct Ius2DPosition *locations2D;
    struct Ius3DPosition *locations3D;
    float fNumber;        /**< distance in [m] of sources to transducer for POLAR */
    float deltaTheta;     /**< angle in [rad] between sources */
    float startTheta;     /**< angle in [rad] between sources */

    float deltaPhi;       /**< angle in [rad] between sources */
    float startPhi;       /**< angle in [rad] between sources */
};

IUS_BOOL iusBaseSourceCompare
(
    ius_t reference,
    ius_t actual
);

int iusBaseSourceSave
(
    ius_t source,
    hid_t handle
);

int iusSourceSave
(
    ius_t source,
    hid_t handle
);

ius_t iusSourceLoad
(
    hid_t handle
);

#endif //IUSLIBRARY_IUSHLSOURCEIMP_H
