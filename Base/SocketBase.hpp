#ifndef SOCKETBASE_HPP
#define SOCKETBASE_HPP

#include <QByteArray>

class SocketBase
{
protected:
    enum MessageType: quint8
    {
        MotorSetting,
        CameraSetting,
        CameraData
    };

    struct Message
    {
        quint64 id;
        MessageType type;
    };

    struct MotorSetting
    {
        quint8 left;
        quint8 right;
        quint8 immersion;
    };

    struct CameraSetting
    {
        quint8 xAxis;
        quint8 yAxis;
    };

    struct CameraData
    {
        quint8 part;
        quint64 size;
        QByteArray data;
    };
};

#endif // SOCKETBASE_HPP