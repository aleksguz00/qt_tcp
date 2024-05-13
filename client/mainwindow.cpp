#include <QHostInfo>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    socket = new QTcpSocket(this);
    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);

    next_block_size = 0;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_connect_clicked()
{
    QHostInfo hostInfo = QHostInfo::fromName(QHostInfo::localHostName());
        if (!hostInfo.addresses().isEmpty()) {
            QHostAddress localAddress = hostInfo.addresses().first();
            socket->connectToHost(localAddress.toString(), 2323);
            qDebug() << localAddress.toString();
        } else {
            qWarning("Failed to get local IP address");
        }
}

void MainWindow::SendToServer(QString str)
{
    data.clear();

    QDataStream out(&data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_6);

    out << quint16(0) << QTime::currentTime() << str;

    out.device()->seek(0);
    out << quint16(data.size() - sizeof(quint16));

    socket->write(data);

    ui->lineEdit->clear();
}

void MainWindow::slotReadyRead()
{
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_5_6);

    if (in.status() == QDataStream::Ok) {
        while (true) {
            if (next_block_size == 0) {
                if (socket->bytesAvailable() < 2) {
                    break;
                }
                in >> next_block_size;
            }
            if (socket->bytesAvailable() < next_block_size) {
                break;
            }

            QString str;
            QTime time;
            in >> str >> time;
            next_block_size = 0;

            ui->textBrowser->append(time.toString() + " " + str);
        }
    }
    else {
        ui->textBrowser->append("read error");
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    SendToServer(ui->lineEdit->text());
}


void MainWindow::on_lineEdit_returnPressed()
{
    SendToServer(ui->lineEdit->text());
}




