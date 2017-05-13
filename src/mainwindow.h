#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLabel>
#include "mdialog.h"
namespace Ui
{
class MainWindow;
}

class QTimer;
class QextSerialPort;
class QextSerialEnumerator;

class MainWindow : public MDialog
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public :
    void onClosed();
private slots:
    //void onPortAddedOrRemoved();
    void onReadyRead();
#if 0
    void onPortBox_editTextChanged(const QString &arg1);

    void onBaudRateBox_currentIndexChanged(int index);

    void onDataBitsBox_currentIndexChanged(int index);

    void onStopBitsBox_currentIndexChanged(int index);

    void onParityBox_currentIndexChanged(int index);

    void onQueryModeBox_currentIndexChanged(int index);

    void onFlowControlBox_currentIndexChanged(int index);

    void onTimeoutBox_valueChanged(int arg1);
#endif

    void onWordWrapBox_stateChanged(int state);

    void onOpenCloseButton_clicked();

    void onSendButton_clicked();

    void onSetUp_triggered();

private:
    Ui::MainWindow *ui;

    QLabel *txLabel;
    QLabel *rxLabel;
    QLabel *infoLabel;

    QTimer *timer;
    QextSerialPort *port;
    QextSerialEnumerator *enumerator;

    QString portName;
    QString g_string;
    QByteArray g_array;
};

#endif // MAINWINDOW_H
