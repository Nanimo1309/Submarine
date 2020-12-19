#ifndef SOCKET_HPP
#define SOCKET_HPP

#include "SocketBase.hpp"

#include <QObject>
#include <QHostAddress>
#include <QByteArray>

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

    void setting(float leftMotor, float rightMotor, float immersion, float cameraXAxis, float cameraYAxis);

public slots:
    void cameraData(QByteArray data);

private:
    QHostAddress m_address;
    quint16 m_port;
    QUdpSocket* m_udp;
};

#endif // SOCKET_HPP