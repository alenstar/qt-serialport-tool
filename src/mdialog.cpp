#include "jvhelper.h"
#include "mdialog.h"
#include <QPalette>
#include <QPixmap>
#include <QBrush>
#include <QRect>
#include <QApplication>
#include <QDesktopWidget>
#include <QAction>
#include <QMenu>

#ifndef I18N
#define i18nConve(x) (x)
#else
#define i18nConve(x) QString::fromStdString(I18NRepo::getInstance()->Get(std::string("MDialog"), std::string(x)))
#endif

MDialog::MDialog()
{
    // this->parent = parent;
    this->mousePressed = false;

    //this->setupTheme();
    //this->retranslateMDialogUi();
    //JVHelper::GetInstance()->FormInCenter(this);

    this->btnMenu_Max = 0;
    this->btnMenu_Mini = 0;
    this->btnMenu_Menu = 0;


#if USE_USER_BG
    this->setWindowFlags(Qt::FramelessWindowHint);//去掉标题栏
    QPalette p = palette();  //  得到窗口部件的调色板
    QPixmap img("/opt/AndroVManager/share/icons/bg.png");  //蓝色图片
    p.setBrush(QPalette::Window, QBrush(img)); //给窗体设置笔刷，用笔刷设置
    this->setPalette(p);

    //QDesktopWidget* pDw = QApplication::desktop();//获得桌面窗体
    //QDialog* dialog = dynamic_cast<QDialog*>(this->parent());
    //this->move(dialog->width()/2 - this->width()/2, dialog->height()/2 - this->height()/2);//move就是是设置位置的是widget的位置！
#endif
}

MDialog::~MDialog()
{
    //this->btnMenu_Max = 0;
    //this->btnMenu_Mini = 0;
    //this->btnMenu_Menu = 0;
}
#if 0
void MDialog::mousePressEvent(QMouseEvent *event)
{
    this->windowPos = this->pos();
    this->mousePos = event->globalPos();
    this->dPos = mousePos - windowPos;

}

void MDialog::mouseMoveEvent(QMouseEvent *event)
{
    this->move(event->globalPos() - this->dPos);
}
#endif

void MDialog::mouseMoveEvent(QMouseEvent *e)
{
    if (mousePressed && (e->buttons() && Qt::LeftButton)) {
    //if (e->buttons() && Qt::LeftButton) {
        parent->move(e->globalPos() - mousePoint);
        e->accept();
    }
}

void MDialog::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        mousePressed = true;
        mousePoint = e->globalPos() - parent->pos();
        e->accept();
    }
    else
        mousePressed = false;

}

void MDialog::mouseReleaseEvent(QMouseEvent *)
{
    mousePressed = false;
}

