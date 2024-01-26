import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

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
/*        anchors.horizontalCenter: parent.horizontalCenter
        selectedDate: cloud.eventDate
        onClicked: {
            print(date)
            cloud.setEventDate(date)
            close()
        }
        */

    }


}
