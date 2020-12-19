#include "Socket.hpp"

#include <QUdpSocket>
#include <QNetworkDatagram>

Socket::Socket(QObject* parent):
    QObject (parent),
    m_udp   (nullptr)
{

}

void Socket::connectToHost(QHostAddress address, quint16 port)
{

}

void Socket::disconnect()
{

}

void Socket::setting(float leftMotor, float rightMotor, float immersion, float cameraXAxis, float cameraYAxis)
{

}