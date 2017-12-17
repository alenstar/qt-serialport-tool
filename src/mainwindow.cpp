#include "mainwindow.h"
#include "logdef.h"
#include "qextserialport/qextserialenumerator.h"
#include "qextserialport/qextserialport.h"
#include "setupdialog.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QMessageBox>
#include <QtCore>

#if defined(Q_OS_WIN)
#define WORD_WRAP "\r\n"
#else
#define WORD_WRAP "\n"
#endif

MainWindow::MainWindow(QWidget *parent)
    : MDialog(parent), ui(new Ui::MainWindow), _tcp_server(nullptr) {
  ui->setupUi(this);
  this->setup(this, this->ui->topLayout);
  this->setOnClose(std::bind(&MainWindow::onClosed, this));
  QString title = "serial port debug tools";
  this->setTitle(title);
#if 0
    //! [0]
    foreach (QextPortInfo info, QextSerialEnumerator::getPorts())
        ui->portBox->addItem(info.portName);
    //make sure user can input their own port name!
    ui->portBox->setEditable(true);

    ui->baudRateBox->addItem("1200", BAUD1200);
    ui->baudRateBox->addItem("2400", BAUD2400);
    ui->baudRateBox->addItem("4800", BAUD4800);
    ui->baudRateBox->addItem("9600", BAUD9600);
    ui->baudRateBox->addItem("19200", BAUD19200);
    ui->baudRateBox->addItem("38400", BAUD38400);
    ui->baudRateBox->addItem("57600", BAUD57600);
    ui->baudRateBox->addItem("115200", BAUD115200);
    ui->baudRateBox->setCurrentIndex(7);

    ui->parityBox->addItem("NONE", PAR_NONE);
    ui->parityBox->addItem("ODD", PAR_ODD);
    ui->parityBox->addItem("EVEN", PAR_EVEN);
    ui->parityBox->addItem("SPACE", PAR_SPACE);

    ui->flowControlBox->addItem("OFF", FLOW_OFF);
    ui->flowControlBox->addItem("HARDWARE", FLOW_HARDWARE);
    ui->flowControlBox->addItem("XONXOFF", FLOW_XONXOFF);

    ui->dataBitsBox->addItem("5", DATA_5);
    ui->dataBitsBox->addItem("6", DATA_6);
    ui->dataBitsBox->addItem("7", DATA_7);
    ui->dataBitsBox->addItem("8", DATA_8);
    ui->dataBitsBox->setCurrentIndex(3);

    ui->stopBitsBox->addItem("1", STOP_1);
    ui->stopBitsBox->addItem("2", STOP_2);

    ui->queryModeBox->addItem("Polling", QextSerialPort::Polling);
    ui->queryModeBox->addItem("EventDriven", QextSerialPort::EventDriven);
    //! [0]
#endif
  ui->led->turnOff();
  //msg_model_ = new QStringListModel(msg_list_, this);
  msg_model_ = new SessionListModel(this);
  ui->listBox->setModel(msg_model_);
  _tcp_server = new TcpServer(this);
      connect(_tcp_server, &TcpServer::on_message,
            this, &MainWindow::onmessage);
    tcp_client_ = new TcpClient(this);
    connect(tcp_client_, &TcpClient::on_message, this, &MainWindow::onmessage);

  timer = new QTimer(this);
  timer->setInterval(40);

  infoLabel = new QLabel(this);
  txLabel = new QLabel(this);
  rxLabel = new QLabel(this);

  // ui->statusBar->addPermanentWidget(infoLabel);
  // ui->statusBar->addPermanentWidget(txLabel);
  // ui->statusBar->addPermanentWidget(rxLabel);

  //! [1]
  PortSettings settings = {BAUD115200, DATA_8, PAR_NONE, STOP_1, FLOW_OFF, 10};
  port = new QextSerialPort(settings, QextSerialPort::Polling);
  // port = new QextSerialPort(ui->portBox->currentText(), settings,
  // QextSerialPort::Polling);
  //! [1]
  port = new QextSerialPort(settings, QextSerialPort::Polling);

  enumerator = new QextSerialEnumerator(this);
  enumerator->setUpNotifications();
#if 0
    connect(ui->baudRateBox, SIGNAL(currentIndexChanged(int)),
            SLOT(onBaudRateBox_currentIndexChanged(int)));
    connect(ui->parityBox, SIGNAL(currentIndexChanged(int)),
            SLOT(onParityBox_currentIndexChanged(int)));
    connect(ui->flowControlBox, SIGNAL(currentIndexChanged(int)),
            SLOT(onFlowControlBox_currentIndexChanged(int)));
    connect(ui->dataBitsBox, SIGNAL(currentIndexChanged(int)),
            SLOT(onDataBitsBox_currentIndexChanged(int)));
    connect(ui->stopBitsBox, SIGNAL(currentIndexChanged(int)),
            SLOT(onStopBitsBox_currentIndexChanged(int)));
    connect(ui->queryModeBox, SIGNAL(currentIndexChanged(int)),
            SLOT(onQueryModeBox_currentIndexChanged(int)));
    connect(ui->timeoutBox, SIGNAL(valueChanged(int)),
            SLOT(onTimeoutBox_valueChanged(int)));
    connect(ui->portBox, SIGNAL(editTextChanged(QString)),
            SLOT(onPortBox_editTextChanged(QString)));
#endif
  connect(ui->openCloseButton, SIGNAL(clicked()),
          SLOT(onOpenCloseButton_clicked()));
  connect(ui->sendButton, SIGNAL(clicked()), SLOT(onSendButton_clicked()));
  //connect(ui->wordWrapBox, SIGNAL(stateChanged(int)),
  //        SLOT(onWordWrapBox_stateChanged(int)));

  connect(timer, SIGNAL(timeout()), SLOT(onReadyRead()));
  connect(port, SIGNAL(readyRead()), SLOT(onReadyRead()));

  // connect(enumerator, SIGNAL(deviceDiscovered(QextPortInfo)),
  // SLOT(onPortAddedOrRemoved()));
  // connect(enumerator, SIGNAL(deviceRemoved(QextPortInfo)),
  // SLOT(onPortAddedOrRemoved()));

  // connect(ui->action_Setup, SIGNAL(triggered()), SLOT(onSetUp_triggered()));


}

