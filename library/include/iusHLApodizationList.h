// Created by nlv09165 on 24/04/2018.
#ifndef IUSLIBRARY_IUSHLAPODIZATIONLIST_H
#define IUSLIBRARY_IUSHLAPODIZATIONLIST_H

// ADT
typedef struct IusApodizationList IusApodizationList;
typedef IusApodizationList *iual_t;
#define  IUAL_INVALID (iual_t) NULL

iual_t iusHLApodizationListCreate
(
    int numApodizations,
    int numElements
);

int iusHLApodizationListDelete
(
    iual_t iusApodizationList
);


// operations
int iusHLApodizationListCompare
(
    iual_t reference,
    iual_t actual
);

// Getters
int iusHLApodizationListGetNumApodizations
(
    iual_t iusApodizationList
);

int iusHLApodizationListGetNumElements
(
    iual_t iusApodizationList
);

float iusHLApodizationListGetApodization
(
    iual_t iusApodizationList,
    int listIndex,
    int elementIndex
);

// Setters
int iusHLApodizationListSetApodization
(
    iual_t iusApodizationList,
    int listIndex,
    int elementIndex,
    float apodization
);



#endif //IUSLIBRARY_IUSHLAPODIZATIONLIST_H
