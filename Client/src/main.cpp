#include "MainWindow.hpp"
#include "UdpProtocol.hpp"

#include <QGuiApplication>

#include <QImage>
#include <QVideoFrame>

int main(int argc, char** argv)
{
    QGuiApplication app(argc, argv);

    auto mainWindow = new MainWindow(qGuiApp);

    auto a = new UdpProtocol(QHostAddress("::1"), 2137, qGuiApp);
    a->listen();
    QObject::connect(a, &UdpProtocol::error, [](auto e){qDebug() << e;});

    QObject::connect(a, &UdpProtocol::receivedData, [mainWindow](QByteArray array)
    {
        array = qUncompress(array);
        auto data = array.constData();
        auto width = *reinterpret_cast<const decltype(QImage().width())*>(data);
        auto height = *reinterpret_cast<const decltype(QImage().height())*>(data += sizeof(decltype(width)));
        auto format = *reinterpret_cast<const decltype(QImage().format())*>(data += sizeof(decltype(height)));
        auto img = reinterpret_cast<const uchar*>(data + sizeof(decltype(format)));

        mainWindow->cameraFrame(QVideoFrame(QImage(img, width, height, format)));
    });

    mainWindow->show();

    return QGuiApplication::exec();
}