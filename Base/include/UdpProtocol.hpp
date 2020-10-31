#ifndef UDPPROTOCOL_HPP
#define UDPPROTOCOL_HPP

#include <QHostAddress>
#include <QByteArray>

class QUdpSocket;

class UdpProtocol:
    public QObject
{
    Q_OBJECT
    Q_PROPERTY(QHostAddress address MEMBER m_address)
    Q_PROPERTY(quint16 port MEMBER m_port)

public:
    UdpProtocol(const QHostAddress& address, quint16 port, QObject* parent = nullptr);

    bool listen();
    void stopListening();

signals:
    void receivedData(QByteArray data);
    void error(QAbstractSocket::SocketError error);

public slots:
    void sendData(QByteArray data);

private:
    void readData();

    QUdpSocket* m_socket;
    QHostAddress m_address;
    quint16 m_port;

    quint64 m_inIndex;
    quint64 m_outIndex;
};

#endif //UDPPROTOCOL_HPP