MainWindow::~MainWindow() {
  if (_tcp_server) {
    delete _tcp_server;
    _tcp_server = nullptr;
  }
  delete ui;
  delete port;
}

void MainWindow::onClosed() {
  qDebug() << "close";
  qApp->quit();
}
#if 0
void MainWindow::onPortBox_editTextChanged(const QString &arg1)
{
    qDebug() << "port name is " << arg1;
    if (port->isOpen()) {
        port->close();
        ui->led->turnOff();
    }
}

void MainWindow::onBaudRateBox_currentIndexChanged(int index)
{
    port->setBaudRate((BaudRateType)ui->baudRateBox->itemData(index).toInt());
}

void MainWindow::onDataBitsBox_currentIndexChanged(int index)
{
    port->setDataBits((DataBitsType)ui->dataBitsBox->itemData(index).toInt());

}

void MainWindow::onStopBitsBox_currentIndexChanged(int index)
{
    port->setStopBits((StopBitsType)ui->stopBitsBox->itemData(index).toInt());

}

void MainWindow::onParityBox_currentIndexChanged(int index)
{
    port->setParity((ParityType)ui->parityBox->itemData(index).toInt());

}

void MainWindow::onFlowControlBox_currentIndexChanged(int index)
{
    port->setFlowControl((FlowType)ui->flowControlBox->itemData(index).toInt());

}

void MainWindow::onQueryModeBox_currentIndexChanged(int index)
{
    port->setQueryMode((QextSerialPort::QueryMode)ui->queryModeBox->itemData(
                           index).toInt());

}

void MainWindow::onTimeoutBox_valueChanged(int arg1)
{
    port->setTimeout(arg1);
}
#endif

void MainWindow::onOpenCloseButton_clicked() {
  if (!port->isOpen()) {
    port->setPortName(this->portName);
    port->open(QIODevice::ReadWrite);

    this->infoLabel->setText(this->portName);
    this->txLabel->setText("TX: ");
    this->rxLabel->setText("RX: ");
  } else {
    port->close();
  }

  // If using polling mode, we need a QTimer
  if (port->isOpen() && port->queryMode() == QextSerialPort::Polling)
    timer->start();
  else
    timer->stop();

  ui->led->turnOn(port->isOpen());
}

