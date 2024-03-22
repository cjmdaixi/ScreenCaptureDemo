import QtQuick
import WindowCapturer
import QtQuick.Controls
import QtQuick.Layouts

Window {
    id: window
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")

    WindowCapturer{
        id: windowCapturer
        window: window
        capturedWidth: screenCapturedWidth.value
        capturedHeight: screenCapturedHeight.value
    }

    ColumnLayout{
        anchors.fill: parent
        Item{Layout.fillHeight: true}
        RowLayout{
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignHCenter
            Label{
                text: qsTr("Width:")
            }
            SpinBox{
                id: screenCapturedWidth
                from: 1
                to: Screen.width * Screen.pixelDensity
                value: 2560
                editable: true
            }
        }
        RowLayout{
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignHCenter
            Label{
                text: qsTr("Height:")
            }
            SpinBox{
                id: screenCapturedHeight
                from: 1
                to: Screen.height * Screen.pixelDensity
                value: 1440
                editable: true
            }
        }

        Button{
            Layout.alignment: Qt.AlignHCenter
            text: "Start"
            onClicked: windowCapturer.startCapturing("");
        }
        Button{
            Layout.alignment: Qt.AlignHCenter
            text: "Stop"
            onClicked: windowCapturer.stopCapturing();
        }
        Item{Layout.fillHeight: true}
    }


}
