import QtQuick 2.11
import QtQuick.Window 2.11
import QtQuick.Controls 1.4
import QtQuick.Layouts  1.2

Window {
    id: window
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
    property real   _imageTextPointSize:    20
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

            RowLayout {

                Label {
                    font.pointSize: _headingPointSize
                    text:           "Packs"
                }

                Button {
                    text:       "Print"
                    onClicked:  pdcDB.print(onePackPerFile.checked)
                }

                CheckBox {
                    id:         onePackPerFile
                    text:       "One Pack Per File"
                    checked:    true
                }
            }

            GridLayout {
                columns: 3

                Button {
                    text:               "Show All Packs"
                    Layout.fillWidth:   true
                    onClicked:          stack.push(dogComponent)
                }

                Button {
                    text:               "Photo Search"
                    Layout.fillWidth:   true
                    onClicked:          stack.push(searchComponent)
                }

                Repeater {
                    model: _packModel

                    Button {
                        text:               qsTr("%1 (%2)").arg(name).arg(dogCount)
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
                        text:               qsTr("%1 (%2%3-%4) %5").arg(name).arg(sexString).arg(alphaString == "A" ? "-A" : "").arg(pack).arg(collarFreq)
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

            Flickable {
                Layout.fillWidth:   true
                Layout.fillHeight:  true
                contentWidth:       grid.width
                contentHeight:      grid.height

                Grid {
                    id:             grid
                    columns:        4
                    columnSpacing:  0
                    rowSpacing:     0

                    property real imageWidth: width / 4

                    Repeater {
                        model: _photoModel

                        Image {
                            width: window.width / 4
                            height: width
                            fillMode:           Image.PreserveAspectFit
                            source:             "image://Photos/" + id

                            Rectangle {
                                anchors.fill:   nameLabel
                                color:          "black"
                                opacity:        0.25
                            }

                            Label {
                                id:                     nameLabel
                                anchors.margins:        5
                                anchors.topMargin:      (parent.height - parent.paintedHeight) / 2
                                anchors.top:            parent.top
                                anchors.left:           leftLabel ? undefined: parent.left
                                anchors.right:          leftLabel ? parent.right : undefined
                                color:                  "white"
                                font.pointSize:         _imageTextPointSize
                                text:                   dog

                                property bool leftLabel: leftPhoto === 1

                            }

                            MouseArea {
                                anchors.fill: parent

                                onClicked: {
                                    _dogImageName = dog
                                    _dogImageUrl = "image://Photos/" + id
                                    stack.push(dogImageComponent)
                                }
                            }

                        }
                    }
                }
            }
        }
    }

    Component {
        id: dogImageComponent

        Item {
            Flickable {
                id:             flick
                anchors.fill:   parent
                contentWidth:   width
                contentHeight:  height

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
                                flick.resizeContent(flick.width, flick.height, Qt.point(flick.width / 2, flick.height / 2))
                                flick.contentX = 0
                                flick.contentY = 0
                            }
                            onWheel: {
                                if (wheel.angleDelta.y > 0) {
                                    flick.resizeContent(flick.contentWidth * 1.1, flick.contentHeight * 1.1, Qt.point(flick.width / 2, flick.height / 2))
                                } else {
                                    flick.resizeContent(flick.contentWidth * 0.9, flick.contentHeight * 0.9, Qt.point(flick.width / 2, flick.height / 2))
                                }
                                wheel.accepted = true
                            }
                        }
                    }
                }
            }

            Rectangle {
                anchors.fill:   labelRow
                color:          "white"
                opacity:        0.25
            }

            RowLayout {
                id: labelRow

                Label {
                    font.pointSize: _headingPointSize
                    text:           " < "

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
        }
    }

    Component {
        id: searchComponent

        Column {
            Layout.fillWidth:   true
            Layout.fillHeight:  true
            spacing:            10

            ExclusiveGroup { id: sexGroup }

            RowLayout {
                id: labelRow

                Label {
                    font.pointSize: _headingPointSize
                    text:           " < Search"

                    MouseArea {
                        anchors.fill:   parent
                        onClicked:      stack.pop()
                    }
                }
            }

            RowLayout {
                RadioButton {
                    text:           "Female"
                    exclusiveGroup: sexGroup
                    property int sexValue: 1
                }
                RadioButton {
                    text:           "Male"
                    exclusiveGroup: sexGroup
                    property int sexValue: 2
                }
                RadioButton {
                    text:           "Any"
                    checked:        true
                    exclusiveGroup: sexGroup
                    property int sexValue: 0
                }
            }

            CheckBox {
                id:                         collared
                text:                       checkedState === Qt.Checked ?
                                                "HAS Collar" :
                                                (checkedState === Qt.Unchecked ? "HAS NO Collar" : "Collar Doesn't Matter")
                checkedState:               Qt.PartiallyChecked
                partiallyCheckedEnabled:    true
            }

            CheckBox {
                id:                         blackTip
                text:                       checkedState === Qt.Checked ?
                                                "Black In Tail Tip" :
                                                (checkedState === Qt.Unchecked ? "Fully White Tail Tip" : "Tail Tip Doesn't Matter")
                checkedState:               Qt.PartiallyChecked
                partiallyCheckedEnabled:    true
            }

            Label {
                text: "Left Side Markings"
            }

            CheckBox {
                id:                         markingLeftLeg
                text:                       checkedState === Qt.Checked ?
                                                "Front Leg HAS White Marking" :
                                                (checkedState === Qt.Unchecked ? "Front Leg HAS NO White Marking" : "Front Leg Marking Doesn't Matter")
                checkedState:               Qt.PartiallyChecked
                partiallyCheckedEnabled:    true
            }

            CheckBox {
                id:                         markingLeftShoulder
                text:                       checkedState === Qt.Checked ?
                                                "Front Shoulder HAS White Marking" :
                                                (checkedState === Qt.Unchecked ? "Front Shoulder HAS NO White Marking" : "Front Shoulder Marking Doesn't Matter")
                checkedState:               Qt.PartiallyChecked
                partiallyCheckedEnabled:    true
            }

            CheckBox {
                id:                         markingLeftAbdomen
                text:                       checkedState === Qt.Checked ?
                                                "Abdomen HAS White Marking" :
                                                (checkedState === Qt.Unchecked ? "Abdomen HAS NO White Marking" : "Abdomen Marking Doesn't Matter")
                checkedState:               Qt.PartiallyChecked
                partiallyCheckedEnabled:    true
            }

            CheckBox {
                id:                         markingLeftHind
                text:                       checkedState === Qt.Checked ?
                                                "Hind HAS White Marking" :
                                                (checkedState === Qt.Unchecked ? "Hind HAS NO White Marking" : "Hind Marking Doesn't Matter")
                checkedState:               Qt.PartiallyChecked
                partiallyCheckedEnabled:    true
            }

            Label {
                text: "Right Side Markings"
            }

            CheckBox {
                id:                         markingRightLeg
                text:                       checkedState === Qt.Checked ?
                                                "Front Leg HAS White Marking" :
                                                (checkedState === Qt.Unchecked ? "Front Leg HAS NO White Marking" : "Front Leg Marking Doesn't Matter")
                checkedState:               Qt.PartiallyChecked
                partiallyCheckedEnabled:    true
            }

            CheckBox {
                id:                         markingRightShoulder
                text:                       checkedState === Qt.Checked ?
                                                "Front Shoulder HAS White Marking" :
                                                (checkedState === Qt.Unchecked ? "Front Shoulder HAS NO White Marking" : "Front Shoulder Marking Doesn't Matter")
                checkedState:               Qt.PartiallyChecked
                partiallyCheckedEnabled:    true
            }

            CheckBox {
                id:                         markingRightAbdomen
                text:                       checkedState === Qt.Checked ?
                                                "Abdomen HAS White Marking" :
                                                (checkedState === Qt.Unchecked ? "Abdomen HAS NO White Marking" : "Abdomen Marking Doesn't Matter")
                checkedState:               Qt.PartiallyChecked
                partiallyCheckedEnabled:    true
            }

            CheckBox {
                id:                         markingRightHind
                text:                       checkedState === Qt.Checked ?
                                                "Hind HAS White Marking" :
                                                (checkedState === Qt.Unchecked ? "Hind HAS NO White Marking" : "Hind Marking Doesn't Matter")
                checkedState:               Qt.PartiallyChecked
                partiallyCheckedEnabled:    true
            }


            Button {
                text: "Search Photos"
                onClicked: {
                    _photoModel.filterPhoto(collared.checkedState, blackTip.checkedState, sexGroup.current.sexValue,
                                            markingLeftLeg.checkedState, markingLeftShoulder.checkedState, markingLeftAbdomen.checkedState, markingLeftHind.checkedState,
                                            markingRightLeg.checkedState, markingRightShoulder.checkedState, markingRightAbdomen.checkedState, markingRightHind.checkedState)
                    stack.pop()
                    stack.push(photoComponent)
                }
            }
        }
    }
}
