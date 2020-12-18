#include "Socket.hpp"

#include <QTcpServer>
#include <QTcpSocket>
#include <QUdpSocket>
#include <QNetworkDatagram>

Socket::Socket(QObject* parent):
    QObject     (parent),
    m_server    (new QTcpServer(this)),
    m_tcp       (nullptr),
    m_udp       (nullptr)
{
    connect(m_server, &QTcpServer::newConnection, [this]()
    {
        if(m_tcp)
        {
            MessageType type = MessageType::OtherConnected;
            auto tcp = m_server->nextPendingConnection();
            tcp->write(reinterpret_cast<char*>(&type), sizeof(MessageType));
            tcp->deleteLater();
            return;
        }

        m_tcp = m_server->nextPendingConnection();

        connect(m_tcp, &QTcpSocket::disconnected, [this]()
        {
            m_tcp->deleteLater();
            m_tcp = nullptr;
            m_udp = nullptr;

            disconnected();
        });

        connect(m_tcp, &QTcpSocket::readyRead, [this]()
        {
            while(m_tcp->bytesAvailable())
            {
                auto type = *reinterpret_cast<const MessageType*>(m_tcp->read(sizeof(MessageType)).constData());

                switch(type)
                {
                case MessageType::MotorSetting:
                    {
                        auto message = *reinterpret_cast<const MotorSetting*>(m_tcp->read(sizeof(MotorSetting)).constData());
                        motor(message.left, message.right, message.immersion);
                        break;
                    }
                case MessageType::CameraSetting:
                    {
                        auto message = *reinterpret_cast<const CameraSetting*>(m_tcp->read(sizeof(CameraSetting)).constData());
                        camera(message.xAxis, message.yAxis);
                        break;
                    }
                default:
                    assert("Invalid request");
                    return;
                }
            }
        });

        MessageType type = MessageType::AcceptConnection;
        m_tcp->write(reinterpret_cast<char*>(&type), sizeof(MessageType));

        connected();
    });
}

void Socket::listen(quint16 port)
{
    m_server->listen(QHostAddress::Any, port);
}

void Socket::cameraImage(QByteArray data)
{
    if(m_udp)
    {

    }
}