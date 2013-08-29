import QtQuick 2.0
Rectangle{
    z: 4
    property string percent: ""
anchors.centerIn: parent
    Image {
        anchors.centerIn: parent
        source: "../res/WaitNote.png"
        NumberAnimation on rotation {
            loops: Animation.Infinite
            from: 0
            to: 360
            duration: 1500 // Define the desired rotation speed.
        }

    }

    Text {
        anchors.centerIn: parent
        text: percent
        font.family: "Helvetica"
        font.pointSize: 22
        color: "gray"
    }
}
