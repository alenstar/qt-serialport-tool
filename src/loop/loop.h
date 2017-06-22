#ifndef LOOP_H
#define LOOP_H
#include <QThread>

struct aeEventLoop;
class Loop: public QThread
{
public:
    explicit Loop(QObject* parent = 0, int size = 64);
    virtual ~Loop();

    virtual void run() override;

    aeEventLoop* get_loop();
private:
     aeEventLoop *loop;

public:
     static aeEventLoop* default_loop();
     static void default_loop(aeEventLoop* l);
};

#endif // LOOP_H
