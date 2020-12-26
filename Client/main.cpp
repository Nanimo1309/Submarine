#include "Socket.hpp"

#include "QtHelper.hpp"

#include <QCoreApplication>
#include <QTimer>

int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);

    qDebug() << "Client";

    auto socket = new Socket(QCoreApplication::instance());

    auto timer = new QTimer(socket);

    CONNECT(socket, connected, ([socket, timer]()
    {
        qDebug() << "Connected";

        CONNECT(timer, timeout, [socket]()
        {
            socket->setting(1.23f, 2.34f, 3.45f, 4.56f, 5.67f);
        });

        timer->start(1000);

        CONNECT(socket, cameraData, [](QByteArray data)
        {
            qDebug() << data;
        });
    }));

    CONNECT(socket, otherConnected, []()
    {
        qDebug() << "Other Connected";
    });

    CONNECT(socket, disconnected, [timer]()
    {
        qDebug() << "Disconnected";

        timer->deleteLater();
    });

    socket->connectToServer(2137);

    //QTimer::singleShot(2000, [socket](){socket->disconnect(); qDebug() << "Disconnected";});
    //QTimer::singleShot(2000, [socket](){socket->deleteLater(); qDebug() << "Deleted";});

    return QCoreApplication::exec();
}