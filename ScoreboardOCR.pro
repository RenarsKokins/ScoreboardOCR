QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
INCLUDEPATH += /usr/include/opencv4

LIBS += `pkg-config --cflags --libs opencv4`


SOURCES += \
    capturemanager.cpp \
    main.cpp \
    mainworker.cpp \
    scoreboardocr.cpp

HEADERS += \
    capturemanager.h \
    mainworker.h \
    scoreboardocr.h

FORMS += \
    scoreboardocr.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
