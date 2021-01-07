#include "Socket.hpp"

#include "QtHelper.hpp"

#include <QUdpSocket>
#include <QNetworkDatagram>

Socket::Socket(QObject* parent):
    QObject             (parent),
    m_address           (QHostAddress::Null),
    m_udp               (nullptr),
    m_outId             (0),
    m_lastResponseHash  (0)
{
    CONNECT(&m_responseTimer, timeout, [this]()
    {
        decltype(m_lastResponseHash) temp = m_controlDataInId;

        if(m_lastResponseHash != temp)
            m_lastResponseHash = temp;
        else
        {
            m_responseTimer.stop();
            lostConnection();

            ReinterpretMessage msg;

            msg.data.id = ++m_outId;
            msg.data.type = MessageType::ServerShutdown;

            m_udp->writeDatagram(msg.byteData, sizeof(msg), m_address, m_port);

            m_address = QHostAddress::Null;

            disconnected();
        }
    });
}

void Socket::listen(quint16 port)
{
    if(m_udp)
        stopListening();

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
            case MessageType::ConnectionQuery:
                if(m_address.isNull())
                {
                    m_address = datagram.senderAddress();
                    m_port = datagram.senderPort();

                    ReinterpretMessage msg;

                    msg.data.id = ++m_outId;
                    msg.data.type = MessageType::AcceptConnection;

                    m_udp->writeDatagram(msg.byteData, sizeof(msg), m_address, m_port);

                    m_controlDataInId = 0;

                    m_responseTimer.start(MAX_SILENCE_TIME);

                    connected();
                }
                else
                {
                    ReinterpretMessage msg;

                    msg.data.id = ++m_outId;
                    msg.data.type = MessageType::OtherConnected;

                    m_udp->writeDatagram(msg.byteData, sizeof(msg), datagram.senderAddress(), datagram.senderPort());
                }
                break;
            case MessageType::ClientDisconnect:
                if(m_address == datagram.senderAddress() && m_port == datagram.senderPort())
                {
                    m_responseTimer.stop();

                    m_address = QHostAddress::Null;

                    disconnected();
                }
                break;
            case MessageType::ControlData:
                if(m_address == datagram.senderAddress() && m_port == datagram.senderPort() && m_controlDataInId < header->id)
                {
                    auto msg = reinterpret_cast<const ReinterpretMessage<ControlData>*>(datagram.data().constData());

                    m_controlDataInId = msg->data.id;

                    setting(msg->data.leftMotor, msg->data.rightMotor, msg->data.immersion, msg->data.cameraXAxis, msg->data.cameraYAxis);
                }
                break;
            default:
                assert("Socket: Invalid client message type");
                break;
            }
        }
    });

    if(!m_udp->bind(port))
        throw std::runtime_error("Socket: Cannot bind server to specified port");
}

void Socket::stopListening()
{
    if(!m_udp)
        return;

    if(!m_address.isNull())
    {
        m_responseTimer.stop();

        ReinterpretMessage msg;

        msg.data.id = ++m_outId;
        msg.data.type = MessageType::ServerShutdown;

        m_udp->writeDatagram(msg.byteData, sizeof(msg), m_address, m_port);

        m_address = QHostAddress::Null;

        disconnected();
    }

    m_udp->deleteLater();
    m_udp = nullptr;
}

void Socket::status(float batteryCharge)
{
    if(!m_udp || m_address.isNull())
        return;

    ReinterpretMessage<Status> msg;

    msg.data.id = ++m_outId;
    msg.data.type = MessageType::Status;
    msg.data.batteryCharge = batteryCharge;

    m_udp->writeDatagram(msg.byteData, sizeof(msg), m_address, m_port);
}

void Socket::cameraData(QByteArray data)
{
    if(!m_udp || m_address.isNull())
        return;

    QByteArray msg;

    msg.resize(sizeof(ReinterpretMessage<CameraData>));

    auto header = reinterpret_cast<ReinterpretMessage<CameraData>*>(msg.data());

    header->data.id = ++m_outId;
    header->data.type = MessageType::CameraData;
    header->data.size = data.size();

    size_t sended = 0;

    constexpr auto maxSize = MAX_PACKET_SIZE - sizeof(*header);
    auto parts = header->data.size / maxSize + (header->data.size % maxSize ? 1 : 0);

    for(size_t i = 0; i < parts; ++i)
    {
        msg.resize(sizeof(*header));
        header = reinterpret_cast<ReinterpretMessage<CameraData>*>(msg.data());
        header->data.part = i;

        msg.append(data.mid(sended, maxSize).data());
        sended += maxSize;

        m_udp->writeDatagram(msg, m_address, m_port);
    }
}