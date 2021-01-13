#ifndef SOCKET_HPP
#define SOCKET_HPP

#include "SocketBase.hpp"

#include <QObject>
#include <QHostAddress>
#include <QByteArray>
#include <QMap>
#include <QTimer>

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

    void status(float batteryCharge);
    void cameraData(QByteArray data);

private:
    QHostAddress m_address; // When socket is not connected m_address == QHostAddress::Null
    quint16 m_port;
    QUdpSocket* m_udp;

    decltype(Header::id) m_outId;
    decltype(Header::id) m_controlDataInId;

    QTimer m_responseTimer;
    decltype(Header::id) m_lastResponseHash;
};

#endif // SOCKET_HPP