void MainWindow::onSendButton_clicked() {
    switch (ui->tabWidget->currentIndex()) {
    case 0: { // forward
        break;
    }
    case 1: { // server
        if (_tcp_server->is_running() && !ui->sendBox->toPlainText().isEmpty()) {
            QByteArray ba = ui->sendBox->toPlainText().toLocal8Bit();
           _tcp_server->post_message(ba);
        }
        break;
    }
    case 2: { // client
        if (tcp_client_->is_running() && !ui->sendBox->toPlainText().isEmpty()) {
            QByteArray ba = ui->sendBox->toPlainText().toLocal8Bit();
           tcp_client_->post_message(ba);
        }
        break;
    }
    case 3:{ // serialport
        if (port->isOpen() && !ui->sendBox->toPlainText().isEmpty()) {
    if (ui->hexSendBox->isChecked()) {
      // port->write(stringToHex(g_array,
      // ui->sendEdit->toPlainText().toLatin1()));
      bool ok;
      char data;
      QStringList list;
      g_array.clear();
      list = ui->sendBox->toPlainText().split(" ");
      for (int i = 0; i < list.count(); i++) {
        if (list.at(i) == " ")
          continue;
        if (list.at(i).isEmpty())
          continue;
        data = (char)list.at(i).toInt(&ok, 16);
        if (!ok) {
          QMessageBox::information(this, tr("Warning: "), tr("data is bad "),
                                   QMessageBox::Ok);
        }
        g_array.append(data);
      }
      port->write(g_array);
    } else
      port->write(ui->sendBox->toPlainText().toLatin1());
  }
        break;
    }
    default:
        break;

    }


  if (ui->clearBox->isChecked())
    ui->sendBox->clear();
}

void MainWindow::onWordWrapBox_stateChanged(int state) {
  // if(ui->wordWrapBox->checkState() == Qt::Checked)
  // ui->recvEdit->insertPlainText(WORD_WRAP);
  if (state == Qt::Checked)
    ui->recvBox->insertPlainText(WORD_WRAP);
}

void MainWindow::onReadyRead() {
  if (port->bytesAvailable()) {
    ui->recvBox->moveCursor(QTextCursor::End);
    /*
    if (ui->hexShowBox->isChecked()) {
      g_string.clear();
      // ui->recvEdit->insertPlainText(showHex(g_string, port->readAll()));
      QByteArray array;
      array = port->readAll();
      for (int i = 0; i < array.count(); i++) {
        QString s;
        s.sprintf("%02x ", (unsigned char)array.at(i));
        g_string.append(s);
      }
      ui->recvEdit->insertPlainText(g_string);
    } else
      ui->recvEdit->insertPlainText(QString::fromLatin1(port->readAll()));
    if (ui->wordWrapBox->isChecked())
      ui->recvEdit->insertPlainText(WORD_WRAP);
      */
  }
}

#if 0
void MainWindow::onPortAddedOrRemoved()
{
    QString current = ui->portBox->currentText();

    ui->portBox->blockSignals(true);
    ui->portBox->clear();
    foreach (QextPortInfo info, QextSerialEnumerator::getPorts())
        ui->portBox->addItem(info.portName);

    ui->portBox->setCurrentIndex(ui->portBox->findText(current));

    ui->portBox->blockSignals(false);
}
#endif
void MainWindow::onSetUp_triggered() {
  setupDialog setup(this);
  setup.setWindowTitle(tr("serial port setup"));

  connect(enumerator, SIGNAL(deviceDiscovered(QextPortInfo)), &setup,
          SLOT(onPortAddedOrRemoved()));
  connect(enumerator, SIGNAL(deviceRemoved(QextPortInfo)), &setup,
          SLOT(onPortAddedOrRemoved()));

  if (setup.exec()) {
    // qDebug()<<setup.getPortName();
    this->portName = setup.getPortName();
    this->port->setBaudRate((BaudRateType)setup.getBaudRate());
    this->port->setDataBits((DataBitsType)setup.getDataBits());
    this->port->setParity((ParityType)setup.getParity());
    this->port->setStopBits((StopBitsType)setup.getStopBits());
    this->port->setFlowControl((FlowType)setup.getFlowControl());
    this->port->setQueryMode((QextSerialPort::QueryMode)setup.getQueryMode());
  }
  disconnect(enumerator, SIGNAL(deviceDiscovered(QextPortInfo)), &setup,
             SLOT(onPortAddedOrRemoved()));
  disconnect(enumerator, SIGNAL(deviceRemoved(QextPortInfo)), &setup,
             SLOT(onPortAddedOrRemoved()));
}

