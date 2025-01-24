import QtQuick
import QtQuick.Window
import QtQuick.Controls
import Qt.labs.qmlmodels
import JsonModels
Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Demo 2")

    HorizontalHeaderView{
        id: headerView
        anchors{
            left: parent.left
            right: parent.right
            top: parent.top
        }
        boundsBehavior: Flickable.StopAtBounds
        syncView: tableView
    }

    TableView{
        id: tableView
        anchors{
            left: parent.left
            right: parent.right
            top: headerView.bottom
            bottom: parent.bottom
        }
        clip: true

        boundsBehavior: Flickable.StopAtBounds
        columnWidthProvider: function (column){return tableView.width/model.columnCount();}

        onWidthChanged: forceLayout();

        model: JsonModel{

            JsonModelColumn{
                displayName: "test1"
                key:"col1"

            }

            JsonModelColumn{
                displayName: "test2"
                key:"col2"
            }

            records:[{"col1":"1","col2":"2"},{"col1":"2","col2":"2"}]

        }








        delegate: DelegateChooser{
            role: "delegateType"
            DelegateChoice{ roleValue: "text"; ItemDelegate{contentItem:Text{text: model.display} }}
        }
    }
}
