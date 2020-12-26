#ifndef SOCKET_HPP
#define SOCKET_HPP

#include "SocketBase.hpp"

#include <QObject>
#include <QHostAddress>
#include <QByteArray>
#include <QMap>

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
    void lostConnection();
    void disconnected();

    void setting(float leftMotor, float rightMotor, float immersion, float cameraXAxis, float cameraYAxis);

public slots:
    void listen(quint16 port);
    void stopListening();

    void cameraData(QByteArray data);

private:
    QHostAddress m_address; // When socket is not connected m_address == QHostAddress::Null
    quint16 m_port;
    QUdpSocket* m_udp;

    quint64 m_outId;
    QMap<MessageType, quint64> m_inId;
};

#endif // SOCKET_HPP