import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtMultimedia 5.15

Window
{
    id: window
    title: "Submarine Controler"
    width: 800
    height: 600
    color: "#222"

    Rectangle
    {
        color: "#333"
        anchors.fill: parent
        anchors.margins: 5

        VideoOutput
        {
            objectName: "cameraDisplay"
            anchors.fill: parent
        }
    }

    Item
    {
        objectName: "controler"
        focus: true

        signal rightMotor(real power)
        signal leftMotor(real power)
        signal immersion(real depth)

        Keys.onPressed: function(e)
        {
            if(e.key == Qt.Key_Q)
            {
                window.close();
            }
        }
    }
}