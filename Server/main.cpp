#include "Socket.hpp"

#include "QtHelper.hpp"

#include <QGuiApplication>
#include <QTimer>
#include <QMovie>
#include <QString>

int main(int argc, char** argv)
{
    QGuiApplication app(argc, argv);

    qDebug() << "Server";

    auto socket = new Socket(qGuiApp);

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

        auto xd = new QMovie(qGuiApp);
        xd->setFileName(":xd.gif");
        CONNECT(xd, frameChanged, ([socket,xd](int)
        {
            auto img = xd->currentImage();
            auto width = img.width();
            auto height = img.height();
            auto format = img.format();

            QByteArray data;
            data.append(reinterpret_cast<const char*>(&width), sizeof(decltype(width)))
                .append(reinterpret_cast<const char*>(&height), sizeof(decltype(height)))
                .append(reinterpret_cast<const char*>(&format), sizeof(decltype(format)))
                .append(QByteArray::fromRawData(reinterpret_cast<char*>(img.bits()), img.sizeInBytes()));

            socket->cameraData(qCompress(data));
        }));

        if(xd->loopCount() == -1)
            CONNECT(xd, finished, xd, start);

        xd->start();
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

    return QGuiApplication::exec();
}