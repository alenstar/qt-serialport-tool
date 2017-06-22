#include "loop.h"
#include "aeio.h"
#include "logdef.h"
Loop::Loop(QObject* parent, int size): QThread(parent)
{
    loop = aeCreateEventLoop(size);
    if (!loop) {
        LOGE("aeCreateEventLoop failure");
    }
}

Loop::~Loop()
{
    aeDeleteEventLoop(loop);
}

void Loop::run() {
    // TODO
    if (loop) aeMain(loop);
}

aeEventLoop *Loop::get_loop()
{
    return loop;
}

static aeEventLoop *def_loop = 0;
aeEventLoop *Loop::default_loop()
{
    return def_loop;
}

void Loop::default_loop(aeEventLoop *l)
{
    if (def_loop == 0 && l != 0) {
       def_loop = l;
    }
}
