#ifndef SOCKET_HPP
#define SOCKET_HPP

#include "SocketBase.hpp"

#include <QObject>
#include <QHostAddress>
#include <QByteArray>
#include <QMap>
#include <QList>

class QUdpSocket;

class Socket:
    public QObject,
    protected SocketBase
{
    Q_OBJECT

public:
    Socket(QObject* parent = nullptr);

signals:
    void connected();
    void otherConnected();
    void lostConnection();
    void disconnected();

    void cameraData(QByteArray data);

public slots:
    void connectToServer(quint16 port);
    void disconnect();

    void setting(float leftMotor, float rightMotor, float immersion, float cameraXAxis, float cameraYAxis);

private:
    QHostAddress m_address;
    quint16 m_port;
    QUdpSocket* m_udp;

    quint64 m_outId;
    QMap<MessageType, quint64> m_inId;

    struct CameraBufferStruct
    {
        quint64 id;
        quint8 parts;
        QByteArray data;
    };

    QList<CameraBufferStruct> m_cameraBuffer;
};

#endif // SOCKET_HPP