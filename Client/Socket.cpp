#include "Socket.hpp"

#include <QTcpSocket>
#include <QUdpSocket>

Socket::Socket(QObject* parent):
    QObject (parent),
    m_tcp   (new QTcpSocket(this)),
    m_udp   (new QUdpSocket(this))
{

}

void Socket::connect(QHostAddress address)
{

}

void Socket::disconnect()
{

}

void Socket::setMotor(float left, float right, float immersion)
{

}

void Socket::setCamera(float xAxis, float yAxis)
{

}