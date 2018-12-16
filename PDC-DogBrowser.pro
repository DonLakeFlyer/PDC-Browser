QT += quick sql printsupport

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Refer to the documentation for the
# deprecated API to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

HEADERS += \
    PDCDatabase.h \
    ModelBase.h \
    PackModel.h \
    DogModel.h \
    PhotoModel.h \
    Print.h \

SOURCES += \
    main.cpp \
    PDCDatabase.cc \
    ModelBase.cc \
    PackModel.cc \
    DogModel.cc \
    PhotoModel.cc \
    Print.cc \

RESOURCES += qml.qrc

CONFIG(debug, debug|release) {
    message(Debug flavor)
    CONFIG += DebugBuild
} else:CONFIG(release, debug|release) {
    message(Release flavor)
    CONFIG += ReleaseBuild
} else {
    error(Unsupported build flavor)
}

DebugBuild {
    DESTDIR  = $${OUT_PWD}/debug
} else {
    DESTDIR  = $${OUT_PWD}/release
}
BASEDIR      = $$IN_PWD


android-g++ {
    my_files.path = /assets
    my_files.files = $$IN_PWD/PDC.db
    INSTALLS = my_files
    DEFINES += __android__
} else : macx {
     QMAKE_POST_LINK += $$QMAKE_COPY $$IN_PWD/PDC.db $$DESTDIR/$${TARGET}.app/Contents/MacOS

    ReleaseBuild {
        # We cd to release directory so we can run macdeployqt without a path to the
        # qgroundcontrol.app file. If you specify a path to the .app file the symbolic
        # links to plugins will not be created correctly.
        QMAKE_POST_LINK += && cd $${DESTDIR} && $$dirname(QMAKE_QMAKE)/macdeployqt $${TARGET}.app -appstore-compliant -verbose=2 -qmldir=$${BASEDIR}

        # macdeployqt is missing some relocations once in a while. "Fix" it:
        #QMAKE_POST_LINK += && python $$BASEDIR/tools/osxrelocator.py $${TARGET}.app/Contents @rpath @executable_path/../Frameworks -r > /dev/null 2>&1

        # Create package
        QMAKE_POST_LINK += && hdiutil create /tmp/tmp.dmg -ov -volname "$${TARGET}-$${MAC_VERSION}" -fs HFS+ -srcfolder "$${DESTDIR}/"
        QMAKE_POST_LINK += && mkdir -p $${DESTDIR}/package
        QMAKE_POST_LINK += && hdiutil convert /tmp/tmp.dmg -format UDBZ -o $${DESTDIR}/package/$${TARGET}.dmg
        QMAKE_POST_LINK += && rm /tmp/tmp.dmg
    }
}

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
