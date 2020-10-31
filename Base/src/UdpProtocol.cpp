#include "UdpProtocol.hpp"

#include <QUdpSocket>
#include <QNetworkDatagram>

UdpProtocol::UdpProtocol(const QHostAddress& address, quint16 port, QObject* parent):
    QObject     (parent),
    m_socket    (new QUdpSocket(this)),
    m_address   (address),
    m_port      (port),
    m_inIndex   (0),
    m_outIndex  (0)
{
    QObject::connect(m_socket, &QUdpSocket::readyRead, this, &UdpProtocol::readData);
}

bool UdpProtocol::listen()
{
    return m_socket->bind(m_address, m_port);
}

void UdpProtocol::stopListening()
{
    m_socket->close();
}

void UdpProtocol::sendData(QByteArray data)
{
    data.prepend(reinterpret_cast<char*>(&++m_outIndex), sizeof(m_outIndex));
    if(m_socket->writeDatagram(data, m_address, m_port) == -1)
        error(m_socket->error());
}

void UdpProtocol::readData()
{
    auto datagram = m_socket->receiveDatagram().data();
    auto inIndex = *reinterpret_cast<const decltype(m_inIndex)*>(datagram.constData());

    if(m_inIndex < inIndex)
    {
        m_inIndex = inIndex;
        receivedData(datagram.right(datagram.size() - sizeof(m_inIndex)));
    }
}