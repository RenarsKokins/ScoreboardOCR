TEMPLATE = app
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

isEmpty(CATCH_INCLUDE_DIR): CATCH_INCLUDE_DIR=$$(CATCH_INCLUDE_DIR)
!isEmpty(CATCH_INCLUDE_DIR): INCLUDEPATH *= $${CATCH_INCLUDE_DIR}

isEmpty(CATCH_INCLUDE_DIR): {
    message("CATCH_INCLUDE_DIR is not set, assuming Catch2 can be found automatically in your system")
}

SOURCES += \
    ../scoreboardocr/src/capturemanager.cpp \
    ../scoreboardocr/src/capturescene.cpp \
    ../scoreboardocr/src/filtermanager.cpp \
    ../scoreboardocr/src/maincapturescene.cpp \
    ../scoreboardocr/src/mainworker.cpp \
    ../scoreboardocr/src/outputmanager.cpp \
    ../scoreboardocr/src/recognitionmanager.cpp \
    ../scoreboardocr/src/selection.cpp \
    ../scoreboardocr/src/selectiondialog.cpp \
    ../scoreboardocr/src/selectiontype.cpp \
    ../scoreboardocr/src/selectionwidget.cpp \
    ../scoreboardocr/src/settingsdialog.cpp \
    ../scoreboardocr/src/settingsmanager.cpp \
    main.cpp \
    tst_scoreboardocr_tests.cpp

HEADERS += \
    ../scoreboardocr/src/capturemanager.h \
    ../scoreboardocr/src/capturescene.h \
    ../scoreboardocr/src/filtermanager.h \
    ../scoreboardocr/src/maincapturescene.h \
    ../scoreboardocr/src/mainworker.h \
    ../scoreboardocr/src/outputmanager.h \
    ../scoreboardocr/src/recognitionmanager.h \
    ../scoreboardocr/src/selection.h \
    ../scoreboardocr/src/selectiondialog.h \
    ../scoreboardocr/src/selectiontype.h \
    ../scoreboardocr/src/selectionwidget.h \
    ../scoreboardocr/src/settingsdialog.h \
    ../scoreboardocr/src/settingsmanager.h

FORMS += \
    ../scoreboardocr/scoreboardocr.ui \
    ../scoreboardocr/selectiondialog.ui \
    ../scoreboardocr/selectionwidget.ui \
    ../scoreboardocr/settingsdialog.ui

