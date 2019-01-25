
#ifndef MX_HANDLELIST_H
#define MX_HANDLELIST_H

#include <ius.h>
#include "mex.h"

/* ADT for mx handle */
typedef void * handle_t;
#define MXHANDLE_INVALID (handle_t) NULL

/* ADT for mx handle struct*/
typedef struct MxHandle MxHandle;
typedef MxHandle * mxh_t;
#define MXH_INVALID (mxh_t) NULL

double mxHandleListAppend
(
    mxh_t * list,
    handle_t handle
);

handle_t mxHandleListGetById
(
    mxh_t * list,
    double handleId
);

handle_t mxHandleListRemoveById
(
    mxh_t * list,
    double handleId
);

handle_t mxHandleListRemoveFirst
(
    mxh_t * list
);

#endif // MX_HANDLELIST_H