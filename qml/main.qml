import QtQuick 2.10
import QtQuick.Controls 2.3
import QtQuick.Controls.Material 2.1
import QtQuick.Dialogs 1.0


ApplicationWindow {

    visible: true
    width: 650
    height: 550
    minimumWidth: 650
    minimumHeight: 550
    title: "TRAPTACloud 2.2"

    App {
        anchors.fill: parent
    }

    CalendarDialog {
        id: calendarDialog
    }

    PopupBox {
        id: popupBox
        Connections {
            target: viewcontroller
            onLogError: popupBox.pop(icon, title, content)
        }
    }

}
