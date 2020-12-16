#include "Socket.hpp"

#include <QTcpServer>
#include <QTcpSocket>
#include <QUdpSocket>

Socket::Socket(QObject* parent):
    QObject     (parent),
    m_server    (new QTcpServer(this)),
    m_tcp       (nullptr),
    m_udp       (new QUdpSocket(this))
{

}

void listen(quint16 port)
{

}

void cameraImage(QByteArray data)
{

}

void processTcp()
{

}