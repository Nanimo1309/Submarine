#include "Socket.hpp"
#include <QCoreApplication>

int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);

    qDebug() << "Server";

    auto socket = new Socket(QCoreApplication::instance());

    socket->listen(2137);

    QObject::connect(socket, &Socket::connected, []()
    {
        qDebug() << "Connected";
    });

    QObject::connect(socket, &Socket::disconnected, []()
    {
        qDebug() << "Disconnected";
    });

    QObject::connect(socket, &Socket::motor, [](float left, float right, float immersion)
    {
        qDebug() << "Motor: " << left << right << immersion;
    });

    QObject::connect(socket, &Socket::camera, [](float x, float y)
    {
        qDebug() << "Camera: " << x << y;
    });

    return QCoreApplication::exec();
}