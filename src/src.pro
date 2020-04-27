TEMPLATE = app
CONFIG += console c++20
CONFIG -= app_bundle
CONFIG -= qt
QMAKE_CXXFLAGS += -std=gnu++1z

SOURCES += \
        common/experiments.cpp \
        common/instance.cpp \
        common/solution.cpp \
        common/utils.cpp \
        genetic_algorithm/ga.cpp \
        genetic_algorithm/nsga_ii.cpp \
        main.cpp \
        metric/hypervolume.cpp \
        metric/metric.cpp

HEADERS += \
    common/Timer.h \
    common/experiments.h \
    common/instance.h \
    common/solution.h \
    common/utils.h \
    genetic_algorithm/ga.h \
    genetic_algorithm/nsga_ii.h \
    metric/hypervolume.h \
    metric/metric.h
