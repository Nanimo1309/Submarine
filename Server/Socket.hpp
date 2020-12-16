#ifndef SOCKET_HPP
#define SOCKET_HPP

#include "SocketBase.hpp"

#include <QObject>
#include <QByteArray>

class QTcpServer;
class QTcpSocket;
class QUdpSocket;

class Socket:
    public QObject,
    public SocketBase
{
    Q_OBJECT

public:
    Socket(QObject* parent = nullptr);

    void listen(quint16 port);

signals:
    void connected();
    void disconnected();

    void motor(float left, float right, float immersion);
    void camera(float xAxis, float yAxis);

public slots:
    void cameraImage(QByteArray data);

private:
    QTcpSrever* m_server;
    QTcpSocket* m_tcp;
    QUdpSocket* m_udp;
};

#endif // SOCKET_HPP