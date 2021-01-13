#include "MainWindow.hpp"
#include "Socket.hpp"

#include "QtHelper.hpp"

#include <QGuiApplication>
#include <QTimer>

#include <QImage>
#include <QVideoFrame>

int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);

    auto mainWindow = new MainWindow(qGuiApp);

    qDebug() << "Client";

    auto socket = new Socket(QCoreApplication::instance());

    auto timer = new QTimer(socket);
    auto timer2 = new QTimer(socket);

    CONNECT(timer, timeout, [socket](){socket->connectToServer(2137);});
    timer->start(500);

    CONNECT(socket, connected, ([socket, timer, timer2]()
    {
        qDebug() << "Connected";

        timer->stop();

        CONNECT(timer2, timeout, [socket]()
        {
            socket->setting(1.23f, 2.34f, 3.45f, 4.56f, 5.67f);
        });

        timer2->start(200);
    }));

    CONNECT(socket, otherConnected, []()
    {
        qDebug() << "Other Connected";
    });

    CONNECT(socket, lostConnection, []()
    {
        qDebug() << "Lost Connection";
    });

    CONNECT(socket, disconnected, [timer2]()
    {
        qDebug() << "Disconnected";

        timer2->stop();
    });

    CONNECT(socket, cameraData, [mainWindow](QByteArray array)
    {
        qDebug() << "Received data";

        array = qUncompress(array);
        auto data = array.constData();
        auto width = *reinterpret_cast<const decltype(QImage().width())*>(data);
        auto height = *reinterpret_cast<const decltype(QImage().height())*>(data += sizeof(decltype(width)));
        auto format = *reinterpret_cast<const decltype(QImage().format())*>(data += sizeof(decltype(height)));
        auto img = reinterpret_cast<const uchar*>(data + sizeof(decltype(format)));

        mainWindow->cameraFrame(QVideoFrame(QImage(img, width, height, format)));
    });

    CONNECT(socket, status, [](float batteryCharge)
    {
        qDebug() << batteryCharge;
    });

    mainWindow->show();

    return QGuiApplication::exec();
}