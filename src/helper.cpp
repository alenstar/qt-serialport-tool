#include "helper.h"
Helper *Helper::__instance = 0;
Helper::Helper(QObject *) :
    QObject(qApp)
{
    int fontId = QFontDatabase::addApplicationFont(":/fontawesome-webfont.ttf");
    QString fontName = QFontDatabase::applicationFontFamilies(fontId).at(0);
    iconFont = QFont(fontName);
}

void Helper::SetIcon(QLabel *lab, QChar c, int size)
{
    iconFont.setPointSize(size);
    lab->setFont(iconFont);
    lab->setText(c);
}

void Helper::SetIcon(QPushButton *btn, QChar c, int size)
{
    iconFont.setPointSize(size);
    btn->setFont(iconFont);
    btn->setText(c);
}

QIcon &Helper::GetAppIcon()
{
    return this->appIcon;
}

void Helper::SetAppIcon(QString name)
{
    this->appIcon.addFile(name, QSize(), QIcon::Normal, QIcon::Off);
}

bool Helper::TcpPortIsFreed(int port)
{
#if 0
    QTcpServer tcpServer;

    bool b = tcpServer.listen(QHostAddress::Any, port);
    tcpServer.close();

    return b;
#else
    return true;
#endif
}

QString Helper::FromBase64ForUrl(QString base64String)
{
    //return base64String.replace('.', '=').replace('*', '+').replace('-', '/');
    return base64String.replace('-', '+');
}

/// <summary>
/// 从普通字符串转换为适用于URL的Base64编码字符串
/// </summary>
QString Helper::ToBase64StringForUrl(QString normalString)
{
    //return normalString.replace('+', '*').replace('/', '-').replace('=', '.');
    return normalString.replace('+', '-');
}


Button::Button(QWidget *parent) :
    QPushButton(parent)
{

}


//拷贝文件：
bool copyFileToPath(QString sourceDir, QString toDir, bool coverFileIfExist)
{
    toDir.replace("\\", "/");
    if (sourceDir == toDir) {
        return true;
    }
    if (!QFile::exists(sourceDir)) {
        // clog_debug(CLOG(CLOG_UUID), "not exists %s", sourceDir.toUtf8().data());
        return false;
    }
    QDir *createfile = new QDir;
    bool exist = createfile->exists(toDir);
    if (exist) {
        if (coverFileIfExist) {
            createfile->remove(toDir);
        }
    }//end if

    if (!QFile::copy(sourceDir, toDir)) {
        // clog_debug(CLOG(CLOG_UUID), "copy %s to %s", sourceDir.toUtf8().data(), toDir.toUtf8().data());
        return false;
    }
    return true;
}

//拷贝文件夹：
bool copyDirectoryFiles(const QString &fromDir, const QString &toDir,
                        bool coverFileIfExist)
{
    QDir sourceDir(fromDir);
    QDir targetDir(toDir);
    if (!targetDir.exists()) {   /**< 如果目标目录不存在，则进行创建 */
        if (!targetDir.mkdir(targetDir.absolutePath()))
            return false;
    }

    QFileInfoList fileInfoList = sourceDir.entryInfoList();
    foreach (QFileInfo fileInfo, fileInfoList) {
        if (fileInfo.fileName() == "." || fileInfo.fileName() == "..")
            continue;

        if (fileInfo.isDir()) {   /**< 当为目录时，递归的进行copy */
            if (!copyDirectoryFiles(fileInfo.filePath(),
                                    targetDir.filePath(fileInfo.fileName()),
                                    coverFileIfExist))
                return false;
        } else {         /**< 当允许覆盖操作时，将旧文件进行删除操作 */
            if (coverFileIfExist && targetDir.exists(fileInfo.fileName())) {
                targetDir.remove(fileInfo.fileName());
            }

            /// 进行文件copy
            if (!QFile::copy(fileInfo.filePath(),
                             targetDir.filePath(fileInfo.fileName()))) {
                return false;
            }
        }
    }
    return true;
}

QStringList findFiles(const QString &path)
{
    QStringList files;

    QDir dir = QDir::current();
    // clog_debug(CLOG(CLOG_UUID), "findFiles: %s", path.toUtf8().data());
    if (!path.isEmpty())
        dir = QDir(path);

    dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
#if QT_VERSION >= 0x040000
    QFileInfoList list = dir.entryInfoList();
    for (int i = 0; i < list.size(); ++i) {
        QFileInfo fileInfo = list.at(i);
        files.append(dir.absoluteFilePath(fileInfo.fileName()));
        // clog_debug(CLOG(CLOG_UUID), "findFiles FileName: %s", fileInfo.fileName().toUtf8().data());
    }
#else
    const QFileInfoList *list = dir.entryInfoList();
    if (list) {
        QFileInfoListIterator it(*list);
        QFileInfo *fi;
        while ((fi = it.current()) != 0) {
            ++it;
            files.append(dir.absFilePath(fi->fileName()));
        }
    }
#endif

    return files;
}


