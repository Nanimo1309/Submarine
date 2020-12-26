#include "Socket.hpp"

#include "QtHelper.hpp"

#include <QUdpSocket>
#include <QNetworkDatagram>

Socket::Socket(QObject* parent):
    QObject     (parent),
    m_udp       (nullptr)
{
    CONNECT(this, destroyed, this, disconnect);
}

void Socket::connectToServer(quint16 port)
{
    if(m_udp)
        disconnect();

    m_port = port;

    m_udp = new QUdpSocket;

    CONNECT(m_udp, readyRead, [this]()
    {
        auto datagram = m_udp->receiveDatagram();
        auto header = reinterpret_cast<const Header*>(datagram.data().constData());

        switch(header->type)
        {
        case MessageType::AcceptConnection:
            if(m_address.isNull() && m_port == datagram.senderPort())
            {
                m_address = datagram.senderAddress();
                connected();
            }
            break;
        case MessageType::OtherConnected:
            if(m_address.isNull() && m_port == datagram.senderPort())
            {
                m_udp->deleteLater();
                m_udp = nullptr;
                otherConnected();
            }
            break;
        case MessageType::ServerShutdown:
            if(m_address == datagram.senderAddress() && m_port == datagram.senderPort())
            {
                m_udp->deleteLater();
                m_udp = nullptr;
                disconnected();
            }
            break;
        case MessageType::ServerStatus:
            if(m_address == datagram.senderAddress() && m_port == datagram.senderPort())
            {

            }
            break;
        case MessageType::CameraData:
            if(m_address == datagram.senderAddress() && m_port == datagram.senderPort())
            {
                auto header = reinterpret_cast<const ReinterpretMessage<CameraData>*>(datagram.data().constData());

                constexpr auto maxSize = MAX_PACKET_SIZE - sizeof(*header);
                auto parts = header->data.size / maxSize + (header->data.size % maxSize ? 1 : 0);

                CameraBufferStruct* buffer = nullptr;

                for(size_t i = 0; i < m_cameraBuffer.size(); ++i)
                {
                    if(m_cameraBuffer[i].id == header->data.id)
                    {
                        buffer = &m_cameraBuffer[i];
                        break;
                    }
                }

                if(!buffer)
                {
                    m_cameraBuffer.push_back({header->data.id, 0, QByteArray()});
                    buffer = &m_cameraBuffer.back();
                    buffer->data.resize(header->data.size);
                }

                auto dataSize = datagram.data().size() - sizeof(*header);
                buffer->data.replace(header->data.part * maxSize, dataSize, datagram.data().constData() + sizeof(*header), dataSize);

                if(++buffer->parts == parts)
                {
                    cameraData(buffer->data);

                    for(size_t i = 0; i < m_cameraBuffer.size(); ++i)
                    {
                        if(m_cameraBuffer[i].id <= header->data.id)
                            m_cameraBuffer.removeAt(i);
                    }
                }
            }
            break;
        default:
            assert("Socket: Invalid server message type");
            break;
        }
    });

    m_udp->bind();

    ReinterpretMessage msg;

    msg.data.id = ++m_outId;
    msg.data.type = MessageType::ConnectionQuery;

    m_udp->writeDatagram(msg.byteData, sizeof(msg), QHostAddress::Broadcast, m_port);
}

void Socket::disconnect()
{
    if(!m_udp)
        return;

    if(!m_address.isNull())
    {
        ReinterpretMessage<void> msg;

        msg.data.id = ++m_outId;
        msg.data.type = MessageType::ClientDisconnect;

        m_udp->writeDatagram(msg.byteData, sizeof(msg), m_address, m_port);

        disconnected();
    }

    m_udp->deleteLater();
    m_udp = nullptr;
}

void Socket::setting(float leftMotor, float rightMotor, float immersion, float cameraXAxis, float cameraYAxis)
{
    if(!m_udp || m_address.isNull())
        return;

    ReinterpretMessage<ControlData> msg;

    msg.data.id = ++m_outId;
    msg.data.type = MessageType::ControlData;
    msg.data.leftMotor = leftMotor;
    msg.data.rightMotor = rightMotor;
    msg.data.immersion = immersion;
    msg.data.cameraXAxis = cameraXAxis;
    msg.data.cameraYAxis = cameraYAxis;

    m_udp->writeDatagram(msg.byteData, sizeof(msg), m_address, m_port);
}