void MDialog::setupTheme(QLayout *mainLayout1)
{


        //if (this->objectName().isEmpty())
        //    this->setObjectName(QString::fromUtf8("MDialog"));
        //this->resize(303, 143);
        //topLayout = new QVBoxLayout(parent);
        //topLayout->setSpacing(0);
        //topLayout->setContentsMargins(0, 0, 0, 0);
        //topLayout->setObjectName(QString::fromUtf8("topLayout"));
        widget_title = new QWidget(parent);
        widget_title->setObjectName(QString::fromUtf8("widget_title"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(widget_title->sizePolicy().hasHeightForWidth());
        widget_title->setSizePolicy(sizePolicy);
        widget_title->setMinimumSize(QSize(100, 24));
        titleLayout = new QHBoxLayout(widget_title);
        titleLayout->setSpacing(0);
        titleLayout->setContentsMargins(0, 0, 0, 0);
        titleLayout->setObjectName(QString::fromUtf8("titleLayout"));
        lab_Ico = new QLabel(widget_title);
        lab_Ico->setObjectName(QString::fromUtf8("lab_Ico"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(lab_Ico->sizePolicy().hasHeightForWidth());
        lab_Ico->setSizePolicy(sizePolicy1);
        lab_Ico->setMinimumSize(QSize(24, 24));
        lab_Ico->setAlignment(Qt::AlignCenter);

        titleLayout->addWidget(lab_Ico);

        lab_Title = new QLabel(widget_title);
        lab_Title->setObjectName(QString::fromUtf8("lab_Title"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(lab_Title->sizePolicy().hasHeightForWidth());
        lab_Title->setSizePolicy(sizePolicy2);
        //lab_Title->setStyleSheet(QString::fromUtf8("font: 10pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
        lab_Title->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        titleLayout->addWidget(lab_Title);

        widget_menu = new QWidget(widget_title);
        widget_menu->setObjectName(QString::fromUtf8("widget_menu"));
        sizePolicy1.setHeightForWidth(widget_menu->sizePolicy().hasHeightForWidth());
        widget_menu->setSizePolicy(sizePolicy1);
        menuLayout = new QHBoxLayout(parent);
        menuLayout->setSpacing(0);
        menuLayout->setContentsMargins(0, 0, 0, 0);
        menuLayout->setObjectName(QString::fromUtf8("menuLayout"));

        btnMenu_Close = new QPushButton(widget_menu);
        btnMenu_Close->setObjectName(QString::fromUtf8("btnMenu_Close"));
        QSizePolicy sizePolicy3(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(btnMenu_Close->sizePolicy().hasHeightForWidth());
        btnMenu_Close->setSizePolicy(sizePolicy3);
        btnMenu_Close->setMinimumSize(QSize(32, 24));
        btnMenu_Close->setCursor(QCursor(Qt::ArrowCursor));
        btnMenu_Close->setFocusPolicy(Qt::NoFocus);
        btnMenu_Close->setFlat(true);

        menuLayout->addWidget(btnMenu_Close);

        titleLayout->addWidget(widget_menu);

        topLayout->addWidget(widget_title);

        //widget_main = new QWidget(this);
        //widget_main->setObjectName(QString::fromUtf8("widget_main"));
        //widget_main->setStyleSheet(QString::fromUtf8("font: 11pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
        //mainLayout = new QVBoxLayout(parent);
        //mainLayout->setSpacing(5);
        //mainLayout->setContentsMargins(5, 0, 5, 5);
        //mainLayout->setObjectName(QString::fromUtf8("mainLayout"));
        parent->setLayout(mainLayout);
        //topLayout->addWidget(widget_main);

        QMetaObject::connectSlotsByName(parent);

        //设置窗体标题栏隐藏
        parent->setWindowFlags(Qt::FramelessWindowHint);
        //设置窗体关闭时自动释放内存
        parent->setAttribute(Qt::WA_DeleteOnClose);


        this->nowLocation = parent->geometry();
        this->isMax = false;

        this->lab_Title->installEventFilter(parent);

        //JVHelper::FormInCenter(this);

        //this->setWindowFlags(Qt::FramelessWindowHint);//去掉标题栏
}

void MDialog::showMaxBtn()
{
    if(this->btnMenu_Max == 0){
        btnMenu_Max= new QPushButton(widget_menu);
        btnMenu_Max->setObjectName(QString::fromUtf8("btnMenu_Max"));
        QSizePolicy sizePolicy3(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(btnMenu_Close->sizePolicy().hasHeightForWidth());
        btnMenu_Max->setSizePolicy(sizePolicy3);
        btnMenu_Max->setMinimumSize(QSize(32, 24));
        btnMenu_Max->setCursor(QCursor(Qt::ArrowCursor));
        btnMenu_Max->setFocusPolicy(Qt::NoFocus);
        btnMenu_Max->setFlat(true);

        menuLayout->insertWidget(0, btnMenu_Max);

#ifndef QT_NO_TOOLTIP
        btnMenu_Max->setToolTip(i18nConve("Maximize"));
#endif // QT_NO_TOOLTIP
        JVHelper::GetInstance()->SetIcon(this->btnMenu_Max, QChar(0xf096), 10);

        // connect(this->btnMenu_Max, SIGNAL(clicked()), this, SLOT(btnMenuMaxClicked()));
    }
}

void MDialog::showMiniBtn()
{
    if(this->btnMenu_Mini == 0){
        btnMenu_Mini = new QPushButton(widget_menu);
        btnMenu_Mini->setObjectName(QString::fromUtf8("btnMenu_Mini"));
        QSizePolicy sizePolicy3(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(btnMenu_Close->sizePolicy().hasHeightForWidth());
        btnMenu_Mini->setSizePolicy(sizePolicy3);
        btnMenu_Mini->setMinimumSize(QSize(32, 24));
        btnMenu_Mini->setCursor(QCursor(Qt::ArrowCursor));
        btnMenu_Mini->setFocusPolicy(Qt::NoFocus);
        btnMenu_Mini->setFlat(true);

        menuLayout->insertWidget(0, btnMenu_Mini);

#ifndef QT_NO_TOOLTIP
        btnMenu_Mini->setToolTip(i18nConve("Minimize"));
#endif // QT_NO_TOOLTIP
        JVHelper::GetInstance()->SetIcon(this->btnMenu_Mini, QChar(0xf068), 10);
        // connect(this->btnMenu_Mini, SIGNAL(clicked()), this, SLOT(showMinimized()));
    }
}

void MDialog::showMenuBtn()
{
    if(this->btnMenu_Menu == 0){
        btnMenu_Menu = new QPushButton(widget_menu);
        btnMenu_Menu->setObjectName(QString::fromUtf8("btnMenu_Menu"));
        QSizePolicy sizePolicy3(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(btnMenu_Close->sizePolicy().hasHeightForWidth());
        btnMenu_Menu->setSizePolicy(sizePolicy3);
        btnMenu_Menu->setMinimumSize(QSize(32, 24));
        btnMenu_Menu->setCursor(QCursor(Qt::ArrowCursor));
        btnMenu_Menu->setFocusPolicy(Qt::NoFocus);
        btnMenu_Menu->setFlat(true);

        menuLayout->insertWidget(0, btnMenu_Menu);

#ifndef QT_NO_TOOLTIP
        btnMenu_Menu->setToolTip(i18nConve("Menu"));
#endif // QT_NO_TOOLTIP
        JVHelper::GetInstance()->SetIcon(this->btnMenu_Menu, QChar(0xf0c9), 10);
    }
}

void MDialog::setMainWindow(QWidget *w)
{
    parent = w;
    // mainLayout->addWidget(f);
}

void MDialog::retranslateMDialogUi()
{
    //this->setWindowTitle(i18nConve("title"));
    //lab_Ico->setText(QString());
    //lab_Title->setText(QString());
#ifndef QT_NO_TOOLTIP
    btnMenu_Close->setToolTip(i18nConve("Close"));
#endif // QT_NO_TOOLTIP
    //btnMenu_Close->setText(QString());

    JVHelper::GetInstance()->SetIcon(this->lab_Ico, QChar(0xf015), 12);
    JVHelper::GetInstance()->SetIcon(this->btnMenu_Close, QChar(0xf00d), 10);

} // retranslateUi


void MDialog::SetTitleAndIcon(QString& title, QIcon& icon)
{
    parent->setWindowIcon(icon);
    parent->setWindowTitle(title);
    this->lab_Title->setText(title);
}

void MDialog::setupTray()
{
#if 0
    QIcon icon = QIcon(GET_RES_FILE(app.png));
    //setWindowIcon(icon);
    QSystemTrayIcon* trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(icon);
    trayIcon->setToolTip(this->windowTitle());
    createTrayActions(trayIcon);
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(trayIconActivated(QSystemTrayIcon::ActivationReason)));
    trayIcon->show();
#endif
}

void MDialog::createTrayActions(QSystemTrayIcon* tray)
{
     QAction* minimizeAction = new QAction((i18nConve("&Miniu")), parent);
     // connect(minimizeAction, SIGNAL(triggered()), this, SLOT(hide()));
     QAction* restoreAction = new QAction(i18nConve("&Restore"), parent);
     //connect(restoreAction, SIGNAL(triggered()), this, SLOT(showNormal()));
     //QAction* quitAction = new QAction(i18nConve("&Exit"), this);
     //connect(quitAction, SIGNAL(triggered()), this, SLOT(closeWindow()));

     QMenu* trayIconMenu = new QMenu(parent);
     trayIconMenu->addAction(minimizeAction);
     trayIconMenu->addAction(restoreAction);
     //trayIconMenu->addSeparator();
     //trayIconMenu->addAction(quitAction);
     tray->setContextMenu(trayIconMenu);

     //connect(trayIconMenu, SIGNAL(aboutToShow()), this, SLOT(showNormal()));
}

void MDialog::trayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason)
    {
    case QSystemTrayIcon::Trigger:
        if(parent->isVisible())
            parent->hide();
         else
            parent->showNormal();
        break;
    case QSystemTrayIcon::DoubleClick:
        break;
    case QSystemTrayIcon::MiddleClick:
        break;
    default:
        break;
    }
}

void MDialog::btnMenuMaxClicked()
{
    if (this->isMax) {
        parent->setGeometry(this->nowLocation);
        JVHelper::GetInstance()->SetIcon(this->btnMenu_Max, QChar(0xf096), 10);
        btnMenu_Max->setToolTip(i18nConve("Maximize"));
    } else {
        this->nowLocation = parent->geometry();
        parent->setGeometry(qApp->desktop()->availableGeometry());
        JVHelper::GetInstance()->SetIcon(this->btnMenu_Max, QChar(0xf079), 10);
        btnMenu_Max->setToolTip(i18nConve("Restore"));
    }
    this->isMax = !this->isMax;
}


#undef I18N
