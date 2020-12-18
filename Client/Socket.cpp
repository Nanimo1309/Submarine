#include "Socket.hpp"

#include <QTcpSocket>
#include <QUdpSocket>

Socket::Socket(QObject* parent):
    QObject (parent),
    m_tcp   (nullptr),
    m_udp   (nullptr)
{}

void Socket::connectToHost(QHostAddress address, quint16 port)
{
    if(m_tcp)
        disconnect();

    m_tcp = new QTcpSocket(this);

    connect(m_tcp, &QTcpSocket::errorOccurred, [](QAbstractSocket::SocketError socketError)
    {
        qDebug() << "Tcp socket error: " << socketError;
    });

    connect(m_tcp, &QTcpSocket::connected, [this]()
    {
        auto type = *reinterpret_cast<const MessageType*>(m_tcp->read(sizeof(MessageType)).constData());

        switch(type)
        {
        case MessageType::AcceptConnection:

            break;
        case MessageType::OtherConnected:
            otherConnected();
            return;
        default:
            assert("Invalid server greeting");
            return;
        }

        m_udp = new QUdpSocket(m_tcp);

        connect(m_udp, &QUdpSocket::errorOccurred, [](QAbstractSocket::SocketError socketError)
        {
            qDebug() << "Udp socket error: " << socketError;
        });

        connect(m_udp, &QUdpSocket::readyRead, [this]()
        {
            cameraImage(QByteArray());
        });

        connected();
    });

    connect(m_tcp, &QTcpSocket::disconnected, this, &Socket::disconnected);

    m_tcp->connectToHost(address, port);
}

void Socket::disconnect()
{
    m_tcp->disconnectFromHost();
    m_tcp->deleteLater();
    m_tcp = nullptr;
    m_udp = nullptr;
}

void Socket::motor(float left, float right, float immersion)
{
    if(m_tcp)
    {
        MessageType type = MessageType::MotorSetting;
        MotorSetting data = {left, right, immersion};

        m_tcp->write(reinterpret_cast<char*>(&type), sizeof(type));
        m_tcp->write(reinterpret_cast<char*>(&data), sizeof(data));
        m_tcp->flush();
    }
}

void Socket::camera(float xAxis, float yAxis)
{
    if(m_tcp)
    {
        MessageType type = MessageType::CameraSetting;
        CameraSetting data = {xAxis, yAxis};

        m_tcp->write(reinterpret_cast<char*>(&type), sizeof(type));
        m_tcp->write(reinterpret_cast<char*>(&data), sizeof(data));
        m_tcp->flush();
    }
}