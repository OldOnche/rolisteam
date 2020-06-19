import QtQuick 2.7
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.12
import InstantMessaging 1.0

Item {
    id: root
    anchors.fill: parent
    property ChatRoom chatRoom: _ctrl.globalChatroom


    Drawer {
        id: add_chat
        width: 0.33 * parent.width
        height: parent.height
        edge: Qt.RightEdge

        Frame {
            anchors.fill: parent

            ColumnLayout {

                RowLayout {
                    Layout.fillWidth: true
                    Label{
                        text: qsTr("Title")
                    }
                    TextField {
                        id: title
                    }
                }
                RowLayout {
                    Layout.fillWidth: true
                    Label{
                        text: qsTr("Everybody")
                    }
                    Switch {
                        id: all
                        checked: true
                    }
                }
                Frame {
                    id: recipiants
                    enabled: !all.checked
                    property int checkedItem: 0
                    ColumnLayout {
                        Repeater {
                            model: contact
                            RowLayout {
                                Layout.fillWidth: true
                                Image {
                                    source: model.avatar
                                    fillMode: Image.PreserveAspectFit
                                    sourceSize.width: 50
                                    sourceSize.height: 50
                                    opacity: all.checked ? 0.5 : 1.0
                                }
                                Label {
                                    text: model.name
                                    Layout.fillWidth: true
                                }
                                Switch {
                                    enabled: !all.checked
                                    onCheckedChanged: recipiants.checkedItem += checked ? 1 : -1
                                }
                            }
                        }
                    }
                }

                Button {
                    text: "add"
                    enabled: title.text.length > 0 && (all.checked || recipiants.checkedItem > 0 )
                }
            }
        }
    }

    ColumnLayout {
        anchors.fill: parent
        RowLayout {
            Layout.fillWidth: true
            TabBar {
                id: tabHeader
                Layout.fillWidth: true
                Repeater {
                    id: repeater
                    model : _ctrl.mainModel
                    TabButton {
                        id: tabButton
                        text: model.title
                        ToolButton {
                            anchors.right: parent.right
                            anchors.top: parent.top
                            anchors.bottom: parent.bottom
                            anchors.margins: 5
                            text: "^"
                            ToolTip.text: "detach"
                            ToolTip.visible: down
                        }
                        onClicked: root.chatRoom = model.chatroom
                    }
                }
            }
            ToolButton {
                text: "+"
                onClicked: add_chat.open()
            }
        }
        SplitView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.leftMargin: 10
            Layout.rightMargin: 10
            orientation: Qt.Vertical

            ListView {
                id: listView
                SplitView.fillWidth: true
                SplitView.fillHeight: true
                model: chatRoom.messageModel
                clip: true
                verticalLayoutDirection: ListView.BottomToTop
                delegate: Column {
                    anchors.right: model.local ? parent.right : undefined
                    spacing: 6
                    Row {
                        id: messageRow
                        spacing: 6
                        anchors.right: model.local ? parent.right : undefined

                        Image {
                            id: avatar
                            source: "image://avatar/%1".arg(model.writerId)
                            visible: !model.local
                            fillMode: Image.PreserveAspectFit
                            property int side: 50
                            sourceSize.height: side
                            sourceSize.width:  side
                        }

                        Rectangle {
                            id: rect
                            width: Math.min(messageId.contentWidth + 24,
                                            listView.width - (!model.local ? avatar.width + messageRow.spacing : 0))
                            height: messageId.implicitHeight + 24
                            color: model.local ? "lightgrey" : "steelblue"
                            Label {
                                id: messageId
                                text: model.text
                                anchors.fill: parent
                                anchors.margins: 12
                                wrapMode: Label.Wrap
                            }
                        }

                    }
                    Label {
                        id: timestamp
                        text: model.time
                        anchors.right: model.local ? parent.right : undefined
                        font.pixelSize: 10
                        opacity: 0.6
                    }
                }
            }
            InstantMessagingEditText {
                id: imEditText
                SplitView.fillWidth: true
                SplitView.preferredHeight: 100
                model: _ctrl.localPersonModel
                onSendClicked: root.chatRoom.addMessage(text, imEditText.currentPersonId)
            }
        }
    }
}

