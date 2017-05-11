#include "mainwindow.h"
#include "jvhelper.h"
#include <QApplication>
#include <QTextCodec>
int main(int argc, char *argv[])
{
#if (QT_VERSION > QT_VERSION_CHECK(5,0,0))
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF8"));
#endif

    QApplication a(argc, argv);
    JVHelper::SetStyle("gray");
    MainWindow w;
    w.show();

    return a.exec();
}
