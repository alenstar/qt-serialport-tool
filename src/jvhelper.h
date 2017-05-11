#ifndef JVHELPER_H
#define JVHELPER_H

//#if (QT_VERSION > QT_VERSION_CHECK(5,0,0))
//#if (QT_VERSION <= QT_VERSION_CHECK(5,0,0))
 #include <QApplication>
//#ifdef USE_QT4
#if (QT_VERSION > QT_VERSION_CHECK(5,0,0))
//#include <QtGui>
#include <QtWidgets>
#else
//#include <QtWidgets>
#include <QtGui>
#endif

#include <QMutex>
#include <QCoreApplication>
#include <QTextCodec>
#include <QFont>
#include <QObject>
#include <QFile>
#include <QTranslator>
#include <QTime>
#include <QDesktopWidget>
#include <QLabel>
#include <QIcon>
#include <QPushButton>
#include <QStringList>
#include "jvmessagebox.h"

bool copyDirectoryFiles(const QString &fromDir, const QString &toDir, bool coverFileIfExist);
bool copyFileToPath(QString sourceDir, QString toDir, bool coverFileIfExist);

#if 0
class NoFocusRectangleStyle: public QCommonStyle
{
public:
    void NoFocusRectangleStyle::drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
    {
        if (QStyle::PE_FrameFocusRect == element )
            return;
        else
            QCommonStyle::drawPrimitive(element, option, painter, widget);
    }
};
#endif
class JButton : public QPushButton
{
    Q_OBJECT
public:
	explicit JButton(QWidget *parent = 0);
};

class JVHelper : public QObject
{
private:
    explicit JVHelper(QObject *parent = 0);
    QFont iconFont;
    static JVHelper* __instance;
    //static QString AESKEY("FUCK19JIAG90WEI9");
    QIcon appIcon;
public:
    QString devToken;
#if 1
    static JVHelper* GetInstance()
    {
        static QMutex mutex;
        if (!__instance) {
            QMutexLocker locker(&mutex);
            if (!__instance) {
                __instance = new JVHelper;
            }
        }
        return __instance;
    }
    class Garbo//删除Singleton实例的对象
    {
    public:
        ~Garbo()
        {
            if(JVHelper::__instance){
                delete JVHelper::__instance;
                JVHelper::__instance = NULL;
            }
        }
    };
    static Garbo gb;//在程序结束时，系统会调用它的析构函数
#endif
#ifdef WIN32
    //设置为开机启动
    static void AutoRunWithSystem(bool IsAutoRun, QString AppName, QString AppPath)
    {
        QSettings *reg = new QSettings(
            "HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",
            QSettings::NativeFormat);

        if (IsAutoRun) {
            reg->setValue(AppName, AppPath);
        } else {
            reg->setValue(AppName, "");
        }
    }
#endif
    //设置编码为UTF8
    static void SetUTF8Code()
    {
#if (QT_VERSION <= QT_VERSION_CHECK(5,0,0))
        QTextCodec *codec = QTextCodec::codecForName("UTF-8");
        QTextCodec::setCodecForLocale(codec);
        QTextCodec::setCodecForCStrings(codec);
        QTextCodec::setCodecForTr(codec);
#endif
    }

    static void SetSystemCode()
    {
#if (QT_VERSION <= QT_VERSION_CHECK(5,0,0))
        QTextCodec *codec = QTextCodec::codecForName("System");
        QTextCodec::setCodecForLocale(codec);
        QTextCodec::setCodecForCStrings(codec);
        QTextCodec::setCodecForTr(codec);
#endif
    }

    //设置皮肤样式
    static void SetStyle(const QString &styleName)
    {
        QFile file(QString(":/theme/%1.css").arg(styleName));
        //QFile file(styleName);
        file.open(QFile::ReadOnly);
        QString qss = QLatin1String(file.readAll());
        qApp->setStyleSheet(qss);
        qApp->setPalette(QPalette(QColor("#F0F0F0")));
    }

