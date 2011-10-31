#-------------------------------------------------
#
# Project created by QtCreator 2011-10-06T11:57:56
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = extractor
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

QMAKE_CXXFLAGS += -mtune=core2 -march=core2 -O3

SOURCES += main.cpp \
    FeatureExtractorInterface.cpp \
    LeftRightProfile.cpp \
    FeatureExtractorFactory.cpp \
    ArchiveExtractor.cpp \
    Projection.cpp \
    Crossing.cpp \
    Zones.cpp

HEADERS += \
    FeatureExtractorInterface.h \
    LeftRightProfile.h \
    FeatureExtractorFactory.h \
    ArchiveExtractor.h \
    Projection.h \
    Crossing.h \
    Zones.h






















