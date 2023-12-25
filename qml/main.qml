import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

Window {

    visible: true
    width: 640
    height: 480
    minimumWidth: 640
    minimumHeight: 480
    title: "TRAPTACloud Simple 2"

    App {
        anchors.fill: parent
    }

    PopupBox {
        id: popupBox
        Connections {
            target: viewcontroller
            function onLogError(icon, title, content) {
                popupBox.pop(icon, title, content)
            }
        }
    }

}
