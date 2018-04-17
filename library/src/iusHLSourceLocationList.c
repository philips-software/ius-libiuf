//
// Created by Ruijzendaal on 16/04/2018.
//
#define IUSLIBRARY_IMPLEMENTATION
#include <stddef.h>
#include <include/iusTypes.h>
#include <iusInput.h>
#include <include/iusHLSourceLocationList.h>
#include <include/iusInput.h>
#include <stdlib.h>
#include <include/iusError.h>
#include <include/ius.h>


Ius2DSourceLocationList *iusCreate2DSourceLocationList
(
    int numTransmitSources
)
{
    Ius2DSourceLocationList *sourceLocations = calloc(1, sizeof(Ius2DSourceLocationList));
    if(sourceLocations!=NULL)
    {
        sourceLocations->pSourceLocations = calloc(numTransmitSources,sizeof(Ius2DPosition));
    }
    if( sourceLocations->pSourceLocations == NULL )
    {
        return NULL;
    }
    return sourceLocations;
}

IusParametric2DSourceLocationList *iusCreateParametric2DSourceLocationList
(
    int numTransmitSources
)
{
    IusParametric2DSourceLocationList *sourceLocations = calloc(1, sizeof(IusParametric2DSourceLocationList));
    if(sourceLocations!=NULL)
    {
        sourceLocations->base2d.pSourceLocations = calloc(numTransmitSources,sizeof(Ius2DPosition));
    }
    if( sourceLocations->base2d.pSourceLocations == NULL )
    {
        return NULL;
    }
    return sourceLocations;
}



Ius3DSourceLocationList *iusCreate3DSourceLocationList
(
    int numTransmitSources
)
{
    Ius3DSourceLocationList *sourceLocations = calloc(1, sizeof(Ius3DSourceLocationList));
    if(sourceLocations!=NULL)
    {
        sourceLocations->pSourceLocations = calloc(numTransmitSources,sizeof(Ius3DPosition));
    }
    if( sourceLocations->pSourceLocations == NULL )
    {
        return NULL;
    }
    return sourceLocations;
}

IusParametric3DSourceLocationList *iusCreateParametric3DSourceLocationList
(
    int numTransmitSources
)
{
    IusParametric3DSourceLocationList *sourceLocations = calloc(1, sizeof(IusParametric3DSourceLocationList));
    if(sourceLocations!=NULL)
    {
        sourceLocations->base3d.pSourceLocations = calloc(numTransmitSources,sizeof(Ius3DPosition));
    }
    if( sourceLocations->base3d.pSourceLocations == NULL )
    {
        return NULL;
    }
    return sourceLocations;
}

IusSourceLocationList *iusHLCreateSourceLocationList
(
    IusSourceLocationType locationType,
    int numTransmitSources
)
{
    IusSourceLocationList *locationList=NULL;
    if( numTransmitSources <= 0) return IUSLL_INVALID;

    switch(locationType) {
        case IUS_2D_SOURCE_LOCATION:
            locationList = (IusSourceLocationList*) iusCreate2DSourceLocationList(numTransmitSources);

            break;
        case IUS_3D_SOURCE_LOCATION:
            locationList = (IusSourceLocationList*) iusCreate3DSourceLocationList(numTransmitSources);
            break;
        case IUS_PARAMETRIC_2D_SOURCE_LOCATION:
            locationList = (IusSourceLocationList*) iusCreateParametric2DSourceLocationList(numTransmitSources);
            break;
        case IUS_PARAMETRIC_3D_SOURCE_LOCATION:
            locationList = (IusSourceLocationList*) iusCreateParametric3DSourceLocationList(numTransmitSources);
            break;
        default:
            break;
    }

    // base members
    locationList->count=numTransmitSources;
    locationList->locationType=locationType;
    return locationList;
}


int iusHLSourceLocationListSet2DPosition
(
    IusSourceLocationList* pList,
    Ius2DPosition *pPos,
    int index
)
{
    int status = IUS_ERR_VALUE;
    if( pList == NULL ) return IUS_ERR_VALUE;
    if( pPos == NULL ) return IUS_ERR_VALUE;
    if( index >= pList->count ) return IUS_ERR_VALUE;

    if( pList->locationType == IUS_PARAMETRIC_2D_SOURCE_LOCATION ) {
        ((IusParametric2DSourceLocationList *)pList)->base2d.pSourceLocations[index]=*pPos;
        status = IUS_E_OK;
    }

    if( pList->locationType == IUS_2D_SOURCE_LOCATION ) {
        ((IusParametric2DSourceLocationList *)pList)->base2d.pSourceLocations[index]=*pPos;
        status =  IUS_E_OK;
    }

    return status;
}



int iusHLSourceLocationListSet3DPosition
(
    IusSourceLocationList* pList,
    Ius3DPosition *pPos,
    int index
)
{
    int status = IUS_ERR_VALUE;
    if( pList == NULL ) return IUS_ERR_VALUE;
    if( pPos == NULL ) return IUS_ERR_VALUE;
    if( index >= pList->count ) return IUS_ERR_VALUE;

    if( pList->locationType == IUS_PARAMETRIC_3D_SOURCE_LOCATION ) {
        ((IusParametric3DSourceLocationList *)pList)->base3d.pSourceLocations[index]=*pPos;
        status = IUS_E_OK;
    }

    if( pList->locationType == IUS_3D_SOURCE_LOCATION ) {
        ((IusParametric3DSourceLocationList *)pList)->base3d.pSourceLocations[index]=*pPos;
        status =  IUS_E_OK;
    }

    return status;
}

int iusHLSourceLocationListGetSize
(
    IusSourceLocationList *list
)
{
    return list->count;
}