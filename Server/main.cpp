#include "Socket.hpp"

#include "QtHelper.hpp"

#include <QCoreApplication>
#include <QTimer>
#include <QString>

int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);

    qDebug() << "Server";

    auto socket = new Socket(QCoreApplication::instance());

    socket->listen(2137);

    CONNECT(socket, connected, [socket]()
    {
        qDebug() << "Connected";

        auto timer = new QTimer(socket);

        CONNECT(timer, timeout, [socket]()
        {
            socket->status(99.8f);
        });

        timer->start(200);

        QTimer::singleShot(1000, [socket]()
        {
            QByteArray temp;

            for(size_t i = 0; i < 50000; ++i)
                temp.append(std::to_string(i));

            socket->cameraData(temp);
        });
    });

    CONNECT(socket, lostConnection, []()
    {
        qDebug() << "Lost Connection";
    });

    CONNECT(socket, disconnected, []()
    {
        qDebug() << "Disconnected";
    });

    CONNECT(socket, setting, [](float left, float right, float immersion, float cameraXAxis, float cameraYAxis)
    {
        qDebug() << "Motor: " << left << right << immersion << cameraXAxis << cameraYAxis;
    });

    return QCoreApplication::exec();
}