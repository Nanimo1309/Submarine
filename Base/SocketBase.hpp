#ifndef SOCKETBASE_HPP
#define SOCKETBASE_HPP

#include <QtGlobal>
#include <algorithm>

class SocketBase
{
protected:
    constexpr const static size_t MAX_PACKET_SIZE = 1000; // Have to be greater than ReinterpretMessage

    enum class MessageType: quint8
    {
        // From server to client
        AcceptConnection,
        OtherConnected,
        ServerShutdown,

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
        float bateryCharge;
    };

    struct CameraData
    {
        quint8 part; // Number of the package
        quint64 size; // Whole data size
        // data
    };

    template<class T = void, class Enable = void>
    union ReinterpretMessage;

    template<class T>
    union ReinterpretMessage<T, std::enable_if_t<std::is_same_v<T, void>>>
    {
        Header data;
        char byteData[sizeof(data)];
    };

    template<class T>
    union ReinterpretMessage<T, std::enable_if_t<std::is_same_v<T, ControlData> || std::is_same_v<T, ServerStatus> || std::is_same_v<T, CameraData>>>
    {
        struct:
            Header,
            T
        {} data;

        char byteData[sizeof(data)];
    };
};

#endif // SOCKETBASE_HPP