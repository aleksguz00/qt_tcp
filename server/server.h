#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QVector>

class Server : public QTcpServer {
    Q_OBJECT

public:
    Server();
    QTcpSocket* socket;

private:
    QVector<QTcpSocket*> sockets;
    QByteArray data;
    quint16 next_block_size;

    void SendToClient(QString str);

public slots:
    void incomingConnection(qintptr socket_descriptor);
    void slotReadyRead();
};

#endif // SERVER_H
