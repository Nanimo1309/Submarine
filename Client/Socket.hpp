#ifndef SOCKET_HPP
#define SOCKET_HPP

#include "SocketBase.hpp"

#include <QObject>
#include <QHostAddress>
#include <QByteArray>
#include <QList>

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

    void cameraData(QByteArray data);

public slots:
    void connectToHost(QHostAddress address, quint16 port);
    void disconnect();

    void setting(float leftMotor, float rightMotor, float immersion, float cameraXAxis, float cameraYAxis);

private:
    QHostAddress m_address;
    quint16 port;
    QUdpSocket* m_udp;

    struct CameraBufferStruct
    {
        quint64 id;
        quint8 parts;
        QByteArray data;
    };

    QList<CameraBufferStruct> m_cameraBuffer;
};

#endif // SOCKET_HPP