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

    CONNECT(socket, connected, []()
    {
        qDebug() << "Connected";
    });

    CONNECT(socket, disconnected, []()
    {
        qDebug() << "Disconnected";
    });

    CONNECT(socket, setting, [](float left, float right, float immersion, float cameraXAxis, float cameraYAxis)
    {
        qDebug() << "Motor: " << left << right << immersion << cameraXAxis << cameraYAxis;
    });

    QTimer::singleShot(1000, [socket]()
    {
        QByteArray data;

        for(size_t i = 0; i < 1000; ++i)
        {
            data.append(QString::number(i).toLocal8Bit());
            data.append("_");
        }

        socket->cameraData(data);
    });

    return QCoreApplication::exec();
}