    //加载中文字符
    static void SetChinese()
    {
        QTranslator *translator = new QTranslator(qApp);
        translator->load(":/theme/qt_zh_CN.qm");
        qApp->installTranslator(translator);
    }

    static void SetChinese(QString& lang)
    {
        QTranslator *translator = new QTranslator(qApp);
        translator->load(lang);
        qApp->installTranslator(translator);
    }

    static void SetLanguage(QString& lang)
    {
        QTranslator *translator = new QTranslator(qApp);
        translator->load(lang);
        qApp->installTranslator(translator);
    }

    //判断是否是IP地址
    static bool IsIP(QString IP)
    {
        QRegExp RegExp("((2[0-4]\\d|25[0-5]|[01]?\\d\\d?)\\.){3}(2[0-4]\\d|25[0-5]|[01]?\\d\\d?)");
        return RegExp.exactMatch(IP);
    }

    //显示信息框,仅确定按钮
    static void ShowMessageBoxInfo(QString info, bool center = true)
    {
        JVMessageBox *msg = new JVMessageBox(center);
        msg->SetMessage(info, 0);
        msg->exec();
    }

    //显示错误框,仅确定按钮
    static void ShowMessageBoxError(QString info, bool center = true)
    {
        JVMessageBox *msg = new JVMessageBox(center);
        msg->SetMessage(info, 2);
        msg->exec();
    }

    //显示询问框,确定和取消按钮
    static int ShowMessageBoxQuesion(QString info, bool center = true)
    {
        JVMessageBox *msg = new JVMessageBox(center);
        msg->SetMessage(info, 1);
        return msg->exec();
    }

    static int ShowMessageBoxAbout(QString info, bool center = true)
    {
        JVMessageBox *msg = new JVMessageBox(center);
        msg->SetMessage(info, 0);
        return msg->exec();
    }

    //延时
    static void Sleep(int sec)
    {
        QTime dieTime = QTime::currentTime().addMSecs(sec);
        while ( QTime::currentTime() < dieTime ) {
            QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
        }
    }

    //窗体居中显示
    static void FormInCenter(QWidget *frm)
    {
        int frmX = frm->width();
        int frmY = frm->height();
        QDesktopWidget w;
        int deskWidth = w.width();
        int deskHeight = w.height();
        QPoint movePoint(deskWidth / 2 - frmX / 2, deskHeight / 2 - frmY / 2);
        frm->move(movePoint);
    }

    static void FormInMouse(QWidget *frm)
    {
        int frmX = frm->width();
        int frmY = frm->height();
        QPoint p = QCursor::pos();
        //QDesktopWidget w;
        //int deskWidth = w.width();
        //int deskHeight = w.height();
        QPoint movePoint(p.x() - frmX * 0.5, p.y() - frmY * 0.5);
        frm->move(movePoint);
    }

    void SetIcon(QLabel* lab, QChar c, int size);

    void SetIcon(QPushButton* btn, QChar c, int size);

    QIcon& GetAppIcon();
    void SetAppIcon(QString name);

    bool TcpPortIsFreed(int port);

    static QByteArray AES128ECBDecryption(QByteArray& key, QByteArray& data);
    static QByteArray AES128ECBEncryption(QByteArray& key, QByteArray& data);

    static QByteArray AES128CBCDecryption(QByteArray& key, QByteArray& iv, QByteArray& data);
    static QByteArray AES128CBCEncryption(QByteArray& key, QByteArray& iv, QByteArray& data);

    static QString FromBase64ForUrl(QString base64String);
    static QString ToBase64StringForUrl(QString normalString);
};

