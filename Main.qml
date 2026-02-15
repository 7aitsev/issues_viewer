import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ApplicationWindow {
    width: 640
    height: 480
    visible: true
    title: "Issues Viewer"

    id: root

    property int current_page_count: 1
    property int total_page_count: 1

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 16
        spacing: 16

        RowLayout {
            id: input_row

            Layout.fillWidth: true

            spacing: 16

            TextField {
                id: path
                placeholderText: "Enter the URL of a GitHub repository..."
                Layout.fillWidth: true
                Layout.preferredHeight: 40

                onTextChanged: presenter.on_path_changed(text)
            }

            Button {
                id: load
                text: "Load"

                enabled: presenter.load_enabled

                onClicked: presenter.on_load_issues(path.text)
            }
        }

        Rectangle {
            border.color: "#AEAEAE"
            radius: 4
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true

            ListView {
                id: list_view
                anchors.fill: parent

                enabled: presenter.content_available

                model: 30

                delegate: Label {
                    text: "Item " + (index + 1)
                    font.pixelSize: 16
                    padding: 4
                }

                ScrollBar.vertical: ScrollBar {
                    policy: ScrollBar.AlwaysOn
                }
            }
        }

        RowLayout {
            spacing: 16

            enabled: presenter.content_available

            Button {
                // text: "<"
                icon.name: "go-previous"
                Layout.preferredHeight: path.implicitHeight
            }

            Label {
                text: current_page_count + " / " + total_page_count
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                Layout.fillWidth: true
            }

            Button {
                // text: ">"
                icon.name: "go-next"
                Layout.preferredHeight: path.implicitHeight
            }
        }
    }
}
