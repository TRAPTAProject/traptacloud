import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

FocusScope {

    id: app
    width: 640
    height: 480
    focus: true

    property int fontSize: 18

    Rectangle {
        color: "#E0F2F1"
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: userInfo.height>eventInfo.height?userInfo.bottom:eventInfo.bottom
        anchors.bottomMargin: -15

    }

    Item {
        id: eventInfo
        anchors.top: parent.top
        anchors.right: parent.horizontalCenter
        anchors.left: parent.left
        height: childrenRect.height
        anchors.margins: 20

        Rectangle {
            id: eventLabelBg
            color: Material.primary
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.right: parent.right
            height: userLabel.height*1.5
            Text {
                id: eventLabel
                text: "Évènement"
                color: "white"
                font.pixelSize: app.fontSize
                anchors.left: parent.left
                anchors.leftMargin: 15
                anchors.verticalCenter: parent.verticalCenter
            }
        }
        TextField {
            id: eventName
            font.pixelSize: app.fontSize
            anchors.left: parent.left
            anchors.top: eventLabelBg.bottom
            anchors.topMargin: 20
            anchors.right: parent.right
            placeholderText: "Date et nom de l'évènement"
            selectByMouse: true
            maximumLength: 50
            text: cloud.eventName
            Keys.onReturnPressed: eventName.focus = false
            Keys.onEnterPressed: eventName.focus = false
            onEditingFinished: cloud.setEventName(eventName.text)
            Keys.onEscapePressed: {
                eventName.text = cloud.eventName
                eventName.focus = false
            }

        }



    }

    Item {
        id: userInfo

        anchors.top: parent.top
        anchors.right: parent.right
        anchors.left: parent.horizontalCenter
        height: childrenRect.height
        anchors.margins: 20


        Rectangle {
            id: userLabelBg
            color: Material.primary
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.right: lock.left
            height: userLabel.height*1.5

            Text {
                id: userLabel
                text: "Utilisateur"
                color: "white"
                font.pixelSize: app.fontSize
                anchors.left: parent.left
                anchors.leftMargin: 15
                anchors.verticalCenter: parent.verticalCenter
            }
        }

        Switch {
            id: lock
            anchors.right: parent.right
            anchors.top: parent.top
            ToolTip.delay: 1000
            ToolTip.visible: hovered
            ToolTip.text: "Modifier l'adresse web et mot de passe"

        }


        TextField {
            id: urlScores
            font.pixelSize: app.fontSize
            anchors.left: parent.left
            anchors.top: userLabelBg.bottom
            anchors.topMargin: 20
            anchors.right: parent.right
            selectByMouse: true
            ToolTip.delay: 1000
            ToolTip.visible: hovered
            ToolTip.text: "URL de publication des scores (ex: http://www.monsite.fr/traptascore)"
            placeholderText: "URL de publication des scores"
            text: cloud.urlScores
            Keys.onReturnPressed: urlScores.focus = false
            Keys.onEnterPressed: urlScores.focus = false
            onEditingFinished: cloud.setUrlScores(urlScores.displayText)
            Keys.onEscapePressed: {
                urlScores.text = cloud.urlScores
                urlScores.focus = false
            }
            enabled: lock.checked

        }

        TextField {
            id: password
            font.pixelSize: app.fontSize
            anchors.left: parent.left
            anchors.top: urlScores.bottom
            anchors.topMargin: 20
            anchors.right: parent.right
            selectByMouse: true
            echoMode: lock.checked?TextInput.Normal:TextInput.Password
            placeholderText: "Mot de passe"
            text: cloud.password
            Keys.onReturnPressed: password.focus = false
            Keys.onEnterPressed: password.focus = false
            onEditingFinished: cloud.setPassword(password.text)
            Keys.onEscapePressed: {
                password.text = cloud.password
                password.focus = false
            }
            enabled: lock.checked

        }

        Component.onCompleted: {
            lock.checked = (urlScores.text === "" || password.text === "")
        }
    }


    Item {
        id: visualStatus

        property bool traptalink: false
        property bool traptaDiscovered: viewcontroller.traptaHost!=="" && viewcontroller.traptaPort!==0

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.topMargin: 50
        anchors.leftMargin: 20
        anchors.rightMargin: 20
        anchors.top: userInfo.height>eventInfo.height?userInfo.bottom:eventInfo.bottom
        height: 70

        Rectangle {
            id: horizontalLine
            color: Material.primary
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: 25
            anchors.verticalCenter: traptacloudImage.verticalCenter
            height: 3
        }

        Image {
            id: traptaImage
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            sourceSize.height: parent.height
            fillMode: Image.PreserveAspectFit
            source:visualStatus.traptaDiscovered?"qrc:/images/trapta.png":"qrc:/images/trapta-disabled.png"

        }

        Label {
            font.pixelSize: app.fontSize*0.7
            text: visualStatus.traptaDiscovered?"TRAPTA: "+viewcontroller.traptaHost+":"+viewcontroller.traptaPort:"TRAPTA non détecté"
            anchors.left: traptaImage.left
            anchors.bottom: traptaImage.top
        }


        Image {
            id: traptacloudImage
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            sourceSize.height: parent.height
            fillMode: Image.PreserveAspectFit
            source:"qrc:/images/traptacloud.png"
        }

        Label {
            font.pixelSize: app.fontSize*0.7
            text: "TRAPTA Cloud"
            anchors.horizontalCenter: traptacloudImage.horizontalCenter
            anchors.bottom: traptacloudImage.top
        }


        Image {
            id: cloudImage
            anchors.bottom: parent.bottom
            anchors.right: parent.right
            sourceSize.height: parent.height
            fillMode: Image.PreserveAspectFit
            source:"qrc:/images/cloud.png"
        }

        Label {
            font.pixelSize: app.fontSize*0.7
            text: "Page web (internet)"
            anchors.right: cloudImage.right
            anchors.bottom: cloudImage.top
        }


        Image {
            id: traptalinkImage
            anchors.verticalCenter: horizontalLine.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: parent.width*0.25
            sourceSize.height:40
            fillMode: Image.PreserveAspectFit
            source: "qrc:/images/cross.png"
        }

        Image {
            id: cloudlinkImage
            anchors.verticalCenter: horizontalLine.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: parent.width*0.25
            sourceSize.height:40
            fillMode: Image.PreserveAspectFit
            source: cloud.linkStatus===1?"qrc:/images/tick.png":
                    cloud.linkStatus===0?"qrc:/images/warning.png":"qrc:/images/cross.png"
        }

        Switch {
            id: connectButton
            font.pixelSize: app.fontSize
            anchors.left: traptaImage.left
            anchors.top: traptaImage.bottom
            text: "Connection à TRAPTA"
            enabled: visualStatus.traptaDiscovered
            onCheckedChanged: connectButton.checked?viewcontroller.connectToServer():viewcontroller.disconnectFromServer()
        }

        Connections {
            target: viewcontroller
            function onDisconnected() {
                connectButton.checked = false
                traptalinkImage.source = "qrc:/images/cross.png"
            }
            function onConnected() {
                connectButton.checked = true
                traptalinkImage.source = "qrc:/images/tick.png"
            }
        }
    }

    ScrollView {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: visualStatus.bottom
        anchors.bottom: parent.bottom
        anchors.leftMargin: 20
        anchors.rightMargin: 20
        anchors.topMargin: app.fontSize*3
        anchors.bottomMargin: 20
        ScrollBar.vertical.policy: ScrollBar.AlwaysOn


        TextArea {
            id: logArea
            readOnly: true
            selectByMouse: true
            selectByKeyboard: true
            font.pixelSize: app.fontSize*0.8
            Connections {
                target: viewcontroller
                function onLog(logString) {
                    logArea.append(Qt.formatDateTime(new Date(),"hh:mm:ss:zzz")+": "+logString)
                }
            }
        }
    }

    Keys.onPressed: function(event) {
        print("Key pressed"+event.key)
        if (event.modifiers & Qt.ControlModifier) switch (event.key) {
            case Qt.Key_Minus:
                app.fontSize--;
                break;
            case Qt.Key_Plus:
            case Qt.Key_Equal:
                app.fontSize++;
                break;
        }

    }
}