inline QString GBK2UTF8(const QString &inStr)
//inline QByteArray GBK2UTF8(const QString &inStr)
{
    QTextCodec *gbk = QTextCodec::codecForName("GB18030");
    QTextCodec *utf8 = QTextCodec::codecForName("UTF-8");

    //QByteArray g2u = utf8->fromUnicode(gbk->toUnicode(inStr.toLocal8Bit()));			  // gbk  convert utf8
    return utf8->toUnicode(gbk->toUnicode(inStr.toLocal8Bit()).toUtf8());			  // gbk  convert utf8
    //return utf8->toUnicode(gbk->fromUnicode(inStr));			  // gbk  convert utf8
    //return utf8->toUnicode(g2u);
    //return g2u;
}

#if 0
inline QString UTF82GBK(const QString &inStr)
{
    QTextCodec *gbk = QTextCodec::codecForName("GB18030");
    QTextCodec *utf8 = QTextCodec::codecForName("UTF-8");

    QByteArray utf2gbk = gbk->fromUnicode(utf8->toUnicode(inStr.toLocal8Bit()));
    return gbk->toUnicode(utf2gbk);
}
#else
inline QByteArray UTF82GBK(const QString &inStr)
{
    QTextCodec *gbk = QTextCodec::codecForName("GB18030");
    QTextCodec *utf8 = QTextCodec::codecForName("UTF-8");
    QByteArray utf2gbk = gbk->fromUnicode(utf8->toUnicode(inStr.toLocal8Bit()));
    return utf2gbk;
}

#endif

#if 0
inline std::string gbk2utf8(const QString &inStr)
{
    return GBK2UTF8(inStr).toStdString();
}

inline QString utf82gbk(const std::string &inStr)
{
    QString str = QString::fromStdString(inStr);

    return UTF82GBK(str);
}
#endif

QStringList findFiles(const QString& path = QString());
QString readFile(const QString &filename);

#include <math.h>
void bd_decrypt(double bd_lat, double bd_lon, double &gg_lat, double &gg_lon);
void bd_encrypt(double gg_lat, double gg_lon, double &bd_lat, double &bd_lon);

class JLoader : public QLabel
{
	Q_OBJECT
public:
	explicit JLoader(QString img, QWidget *parent = 0);

	JLoader(QPixmap& img, QWidget *parent = 0);

	~JLoader(){
		if(this->timer->isActive()){
			this->timer->stop();
		}
		delete this->timer;
	}

public slots:
	void start(int speed = 500){ this->timer->start(speed); }
	void stop(){ this->timer->stop();}

private slots:
	void onTimeout(){
		QMatrix matrix;
		//matrix = matrix.translate(-this->img.width(), -this->img.height());
		matrix = matrix.rotate(30);
		//matrix = matrix.translate(this->img.width(), this->img.height());

		this->img = this->img.transformed(matrix, Qt::SmoothTransformation);
		this->setPixmap(this->img);
	}

private:
	QTimer* timer;
	QPixmap img;
};

class CycloProgress : public QLabel
{
	Q_OBJECT
public:
	explicit CycloProgress(QString img, QWidget *parent = 0);
	CycloProgress(QString img, QString bg, QWidget *parent = 0);
	CycloProgress(QPixmap& img, QWidget *parent = 0);
	CycloProgress(QPixmap& img, QPixmap& bg, QWidget *parent = 0);
	/*~CycloProgress(){
		if (this->timer->isActive()){
			this->timer->stop();
		}
		delete this->timer;
	}*/
	void setMinLoops(int loops = 3);
	/**
	* @brief startAnimation start the Animation
	* @param interval   unit is  milliseconds
	*/
	void startAnimation(int interval = 25);
	void stopAnimation();

	void start(int speed = 25){ this->startAnimation(speed); }
	void stop(){ this->stopAnimation(); }
signals:
	void runMinLoopsEnough();
protected:
	void paintEvent(QPaintEvent *);
	void timerEvent(QTimerEvent *event);

private slots:
	//void onTimeout();

private:
	int timerID;
	int minLoops;
	QPixmap m_pixmap;
	QPixmap m_bg;
	qreal m_rotation;
	QTimer* timer;
};

#endif // JVHELPER_H
