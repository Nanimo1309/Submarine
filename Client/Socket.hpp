#ifndef SOCKET_HPP
#define SOCKET_HPP

#include "SocketBase.hpp"

#include <QObject>
#include <QHostAddress>
#include <QByteArray>
#include <QMap>
#include <QList>
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
    void otherConnected();
    void lostConnection();
    void disconnected();

    void status(float batteryCharge);
    void cameraData(QByteArray data);

public slots:
    void connectToServer(quint16 port);
    void disconnect();

    void setting(float leftMotor, float rightMotor, float immersion, float cameraXAxis, float cameraYAxis);

private:
    QHostAddress m_address;
    quint16 m_port;
    QUdpSocket* m_udp;

    decltype(Header::id) m_outId;
    decltype(Header::id) m_statusInId;
    decltype(Header::id) m_cameraDataInId;

    struct CameraBufferStruct
    {
        decltype(Header::id) id;
        decltype(CameraData::part) parts;
        QByteArray data;
    };

    QList<CameraBufferStruct> m_cameraBuffer;

    QTimer m_responseTimer;
    decltype(Header::id) m_lastResponseHash;
};

#endif // SOCKET_HPP