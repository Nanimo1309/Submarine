#include "Socket.hpp"

#include <QUdpSocket>
#include <QNetworkDatagram>

Socket::Socket(QObject* parent):
    QObject (parent),
    m_udp   (nullptr)
{

}

void Socket::listen(quint16 port)
{

}

void Socket::cameraData(QByteArray data)
{

}