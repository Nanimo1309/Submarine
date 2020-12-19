#ifndef SOCKETBASE_HPP
#define SOCKETBASE_HPP

#include <QtGlobal>

class SocketBase
{
protected:
    enum class MessageType: quint8
    {
        // From server to client
        AcceptConnection,
        OtherConnected,
        ServerDisconnect,

        ServerStatus,
        CameraData,

        // From client to server
        ConnectionQuery,
        ClientDisconnect,

        ControlData
    };

    struct Header
    {
        quint64 id;
        MessageType type;
    };

    struct ControlData
    {
        float leftMotor;
        float rightMotor;

        float immersion;

        float cameraXAxis;
        float cameraYAxis;
    };

    struct ServerStatus
    {
        quint64 controlDataId;
        quint64 controlFlagsId;
    };

    struct CameraData
    {
        quint8 part; // Number of the package
        quint64 size; // Whole data size
        // data
    };

    template
    <
        class T,

        std::enable_if_t
        <
            std::is_same_v<T, ControlData>
            || std::is_same_v<T, ServerStatus>
            || std::is_same_v<T, CameraData>,
            bool
        > = true
    >
    union ReinterpretMessage
    {
        struct:
            Header,
            T
        {} data;

        char byteData[sizeof(data)];
    };
};

#endif // SOCKETBASE_HPP