#if 0
const QByteArray &&MainWindow::stringToHex(const QString &string)
{
    QByteArray array;
    char str_h, str_l, data;
    int i, len = string.length();

    for (i = 0; i < len;) {
        if ((i + 1) >= len) return std::move(array);
        //str_h = string[i].toAscii();
        str_h = string[i].toAscii();
        if (str_h == ' ') {
            i++;
            continue;
        }
        i++;
        str_l = string[i].toAscii();
        if (str_l == ' ') return std::move(array);

        data = charToHex(str_h) * 16 + charToHex(str_l);
        array.append(data);
        qDebug() << "send: " << ((int)data & 0x00ff);
    }

    return std::move(array);
}

const QString &&MainWindow::showHex(const QByteArray &array)
{
    int i, len = array.length();
    char c;
    QString str;

    for (i = 0; i < len; i++) {
        c = array[i] / 16;
        if (c >= 0 && c <= 9)
            str.append(c + '0');
        else {
            if (c > 9)
                str.append(c + 'a');
        }
        qDebug() << "recv: " << c;

        c = array[i] - c;
        c = (c > 0) ? c : (-c);
        if (c >= 0 && c <= 9)
            str.append(c + '0');
        else {
            if (c > 9)
                str.append(c + 'a');
        }
        qDebug() << """recv: " << c;

        if ((i + 1) % 16) str.append(' ');
        else str.append('\n');
    }
    return std::move(string);
}

char MainWindow::charToHex(char c)
{
    if (c >= '0' && c <= '9')
        return c - '0';
    else {
        if (c >= 'a' && c <= 'f')
            return c - 'a' + 10;
        else {
            if (c >= 'A' && c <= 'F')
                return c - 'A' + 10;
            else return -1;
        }
    }

    return -1;
}
#endif

void MainWindow::onmessage(QString& session, QByteArray& data)
{
//QString str = QString("%1 %2-->%3 %4").arg(len, 3).arg(peer).arg(sock).arg(data);
QString str = QString("%1 [%2] %3 -->").arg(QTime::currentTime().toString()).arg(data.size(), 4).arg(session); //.arg(data);
int row = msg_model_->rowCount();
if (msg_model_->insertRow(row) ){
    QModelIndex idx = msg_model_->index(row);
    bool ok = msg_model_->setData(idx, str, Qt::DisplayRole);
    if (!ok) {
       LOGE("setData DisplayRole failed");
    }
    ok = msg_model_->setData(idx, data, Qt::UserRole); // QStringListModel not support Qt::UserRole
    if (!ok) {
       LOGE("setData UserRole failed");
    }

} else {
    LOGE("insertRow failed");
}
}

void MainWindow::on_btnClient_clicked(bool checked)
{
    if (checked) {
       if (tcp_client_->is_running()) tcp_client_->stop();
       tcp_client_->start(ui->cboxClientHost->currentText(), ui->cboxClientPort->currentText().toInt());
    } else {
       tcp_client_->start(ui->cboxClientHost->currentText(), ui->cboxClientPort->currentText().toInt());
    }
}

void MainWindow::on_btn_tcp_listen_clicked(bool checked)
{
   LOGD("%s", checked ? "true":"false");
   if (checked) {
         if (_tcp_server->is_running()) {
    _tcp_server->stop();}
    ui->cbox_tcp_addr->setDisabled(true);
    ui->cbox_tcp_port->setDisabled(true);
            _tcp_server->start(ui->cbox_tcp_addr->currentText(),
                      ui->cbox_tcp_port->currentText().toInt());
   } else {
         if (_tcp_server->is_running()) {
    _tcp_server->stop();}
      ui->cbox_tcp_addr->setDisabled(false);
      ui->cbox_tcp_port->setDisabled(false);
   }
}

void MainWindow::on_listBox_clicked(const QModelIndex &index)
{
QByteArray var = msg_model_->data(index, Qt::UserRole).value<QByteArray>();
ui->recvBox->setText(QString(var));
}


void MainWindow::on_btnStr2array_clicked()
{
        QString out;
         QByteArray str = ui->sendBox->toPlainText().toUtf8();
    for (auto& var: str) {
        if (out.size() != 0) {
            out.append(", ");
        }
        QString c = QString("0x%1").arg(var, 1, 16);
       out.append(c);
    }
    ui->recvBox->setPlainText(out);
}

void MainWindow::on_btnArray2str_clicked()
{
    QString out;
    QString str = ui->sendBox->toPlainText().trimmed();
    QStringList ss = str.split(',');
    for (auto& var: ss) {
        QString v = var.trimmed();
        char c = 0;
        if(v.startsWith("0x")) {
            bool ok = false;
            c = var.toInt(&ok, 16);
        } else {
            c = var.toInt();
        }
       out.append(c);
    }
    ui->recvBox->setPlainText(out);
}

