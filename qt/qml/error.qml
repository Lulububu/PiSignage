import QtQuick 2.0
Item{
    Rectangle {
        anchors.fill: parent
        color: "gray"

        Text {
            id: name
            anchors.centerIn: parent
            text: "error"
            font.family: "Helvetica"
            font.pointSize: 22
        }
    }

}
