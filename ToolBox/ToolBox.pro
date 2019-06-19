#-------------------------------------------------
#
# Project created by QtCreator 2017-12-30T20:38:56
#
#-------------------------------------------------

QT       += core gui
QT       += widgets printsupport
QT       += multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
     QT += serialport


TARGET = ToolBox
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    CRC/JQChecksum.cpp \
    Hex2Txt/hex2txt.cpp \
    patcket/packet.cpp \
    SerialAssit/serialassit.cpp \
    Txt2Hex/txt2hex.cpp \
    main.cpp \
    mainwindow.cpp \
    SerialAssit/qcustomplot.cpp \
    SerialAssit/serialportthread.cpp \
    about/about.cpp \
    voice/voice.cpp \
    voice/voice_setting.cpp \
    SerialAssit/audiodevice.cpp

HEADERS += \
    CRC/JQChecksum.h \
    Hex2Txt/hex2txt.h \
    patcket/packet.h \
    SerialAssit/serialassit.h \
    Txt2Hex/txt2hex.h \
    mainwindow.h \
    SerialAssit/qcustomplot.h \
    SerialAssit/serialportthread.h \
    about/about.h \
    voice/sbc_lib.h \
    voice/msbc_lib.h \
    voice/voice.h \
    voice/voice_setting.h \
    voice/adpcm_lib.h \
    SerialAssit/audiodevice.h

FORMS += \
    about/about.ui

RESOURCES += \
    src.qrc

RC_FILE = toolbox.rc


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/voice/ -lsbc_lib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/voice/ -lsbc_libd

INCLUDEPATH += $$PWD/voice
DEPENDPATH += $$PWD/voice

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/voice/libsbc_lib.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/voice/libsbc_libd.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/voice/sbc_lib.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/voice/sbc_libd.lib

DISTFILES += \
    voice/libsbc_lib.a \
    voice/libmsbc_lib.a \
    voice/libadpcm_lib.a

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/voice/ -lmsbc_lib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/voice/ -lmsbc_libd

INCLUDEPATH += $$PWD/voice
DEPENDPATH += $$PWD/voice

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/voice/libmsbc_lib.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/voice/libmsbc_libd.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/voice/msbc_lib.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/voice/msbc_libd.lib

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/voice/ -ladpcm_lib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/voice/ -ladpcm_libd

INCLUDEPATH += $$PWD/voice
DEPENDPATH += $$PWD/voice

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/voice/libadpcm_lib.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/voice/libadpcm_libd.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/voice/adpcm_lib.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/voice/adpcm_libd.lib
