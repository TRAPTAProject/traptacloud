import QtQuick 2.10
import QtQuick.Controls 2.3
import QtQuick.Controls.Material 2.1


Dialog {
    id: popupbox

    focus: true
    width: parent.width
    contentHeight: parent.height*0.5
    topMargin: (parent.height-contentHeight)/2
    modal: true
    closePolicy: Popup.CloseOnEscape

    property int fontSize: 18
    property int icon: 1
    property alias heading: headingLabel.text
    property alias content: contentLabel.text

    Image {
        id: iconImage
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        sourceSize.height: 80
        fillMode: Image.PreserveAspectFit
        source: popupbox.icon===-1?"qrc:/images/cross.png":
                popupbox.icon===0?"qrc:/images/warning.png":"qrc:/images/tick.png"
    }

    Label {
        id: headingLabel
        anchors.top: parent.top
        anchors.left: iconImage.right
        anchors.right: parent.right
        font.pixelSize: fontSize*1.25
        font.weight: Font.Bold
        anchors.margins: 15
    }

    Label {
        id: contentLabel
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: iconImage.right
        anchors.right: parent.right
        font.pixelSize: fontSize
        anchors.margins: 15
        wrapMode: Text.Wrap

    }


    DialogButtonBox {
        id: dialogButtonBox
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right

        alignment: Qt.AlignHCenter
        standardButtons: DialogButtonBox.Ok
        onAccepted: popupbox.accept()
    }

    function pop(icon, title, content) {
        popupbox.heading = title
        popupbox.content = content
        popupbox.icon = icon
        popupbox.open()
    }

}
