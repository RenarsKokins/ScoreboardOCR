QT       += core gui multimedia widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

win32 {
    # more correct variant, how set includepath and libs for mingw
    # add system variable: OPENCV_SDK_DIR=D:/opencv/opencv-build/install
    # read http://doc.qt.io/qt-5/qmake-variable-reference.html#libs
    RC_ICONS += graphics/logo.ico

    INCLUDEPATH += $$(OPENCV_SDK_DIR)/include

    LIBS += -L$$(OPENCV_SDK_DIR)/x64/mingw/lib \
            -lopencv_core460        \
            -lopencv_highgui460     \
            -lopencv_imgcodecs460   \
            -lopencv_imgproc460     \
            -lopencv_videoio460     \
            -lopencv_ml460
}

!win32 {
    INCLUDEPATH += /usr/include/opencv4
    LIBS += `pkg-config --cflags --libs opencv4`
}

SOURCES += \
    src/capturemanager.cpp \
    src/capturescene.cpp \
    src/filtermanager.cpp \
    src/main.cpp \
    src/maincapturescene.cpp \
    src/mainworker.cpp \
    src/outputmanager.cpp \
    src/recognitionmanager.cpp \
    src/scoreboardocr.cpp \
    src/selection.cpp \
    src/selectiondialog.cpp \
    src/selectiontype.cpp \
    src/selectionwidget.cpp \
    src/settingsdialog.cpp \
    src/settingsmanager.cpp

HEADERS += \
    src/capturemanager.h \
    src/capturescene.h \
    src/filtermanager.h \
    src/maincapturescene.h \
    src/mainworker.h \
    src/outputmanager.h \
    src/recognitionmanager.h \
    src/scoreboardocr.h \
    src/selection.h \
    src/selectiondialog.h \
    src/selectiontype.h \
    src/selectionwidget.h \
    src/settingsdialog.h \
    src/settingsmanager.h

FORMS += \
    scoreboardocr.ui \
    selectiondialog.ui \
    selectionwidget.ui \
    settingsdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    graphics/logo.ico \
    graphics/logo.png \
    settings.csv
