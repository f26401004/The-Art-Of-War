#-------------------------------------------------
#
# Project created by QtCreator 2017-04-16T16:58:07
#
#-------------------------------------------------

QT       += core gui
QT       += multimedia
QT       += quick qml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Basis_1
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    gamecontroller.cpp \
    scenetitle.cpp \
    scenechoose.cpp \
    flashcommand.cpp \
    windowbase.cpp \
    gameminon.cpp \
    scenebattle.cpp \
    bar.cpp \
    spritebattler.cpp \
    timebar.cpp \
    pibar.cpp \
    enemyai.cpp \
    sceneresult.cpp \
    resultcontent.cpp \
    particle.cpp \
    emitter.cpp \
    graphicstransition.cpp \
    scenecard.cpp \
    scenedraw.cpp

HEADERS  += mainwindow.h \
    gamecontroller.h \
    scenetitle.h \
    scenechoose.h \
    flashcommand.h \
    windowbase.h \
    gameminon.h \
    scenebattle.h \
    bar.h \
    spritebattler.h \
    timebar.h \
    pibar.h \
    enemyai.h \
    sceneresult.h \
    resultcontent.h \
    particle.h \
    emitter.h \
    graphicstransition.h \
    scenecard.h \
    scenedraw.h

FORMS    += mainwindow.ui

RESOURCES += \
    resource.qrc \
    audio_resource_1.qrc \
    audio_resource_2.qrc \
    audio_resource_3.qrc \
    audio_resource_4.qrc \
    audio_resource_5.qrc \
    resource_2.qrc \
    resource_3.qrc
