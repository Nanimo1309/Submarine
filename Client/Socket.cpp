#include "Socket.hpp"

#include "QtHelper.hpp"

#include <QUdpSocket>
#include <QNetworkDatagram>

Socket::Socket(QObject* parent):
    QObject             (parent),
    m_udp               (nullptr),
    m_lastResponseHash  (0)
{
    CONNECT(&m_responseTimer, timeout, [this]()
    {
        decltype(m_lastResponseHash) temp = m_statusInId + m_cameraDataInId;

        if(m_lastResponseHash != temp)
            m_lastResponseHash = temp;
        else
        {
            lostConnection();
            disconnect();
        }
    });
}

void Socket::connectToServer(quint16 port)
{
    if(m_udp)
        disconnect();

    m_port = port;

    m_udp = new QUdpSocket;

    CONNECT(m_udp, errorOccurred, [](QAbstractSocket::SocketError error){qDebug() << error;});

    CONNECT(m_udp, readyRead, [this]()
    {
        while(m_udp->hasPendingDatagrams())
        {
            auto datagram = m_udp->receiveDatagram();
            auto header = reinterpret_cast<const Header*>(datagram.data().constData());

            switch(header->type)
            {
            case MessageType::AcceptConnection:
                if(m_address.isNull() && m_port == datagram.senderPort())
                {
                    m_address = datagram.senderAddress();

                    m_statusInId = 0;
                    m_cameraDataInId = 0;

                    m_responseTimer.start(MAX_SILENCE_TIME);

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
                    m_responseTimer.stop();

                    m_udp->deleteLater();
                    m_udp = nullptr;
                    disconnected();
                }
                break;
            case MessageType::Status:
                if(m_address == datagram.senderAddress() && m_port == datagram.senderPort() && m_statusInId < header->id)
                {
                    auto header = reinterpret_cast<const ReinterpretMessage<Status>*>(datagram.data().constData());

                    m_statusInId = header->data.id;

                    status(header->data.batteryCharge);
                }
                break;
            case MessageType::CameraData:
                if(m_address == datagram.senderAddress() && m_port == datagram.senderPort() && m_cameraDataInId < header->id)
                {
                    auto header = reinterpret_cast<const ReinterpretMessage<CameraData>*>(datagram.data().constData());

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

                    constexpr auto maxSize = MAX_PACKET_SIZE - sizeof(*header);

                    auto dataSize = datagram.data().size() - sizeof(*header);
                    buffer->data.replace(header->data.part * maxSize, dataSize, datagram.data().constData() + sizeof(*header), dataSize);

                    if(++buffer->parts == header->data.size / maxSize + (header->data.size % maxSize ? 1 : 0))
                    {
                        m_cameraDataInId = buffer->id;

                        cameraData(buffer->data);

                        for(size_t i = 0; i < m_cameraBuffer.size(); ++i)
                            if(m_cameraBuffer[i].id <= buffer->id)
                                m_cameraBuffer.removeAt(i--);
                    }
                }
                break;
            default:
                assert("Socket: Invalid server message type");
                break;
            }
        }
    });

    if(!m_udp->bind())
        throw std::runtime_error("Socket: Cannot bind server to specified port");

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
        m_responseTimer.stop();

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