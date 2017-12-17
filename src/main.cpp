#include "mainwindow.h"
#include "helper.h"
#include <memory>
#include <QApplication>
#include <QTextCodec>
#include "easylogging++.h"

INITIALIZE_EASYLOGGINGPP

//Q_DECLARE_METATYPE(size_t)

int main(int argc, char *argv[])
{
        START_EASYLOGGINGPP(argc, argv);
   el::Loggers::reconfigureAllLoggers(el::ConfigurationType::Format,
                                       "%datetime %func[%fbase] %level: %msg");
#if (QT_VERSION > QT_VERSION_CHECK(5,0,0))
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF8"));
#endif

    QApplication a(argc, argv);
    // qRegisterMetaType<size_t>();
    Helper::SetStyle("gray");
    MainWindow w;
    w.show();
    return a.exec();
}
