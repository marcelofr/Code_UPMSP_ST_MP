TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        common/experiments.cpp \
        common/instance.cpp \
        common/solution.cpp \
        common/utils.cpp \
        genetic_algorithm/ga.cpp \
        genetic_algorithm/nsga_ii.cpp \
        main.cpp

HEADERS += \
    common/Timer.h \
    common/experiments.h \
    common/instance.h \
    common/solution.h \
    common/utils.h \
    genetic_algorithm/ga.h \
    genetic_algorithm/nsga_ii.h
