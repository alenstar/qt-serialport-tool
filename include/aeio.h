#ifndef AEIO_H
#define AEIO_H
#ifdef __cplusplus
extern "C" {
#endif

#include "ae.h"
#include "anet.h"
#include "common.h"
void unlinkFileEvent(aeEventLoop *loop, int fd);

#ifdef __cplusplus
}
#endif
#endif // AEIO_H

