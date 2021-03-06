#-------------------------------------------------
#
# Project created by QtCreator 2018-04-02T15:53:56
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Chip8Emu
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
        src/main.cpp \
        src/mainwindow.cpp \
    src/cpu.cpp \
    src/memory.cpp \
    src/display.cpp \
    src/hexview.cpp \
    src/disasm.cpp \
    src/registers.cpp \
    src/stack.cpp \
    src/keyboard.cpp \
    src/timer.cpp

HEADERS += \
        src/mainwindow.h \
    src/cpu.h \
    src/memory.h \
    src/display.h \
    src/hexview.h \
    src/disasm.h \
    src/registers.h \
    src/stack.h \
    src/config.h \
    src/keyboard.h \
    src/timer.h

DISTFILES +=
