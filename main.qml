import QtQuick 2.11
import QtQuick.Window 2.11
import QtQuick.Controls 1.2
import QtQuick.Layouts  1.2

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    Row {
        Column {
            Label { text: "Packs" }

            TableView {
                id:     packTable
                model:  pdcDB.packModel

                TableViewColumn {
                    role: "name"
                    title: "Name"
                    width: 100
                }
            }
        }

        Column {
            Label { text: "Dogs" }

            TableView {
                model:  pdcDB.dogModel

                TableViewColumn {
                    role: "name"
                    title: "Name"
                    width: 100
                }

                TableViewColumn {
                    role: "pack"
                    title: "Pack"
                    width: 100
                }
            }
        }

        Column {
            Label { text: "Photos" }

            TableView {
                model:  pdcDB.photoModel

                rowDelegate: Rectangle{
                    color: "white"
                    height: 100
                }

                TableViewColumn {
                    role: "id"
                    title: "Photo"
                    width: 100

                    delegate: Image {
                        source: "image://Photos/" + styleData.value
                    }
                }

                TableViewColumn {
                    role: "dogname"
                    title: "Dog"
                    width: 100
                }
            }
        }
    }
}
