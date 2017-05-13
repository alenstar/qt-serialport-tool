#ifndef MDIALOG_H
#define MDIALOG_H
#include <functional>
#include <QDialog>
#include <QMouseEvent>
#include <QPoint>
#include <QtCore/QVariant>
//#ifndef USE_QT4
#if (QT_VERSION > QT_VERSION_CHECK(5,0,0))
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets//QVBoxLayout>
#include <QtWidgets/QWidget>
#else
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#endif
#include <QSystemTrayIcon>

class MDialog: public QDialog
{
    // Q_OBJECT
public:
    explicit MDialog(QWidget *parent);
    //virtual ~MDialog();
    void setup(QWidget *w, QVBoxLayout *mainLayout);
    void showMaxBtn();
    void showMiniBtn();
    void showMenuBtn();
    void setupTray();
    virtual void createTrayActions(QSystemTrayIcon *tray);
    void setTitleAndIcon(QString &title, QIcon &icon);
    void setTitle(QString &title);
    void setIcon(QIcon &icon);
signals:

public slots:

    void trayIconActivated(QSystemTrayIcon::ActivationReason reason);
    void btnMenuMaxClicked();
public:
    void setOnClose(std::function<void()> cb);
    //virtual void onMinimize();
    //virtual void onMaximize();
    //virtual void closeWindow(){ this->close(); };
protected:
#if 0
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
#endif
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *);

private:
    //QPoint windowPos;
    //QPoint mousePos;
    //QPoint dPos;
    QPoint mousePoint;              //鼠标拖动自定义标题栏时的坐标
    bool mousePressed;              //鼠标是否按下
    std::function<void()> oncloseCb;

private slots:
    void onclosed();
protected:
    QWidget *widget_title;
    QHBoxLayout *titleLayout;
    QLabel *lab_Ico;
    QLabel *lab_Title;
    QWidget *widget_menu;
    QHBoxLayout *menuLayout;
    QPushButton *btnMenu_Close;
    QPushButton *btnMenu_Menu;
    QPushButton *btnMenu_Mini;
    QPushButton *btnMenu_Max;

    bool isMax;
    QRect nowLocation;
    QWidget *parent;
};

#endif // MDIALOG_H
