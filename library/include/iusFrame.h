// Created by nlv09165 on 15/08/2018.
#ifndef IUSLIBRARY_IUSHLFRAME_H
#define IUSLIBRARY_IUSHLFRAME_H


// ADT
// TODO: rename iuf_t => iufr_t
// iuf_t => iufr_t
// iufi_t => iuf_t
//
typedef struct IusFrame IusFrame;
typedef IusFrame *iuf_t;
#define  IUF_INVALID (iuf_t) NULL

iuf_t iusFrameCreate
(
    int patternListIndex,
    int dataIndex,
    float time
);

int iusFrameDelete
(
    iuf_t iusFrame
);

// operations
int iusFrameCompare
(
    iuf_t reference,
    iuf_t actual
);

// Getters
float iusFrameGetTime
(
    iuf_t iusFrame
);

int iusFrameGetPatternListIndex
(
    iuf_t iusFrame
);

int iusFrameGetDataIndex
(
    iuf_t iusFrame
);
#endif //IUSLIBRARY_IUSHLFRAME_H
