#include(gtest_dependency.pri)

TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG += thread
CONFIG -= qt

HEADERS += \
        ../../src/common/instance.h \
        ../../src/common/solution.h \
        ../../src/common/utils.h \
        test_common.h

SOURCES += \
        ../../src/common/instance.cpp \
        ../../src/common/solution.cpp \
        ../../src/common/utils.cpp \
        main.cpp \
        test_common.cpp

unix|win32: LIBS += -lgtest

unix|win32: LIBS += -lgtest_main
