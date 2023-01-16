QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++20

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Activation.cpp \
    BackpropTest.cpp \
    CPPN_Neat.cpp \
    ES_Hyperneat.cpp \
    ES_Snake.cpp \
    GeneConnection.cpp \
    GeneNode.cpp \
    Genome.cpp \
    HyperBackprop.cpp \
    Hyperneat.cpp \
    Neat.cpp \
    NeuralNetwork.cpp \
    Node.cpp \
    Species.cpp \
    TestHyperneat.cpp \
    ThreadPool.cpp \
    Utils.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    Activation.h \
    BackpropTest.h \
    CPPN_Neat.h \
    ES_Hyperneat.h \
    ES_Snake.h \
    GeneConnection.h \
    GeneNode.h \
    Genome.h \
    HyperBackprop.h \
    Hyperneat.h \
    Neat.h \
    NeuralNetwork.h \
    Node.h \
    Species.h \
    TestHyperneat.h \
    ThreadPool.h \
    Utils.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
