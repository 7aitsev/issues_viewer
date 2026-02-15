import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ApplicationWindow {
    width: 640
    height: 480
    visible: true
    title: "Issues Viewer"

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 16
        spacing: 16

        RowLayout {
            Layout.fillWidth: true
            spacing: 16

            TextField {
                id: path
                Layout.fillWidth: true
                Layout.preferredHeight: 40

                placeholderText: "Enter the URL of a GitHub repository..."

                onTextChanged: presenter.on_path_changed(text)
                onAccepted: presenter.on_load_issues(text)
            }

            Button {
                text: "Load"

                enabled: presenter.load_enabled

                onClicked: presenter.on_load_issues(path.text)
            }
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true

            border.color: "#AEAEAE"
            radius: 4
            clip: true

            ListView {
                id: list_view
                anchors.fill: parent

                enabled: presenter.content_available

                model: presenter.issues

                delegate: Label {
                    text: modelData
                    font.pixelSize: 16
                    padding: 8

                    width: list_view.width
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
                Layout.preferredHeight: path.implicitHeight

                enabled: presenter.previous_page_available
                // text: "<"
                icon.name: "go-previous"

                onClicked: presenter.on_load_previous_page()
            }

            Label {
                text: presenter.pagination_text
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                Layout.fillWidth: true
            }

            Button {
                Layout.preferredHeight: path.implicitHeight

                enabled: presenter.next_page_available
                // text: ">"
                icon.name: "go-next"

                onClicked: presenter.on_load_next_page()
            }
        }
    }
}
