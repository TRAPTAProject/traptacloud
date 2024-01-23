import QtQuick 2.10
import QtQuick.Controls 2.3
import QtQuick.Controls.Material 2.1


FocusScope {

    id: app
    width: 650
    height: 550
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
            placeholderText: "Nom de l'évènement"
            selectByMouse: true
            text: cloud.eventName
            Keys.onReturnPressed: eventName.focus = false
            Keys.onEnterPressed: eventName.focus = false
            onEditingFinished: cloud.setEventName(eventName.text)
            Keys.onEscapePressed: {
                eventName.text = cloud.eventName
                eventName.focus = false
            }

        }

        Button {
            id: dateButton
            anchors.top: eventName.bottom
            anchors.left: parent.left
            ToolTip.delay: 1000
            ToolTip.visible: hovered
            ToolTip.text: "Sélectionner la date de l'évènement"
            text: "  Date...  "
            onClicked: calendarDialog.open()
        }

        Label {
            id: dateLabel
            anchors.left: dateButton.right
            anchors.leftMargin: 15
            anchors.verticalCenter: dateButton.verticalCenter
            font.pixelSize: app.fontSize
            text: Qt.formatDate(cloud.eventDate, "d MMMM yyyy")
        }

        Label {
            id: timeLabel
            anchors.left: parent.left
            anchors.baseline: timeBox.baseline
            font.pixelSize: app.fontSize
            text: "Début des tirs à  "
        }

        ComboBox {
            id: timeBox
            anchors.left: timeLabel.right
            anchors.top: dateButton.bottom
            currentIndex: cloud.timeChooserIndex
            model: cloud.timeChooserList
            onCurrentIndexChanged: cloud.setTimeChooserIndex(timeBox.currentIndex)

        }

        Button {
            id: publishButton
            anchors.top: timeBox.bottom
            anchors.left: parent.left
            text: "  Publier  "
            font.pixelSize: app.fontSize
            ToolTip.delay: 1000
            ToolTip.visible: hovered
            ToolTip.text: "Afficher l'évènement sur "+cloud.url
            onClicked: cloud.publish()
            highlighted: true
        }

        Button {
            id: hideButton
            anchors.top: timeBox.bottom
            anchors.right: parent.right
            text: "  Cacher  "
            font.pixelSize: app.fontSize
            ToolTip.delay: 1000
            ToolTip.visible: hovered
            ToolTip.text: "Retirer l'évènement de "+cloud.url
            onClicked: cloud.hide()
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
            ToolTip.text: "Modifier les identifiants utilisateur"

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
            ToolTip.text: "URL de publication des scores (ex: http://score.trapta.eu)"
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
            id: urlMarques
            font.pixelSize: app.fontSize
            anchors.left: parent.left
            anchors.top: urlScores.bottom
            anchors.right: parent.right
            selectByMouse: true
            ToolTip.delay: 1000
            ToolTip.visible: hovered
            ToolTip.text: "URL de publication des feuilles de marques (ex: http://score.trapta.eu/uploadpdf.php)"
            placeholderText: "URL de publication des feuilles de marques"
            text: cloud.urlMarques
            Keys.onReturnPressed: urlMarques.focus = false
            Keys.onEnterPressed: urlMarques.focus = false
            onEditingFinished: cloud.setUrlMarques(urlMarques.displayText)
            Keys.onEscapePressed: {
                urlMarques.text = cloud.urlMarques
                urlMarques.focus = false
            }
            enabled: lock.checked

        }


        TextField {
            id: userId
            font.pixelSize: app.fontSize
            anchors.left: parent.left
            anchors.top: urlMarques.bottom
            anchors.right: parent.right
            placeholderText: "Identifiant"
            selectByMouse: true
            text: cloud.userId
            Keys.onReturnPressed: userId.focus = false
            Keys.onEnterPressed: userId.focus = false
            onEditingFinished: cloud.setUserId(userId.displayText)
            Keys.onEscapePressed: {
                userId.text = cloud.userId
                userId.focus = false
            }
            enabled: lock.checked

        }


        TextField {
            id: password
            font.pixelSize: app.fontSize
            anchors.left: parent.left
            anchors.top: userId.bottom
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
            lock.checked = (userId.text === "" || urlMarques.text === "" || urlScores.text === "" || password.text === "")
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


        Button {
            id: pdfButton
            anchors.top: traptaImage.bottom
            anchors.right: parent.right
            text: "  Publier PDF  "
            font.pixelSize: app.fontSize*0.8
            ToolTip.delay: 1000
            ToolTip.visible: hovered
            ToolTip.text: "Publier sur la page web l'ensemble des feuilles de score au format PDF"
            onClicked: Qt.openUrlExternally(urlMarques.text+"?username="+userId.text+"&password="+password.text)
        }

        Connections {
            target: viewcontroller
            onDisconnected: {
                connectButton.checked = false
                traptalinkImage.source = "qrc:/images/cross.png"
            }
            onConnected: {
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
                onLog: logArea.append(Qt.formatDateTime(new Date(),"hh:mm:ss:zzz")+": "+logString)
            }

        }

    }

    Keys.onPressed: {
        print("Key pressed"+event.key)
        if (event.modifiers & Qt.ControlModifier) switch (event.key) {
            case Qt.Key_Minus:
                app.fontSize--;
                break;
            case Qt.Key_Plus:
                app.fontSize++;
                break;
        }

    }



}
