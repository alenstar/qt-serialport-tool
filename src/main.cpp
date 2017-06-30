#include "mainwindow.h"
#include "helper.h"
#include "loop/loop.h"
#include <memory>
#include <QApplication>
#include <QTextCodec>

Loop* defaultLoop = 0;

//Q_DECLARE_METATYPE(size_t)

int main(int argc, char *argv[])
{
#if (QT_VERSION > QT_VERSION_CHECK(5,0,0))
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF8"));
#endif

    QApplication a(argc, argv);
    // qRegisterMetaType<size_t>();
    Helper::SetStyle("gray");
    MainWindow w;
    w.show();
    std::unique_ptr<Loop> loop(new Loop(0));
    loop->start();
    Loop::default_loop(loop->get_loop());
    return a.exec();
}