Loader::Loader(QString img, QWidget *parent) : QLabel(parent)
{
    this->timer = new QTimer();
    this->img = QPixmap(img);
    this->resize(this->img.size());
    this->setPixmap(this->img);
    QObject::connect(this->timer, SIGNAL(timeout()), this, SLOT(onTimeout()));
}

Loader::Loader(QPixmap &img, QWidget *parent) : QLabel(parent)
{
    this->timer = new QTimer();
    this->img = QPixmap(img);
    this->resize(this->img.size());
    this->setPixmap(this->img);
    QObject::connect(this->timer, SIGNAL(timeout()), this, SLOT(onTimeout()));
}

#include <QPainter>
#include <QTimerEvent>

CycloProgress::CycloProgress(QString img, QWidget *parent) :
    QLabel(parent), minLoops(3)
{
    m_rotation = 0.0;
    timerID = 0;
    m_pixmap = QPixmap(img);
    this->resize(this->m_pixmap.size());

    //this->timer = new QTimer();
    //QObject::connect(this->timer, SIGNAL(timeout()), this, SLOT(onTimeout()));
}

CycloProgress::CycloProgress(QString img, QString bg, QWidget *parent) :
    QLabel(parent), minLoops(3)
{
    m_rotation = 0.0;
    timerID = 0;
    m_pixmap = QPixmap(img);
    m_bg = QPixmap(bg);
    this->resize(this->m_pixmap.size());

    //this->timer = new QTimer();
    //QObject::connect(this->timer, SIGNAL(timeout()), this, SLOT(onTimeout()));
}

CycloProgress::CycloProgress(QPixmap &img, QWidget *parent) :
    QLabel(parent), minLoops(3)
{
    m_rotation = 0.0;
    timerID = 0;
    m_pixmap = QPixmap(img);
    this->resize(this->m_pixmap.size());

    //this->timer = new QTimer();
    //QObject::connect(this->timer, SIGNAL(timeout()), this, SLOT(onTimeout()));
}

CycloProgress::CycloProgress(QPixmap &img, QPixmap &bg, QWidget *parent) :
    QLabel(parent), minLoops(3)
{
    m_rotation = 0.0;
    timerID = 0;
    m_pixmap = QPixmap(img);
    m_bg = QPixmap(bg);
    this->resize(this->m_pixmap.size());

    //this->timer = new QTimer();
    //QObject::connect(this->timer, SIGNAL(timeout()), this, SLOT(onTimeout()));
}

void CycloProgress::setMinLoops(int loops)
{
    minLoops = loops;
}

void CycloProgress::startAnimation(int interval)
{
    if (timerID == 0) {
        timerID = startTimer(interval);
    }

    //this->timer->start(interval);
}

void CycloProgress::stopAnimation()
{
    killTimer(timerID);
    timerID = 0;
    //this->timer->stop();
}

void CycloProgress::/*onTimeout()*/timerEvent(QTimerEvent *event)
{
    if (event->timerId() == timerID) {
        m_rotation += 5;
        //repaint();
        update();
        if (m_rotation > minLoops * 360) {
            emit runMinLoopsEnough();
        }
    }

    //clog_debug(CLOG(CLOG_UUID), "onTimeout");
    /*
    m_rotation += 5;
    update();

    if (m_rotation > minLoops * 360){
        emit runMinLoopsEnough();
    }*/
}

void CycloProgress::paintEvent(QPaintEvent * /*event*/)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.setRenderHint(QPainter::Antialiasing);

    //QPixmap leadon_text("://qrc/leadon_e_char.png");

    QPointF center(m_pixmap.width() / qreal(2), m_pixmap.height() / qreal(2));
    if (! m_bg.isNull())
        painter.drawPixmap((m_pixmap.width() - m_bg.width()) / qreal(2),
                           (m_pixmap.height() - m_bg.height()) / qreal(2), m_bg);

    painter.translate(center);
    painter.rotate(m_rotation);
    painter.translate(-center);

    painter.drawPixmap(QPointF(0, 0), m_pixmap);
}


#include <math.h>
const double x_pi = 3.14159265358979324 * 3000.0 / 180.0;
void bd_encrypt(double gg_lat, double gg_lon, double &bd_lat, double &bd_lon)
{
    double x = gg_lon, y = gg_lat;
    double z = sqrt(x * x + y * y) + 0.00002 * sin(y * x_pi);
    double theta = atan2(y, x) + 0.000003 * cos(x * x_pi);
    bd_lon = z * cos(theta) + 0.0065;
    bd_lat = z * sin(theta) + 0.006;
}

void bd_decrypt(double bd_lat, double bd_lon, double &gg_lat, double &gg_lon)
{
    double x = bd_lon - 0.0065, y = bd_lat - 0.006;
    double z = sqrt(x * x + y * y) - 0.00002 * sin(y * x_pi);
    double theta = atan2(y, x) - 0.000003 * cos(x * x_pi);
    gg_lon = z * cos(theta);
    gg_lat = z * sin(theta);
}

QString readFile(const QString &filename)
{
    QFile f(filename);
    if (!f.open(QFile::ReadOnly | QFile::Text)) {
        return QString();
    } else {
        QTextStream in(&f);
        return in.readAll();
    }
}
