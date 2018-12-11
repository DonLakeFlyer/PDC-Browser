import QtQuick 2.11
import QtQuick.Window 2.11
import QtQuick.Controls 1.2
import QtQuick.Layouts  1.2

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("PDC Dog Browser")

    property var    _packModel:             pdcDB.packModel
    property var    _dogModel:              pdcDB.dogModel
    property var    _photoModel:            pdcDB.photoModel
    property string _dogModelPackFilter:    ""
    property string _photoModelPackFilter:  ""
    property string _photoModelDogFilter:   ""
    property real   _headingPointSize:      30
    property string _dogImageName
    property string _dogImageUrl

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

            Label {
                font.pointSize: _headingPointSize
                text: "Packs"
            }

            GridLayout {
                columns: 3

                Button {
                    text:               "Show All Packs"
                    Layout.fillWidth:   true
                    onClicked:          stack.push(dogComponent)
                }

                Repeater {
                    model: _packModel

                    Button {
                        text:               qsTr("%1 (%2)").arg(name).arg(dogcount)
                        Layout.fillWidth:   true
                        onClicked: {
                            _dogModelPackFilter = name
                            _photoModelPackFilter = _dogModelPackFilter
                            _dogModel.filter(_dogModelPackFilter)
                            _photoModel.filterPack(_photoModelPackFilter)
                            stack.push(dogComponent)
                        }
                    }
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
                    font.pointSize: _headingPointSize
                    text: " < "

                    MouseArea {
                        anchors.fill:   parent
                        onClicked: {
                            clearAllFilters()
                            stack.pop()
                        }
                    }
                }

                Label {
                    font.pointSize: _headingPointSize
                    text: "Dogs"
                }
            }


            GridLayout {
                columns: 3

                Button {
                    text:               "All Dogs"
                    Layout.fillWidth:   true
                    onClicked:          stack.push(photoComponent)
                }

                Repeater {
                    model: _dogModel

                    Button {
                        text:               qsTr("%1 %2 (%3)").arg(name).arg(alpha).arg(pack)
                        Layout.fillWidth:   true
                        onClicked: {
                            _photoModelDogFilter = name
                            _photoModel.filterDog(_photoModelDogFilter)
                            stack.push(photoComponent)
                        }
                    }
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
                    font.pointSize: _headingPointSize
                    text: " < "

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
                    font.pointSize: _headingPointSize
                    text: "Photos"
                }
            }


            TableView {
                model:              _photoModel
                Layout.fillWidth:   true
                Layout.fillHeight:  true

                onClicked: {
                    _dogImageName = _photoModel.getData(row, 2)
                    _dogImageUrl = "image://Photos/" + _photoModel.getData(row, 0)
                    stack.push(dogImageComponent)
                }

                rowDelegate: Rectangle{
                    color: "white"
                    height: 200
                }

                TableViewColumn {
                    role: "id"
                    title: "Photo"
                    width: 200

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

    Component {
        id: dogImageComponent

        ColumnLayout {
            Layout.fillWidth:   true
            Layout.fillHeight:  true

            RowLayout {
                Label {
                    font.pointSize: _headingPointSize
                    text: " < "

                    MouseArea {
                        anchors.fill:   parent
                        onClicked:      stack.pop()
                    }
                }

                Label {
                    font.pointSize: _headingPointSize
                    text:           _dogImageName
                }
            }

            Flickable {
                id:                 flick
                Layout.fillWidth:   true
                Layout.fillHeight:  true
                contentWidth:       width
                contentHeight:      height

                PinchArea {
                    width: Math.max(flick.contentWidth, flick.width)
                    height: Math.max(flick.contentHeight, flick.height)

                    property real initialWidth
                    property real initialHeight

                    onPinchStarted: {
                        initialWidth = flick.contentWidth
                        initialHeight = flick.contentHeight
                    }

                    onPinchUpdated: {
                        // adjust content pos due to drag
                        flick.contentX += pinch.previousCenter.x - pinch.center.x
                        flick.contentY += pinch.previousCenter.y - pinch.center.y

                        // resize content
                        flick.resizeContent(initialWidth * pinch.scale, initialHeight * pinch.scale, pinch.center)
                    }

                    onPinchFinished: {
                        // Move its content within bounds.
                        flick.returnToBounds()
                    }

                    Image {
                        id:         dogImage
                        width:      flick.contentWidth
                        height:     flick.contentHeight
                        fillMode:   Image.PreserveAspectFit
                        source:     _dogImageUrl

                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                flick.contentX = 0
                                flick.contentY = 0
                                flick.resizeContent(flick.width, flick.height, Qt.point(flick.width / 2, flick.height / 2))
                            }
                        }
                    }
                }
            }

        }
    }
 }
