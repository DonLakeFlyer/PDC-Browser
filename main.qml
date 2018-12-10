import QtQuick 2.11
import QtQuick.Window 2.11
import QtQuick.Controls 1.2
import QtQuick.Layouts  1.2

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    property var    _packModel:             pdcDB.packModel
    property var    _dogModel:              pdcDB.dogModel
    property var    _photoModel:            pdcDB.photoModel
    property string _dogModelPackFilter:    ""
    property string _photoModelPackFilter:  ""
    property string _photoModelDogFilter:   ""

    function clearAllFilters() {
        _dogModelPackFilter = ""
        _photoModelPackFilter  = ""
        _photoModelDogFilter = ""
        _dogModel.clearFilter()
        _photoModel.clearFilter()
    }

    StackView {
        id:             stack
        anchors.fill:   parent
        initialItem:    packComponent
    }

    Component {
        id: packComponent

        ColumnLayout {
            Layout.fillWidth:   true
            Layout.fillHeight:  true

            RowLayout {
                Label { text: "Packs > " }

                Label {
                    text: "All Dogs"

                    MouseArea {
                        anchors.fill:   parent
                        onClicked:      stack.push(dogComponent)
                    }
                }
            }

            TableView {
                model:              _packModel
                Layout.fillWidth:   true
                Layout.fillHeight:  true

                onClicked: {
                    _dogModelPackFilter = _packModel.getData(row, 0)
                    _photoModelPackFilter = _dogModelPackFilter
                    _dogModel.filter(_dogModelPackFilter)
                    _photoModel.filterPack(_photoModelPackFilter)
                    stack.push(dogComponent)
                }

                TableViewColumn {
                    role: "name"
                    title: "Name"
                    width: 100
                }

                TableViewColumn {
                    role: "dogcount"
                    title: "Size"
                    width: 100
                }
            }
        }
    }

    Component {
        id: dogComponent

        ColumnLayout {
            Layout.fillWidth:   true
            Layout.fillHeight:  true

            RowLayout {
                Label {
                    text: "Packs "

                    MouseArea {
                        anchors.fill:   parent
                        onClicked: {
                            clearAllFilters()
                            stack.pop()
                        }
                    }
                }

                Label {
                    text: "> " + (_dogModelPackFilter == "" ? "Dogs" : _dogModelPackFilter) + " "
                }

                Label {
                    text: "> All Photos"

                    MouseArea {
                        anchors.fill:   parent
                        onClicked:      stack.push(photoComponent)
                    }
                }
            }

            TableView {
                model:              _dogModel
                Layout.fillWidth:   true
                Layout.fillHeight:  true

                onClicked: {
                    _photoModelDogFilter = _dogModel.getData(row, 0)
                    _photoModel.filterDog(_photoModelDogFilter)
                    stack.push(photoComponent)
                }

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
    }

    Component {
        id: photoComponent

        ColumnLayout {
            Layout.fillWidth:   true
            Layout.fillHeight:  true

            RowLayout {
                Label {
                    text: "Packs "

                    MouseArea {
                        anchors.fill:   parent
                        onClicked: {
                            clearAllFilters()
                            stack.pop()
                            stack.pop()
                        }
                    }
                }

                Label {
                    text: "> " + (_photoModelPackFilter === "" ? "Dogs" : _photoModelPackFilter)

                    MouseArea {
                        anchors.fill:   parent
                        onClicked: {
                            _photoModelDogFilter = ""
                            if (_photoModelPackFilter === "") {
                                _photoModel.clearFilter()
                            } else {
                                _photoModel.filterPack(_photoModelPackFilter)
                            }
                            stack.pop()
                        }
                    }
                }

                Label {
                    text: "> " + (_photoModelDogFilter === "" ? "Photos" : _photoModelDogFilter)
                }
            }


            TableView {
                model:              _photoModel
                Layout.fillWidth:   true
                Layout.fillHeight:  true

                rowDelegate: Rectangle{
                    color: "white"
                    height: 100
                }

                TableViewColumn {
                    role: "id"
                    title: "Photo"
                    width: 100

                    delegate: Image {
                        fillMode:   Image.PreserveAspectFit
                        source:     styleData.value ? "image://Photos/" + styleData.value : ""
                    }
                }

                TableViewColumn {
                    role: "dog"
                    title: "Dog"
                    width: 100
                }
            }
        }
    }
}
