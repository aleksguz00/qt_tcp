#include <QDataStream>
#include <QTime>
#include "server.h"

Server::Server() {
    if (this->listen(QHostAddress::Any, 2323)) {
        qDebug() << "start";
    }
    else {
        qDebug() << "error";
    }
    next_block_size = 0;
}

void Server::incomingConnection(qintptr socket_descriptor) {
    socket = new QTcpSocket;
    socket->setSocketDescriptor(socket_descriptor); // Положительное число, которое индетифицирует поток ввода вывода

    connect(socket, &QTcpSocket::readyRead, this, &Server::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);

    sockets.push_back(socket); // добавляем клиента в вектор сокетов

    qDebug() << "client connected" << socket_descriptor;
}

void Server::slotReadyRead() {
    socket = (QTcpSocket*)sender();

    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_5_6);

    if (in.status() == QDataStream::Ok) {
        qDebug() << "read...";

        while (true) {
            if (next_block_size == 0) {
                qDebug() << "next_block_size = 0";

                if (socket->bytesAvailable() < 2) {
                    qDebug() << "Data < 2, break";
                    break;
                }
                in >> next_block_size;
                qDebug() << "next_block_size = " << next_block_size;
            }
            if (socket->bytesAvailable() < next_block_size) {
                qDebug() << "Data not full, break";
                break;
            }

            QString str;
            QTime time;
            in >> time >> str;
            next_block_size = 0;
            qDebug() << str;
            SendToClient(str);
            break;
        }
    }
    else {
        qDebug() << "DataStream error";
    }
}

void Server::SendToClient(QString str) {
    data.clear();

    QDataStream out(&data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_6);

    out << quint16(0) << QTime::currentTime() << str;
    out.device()->seek(0);
    out << quint16(data.size() - sizeof(quint16));
    out << str;

    for (size_t i = 0; i < sockets.size(); ++i) {
        sockets[i]->write(data);
    }
}
