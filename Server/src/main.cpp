#include "UdpProtocol.hpp"

#include <QGuiApplication>
#include <QTimer>
#include <QMovie>

int main(int argc, char** argv)
{
    QGuiApplication app(argc, argv);

    auto proto = new UdpProtocol(QHostAddress("::1"), 2137, qGuiApp);
    QObject::connect(proto, &UdpProtocol::error, [](auto e){qDebug() << e;});

    auto xd = new QMovie(qGuiApp);
    xd->setFileName(":xd.gif");
    xd->start();
    QObject::connect(xd, &QMovie::frameChanged, [proto, xd](int)
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

        proto->sendData(qCompress(data));
    });

    if(xd->loopCount() == -1)
        QObject::connect(xd, &QMovie::finished, xd, &QMovie::start);

    return QGuiApplication::exec();
}