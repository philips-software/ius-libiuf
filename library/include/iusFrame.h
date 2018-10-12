// Created by nlv09165 on 15/08/2018.
#ifndef IUSLIBRARY_IUSHLFRAME_H
#define IUSLIBRARY_IUSHLFRAME_H


// ADT
// TODO: rename iufr_t => iufr_t
// iufr_t => iufr_t
// iuf_t => iufr_t
//
typedef struct IusFrame IusFrame;
typedef IusFrame *iufr_t;
#define  IUF_INVALID (iufr_t) NULL

iufr_t iusFrameCreate
(
    char *patternListLabel,
    int dataIndex,
    float time
);

int iusFrameDelete
(
    iufr_t iusFrame
);

// operations
int iusFrameCompare
(
    iufr_t reference,
    iufr_t actual
);

// Getters
float iusFrameGetTime
(
    iufr_t iusFrame
);

char *iusFrameGetPatternListLabel
(
    iufr_t iusFrame
);

int iusFrameGetDataIndex
(
    iufr_t iusFrame
);
#endif //IUSLIBRARY_IUSHLFRAME_H
