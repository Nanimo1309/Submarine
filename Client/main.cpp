#include "Socket.hpp"
#include <QCoreApplication>
#include <QTimer>

int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);

    qDebug() << "Client";

    auto socket = new Socket(QCoreApplication::instance());

    auto timer = new QTimer(socket);

    QObject::connect(socket, &Socket::connected, [socket, timer]()
    {
        qDebug() << "Connected";

        QObject::connect(timer, &QTimer::timeout, [socket]()
        {
            socket->setting(1.23f, 2.34f, 3.45f, 4.56f, 5.67f);
        });

        timer->start(1000);
    });

    QObject::connect(socket, &Socket::disconnected, [timer]()
    {
        qDebug() << "Disconnected";

        timer->deleteLater();
    });

    socket->connectToHost(QHostAddress("::1"), 2137);

    return QCoreApplication::exec();
}