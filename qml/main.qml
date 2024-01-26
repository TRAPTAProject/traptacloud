import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Dialogs


Window {

    visible: true
    width: 640
    height: 600
    minimumWidth: 640
    minimumHeight: 480
    title: "TRAPTACloud 2.5.0"

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
