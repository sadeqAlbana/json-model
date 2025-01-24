import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import Qt.labs.qmlmodels 1.0
import JsonModels

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")

    HorizontalHeaderView {
        id: headerView
        anchors {
            left: parent.left
            right: parent.right
            top: parent.top
        }
        boundsBehavior: Flickable.StopAtBounds
        syncView: tableView
    }

    TableView {
        id: tableView
        anchors {
            left: parent.left
            right: parent.right
            top: headerView.bottom
            bottom: parent.bottom
        }
        clip: true

        boundsBehavior: Flickable.StopAtBounds
        columnWidthProvider: function (column) {
            return tableView.width / model.columnCount()
        }

        onWidthChanged: forceLayout()

        model: JsonModel {
            JsonModelColumn {
                displayName: "Column 1"
                key: "col1"
                type: "text" // this is optional, only use it with the delegateType


            }

            JsonModelColumn {
                displayName: "Column 2"
                key: "col2"
                type: "text" // this is optional, only use it with the delegateType
            }

            records: [{
                    "col1": "1",
                    "col2": "2"
                }, {
                    "col1": "2",
                    "col2": "2"
                }]
        }

        delegate: DelegateChooser {
            role: "delegateType" //this role is automatically added to each json model
            DelegateChoice {
                roleValue: "text"
                ItemDelegate {
                    contentItem: Text {
                        text: model.display
                    }
                }
            }
        }
    }
}
