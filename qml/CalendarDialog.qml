import QtQuick 2.10
import QtQuick.Controls 2.3
import QtQuick.Controls 1.4
import QtQuick.Controls.Material 2.1

Dialog {
    id: calendarDialog

    focus: true
    width: parent.width
    contentHeight: myCalendar.height
    topMargin: (parent.height-contentHeight)/2
    modal: true
    closePolicy: Popup.CloseOnEscape

    property int fontSize: 18

    Calendar {
        id: myCalendar
        anchors.horizontalCenter: parent.horizontalCenter
        selectedDate: cloud.eventDate
        onClicked: {
            print(date)
            cloud.setEventDate(date)
            close()
        }

    }


}
