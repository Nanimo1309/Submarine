#ifndef SOCKET_HPP
#define SOCKET_HPP

#include "SocketBase.hpp"

#include <QObject>
#include <QHostAddress>
#include <QByteArray>
#include <QMap>

class QTcpSocket;
class QUdpSocket;

class Socket:
    public QObject,
    public SocketBase
{
    Q_OBJECT

public:
    Socket(QObject* parent = nullptr);

signals:
    void connected();
    void otherConnected(); // Other device is connected
    void disconnected();

    void cameraImage(QByteArray data);

public slots:
    void connectToHost(QHostAddress address, quint16 port);
    void disconnect();

    void motor(float left, float right, float immersion);
    void camera(float xAxis, float yAxis);

private:
    QTcpSocket* m_tcp;
    QUdpSocket* m_udp;

    struct CameraBufferStruct
    {
        quint8 parts;
        QByteArray data;
    };

    QMap<quint8, CameraBufferStruct> m_cameraBuffer;
};

#endif // SOCKET_HPP