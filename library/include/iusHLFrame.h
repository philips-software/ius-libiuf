// Created by nlv09165 on 15/08/2018.
#ifndef IUSLIBRARY_IUSHLFRAME_H
#define IUSLIBRARY_IUSHLFRAME_H


// ADT
typedef struct IusFrame IusFrame;
typedef IusFrame *iuf_t;
#define  IUF_INVALID (iuf_t) NULL

iuf_t iusHLFrameCreate
(
    int patternListIndex,
    int dataIndex,
    float time
);

int iusHLFrameDelete
(
    iuf_t iusFrame
);

// operations
int iusHLFrameCompare
(
    iuf_t reference,
    iuf_t actual
);

// Getters
float iusHLFrameGetTime
(
    iuf_t iusFrame
);

int iusHLFrameGetPatternListIndex
(
    iuf_t iusFrame
);

int iusHLFrameGetDataIndex
(
    iuf_t iusFrame
);
#endif //IUSLIBRARY_IUSHLFRAME_H
