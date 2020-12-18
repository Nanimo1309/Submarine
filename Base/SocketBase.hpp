#ifndef SOCKETBASE_HPP
#define SOCKETBASE_HPP

#include <QtGlobal>

class SocketBase
{
protected:
    enum class MessageType: quint8
    {
        // Server responses
        AcceptConnection,
        OtherConnected,

        // Control data
        MotorSetting,
        CameraSetting,

        // Camera image
        CameraData
    };

    struct MotorSetting
    {
        float left;
        float right;
        float immersion;
    };

    struct CameraSetting
    {
        float xAxis;
        float yAxis;
    };

    struct CameraData
    {
        quint64 id;
        quint8 part;
        quint64 size;
        // data
    };
};

#endif // SOCKETBASE_HPP