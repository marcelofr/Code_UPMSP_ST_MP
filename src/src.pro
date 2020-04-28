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
        math_model/model.cpp \
        math_model/run_model.cpp \
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
    math_model/model.h \
    math_model/run_model.h \
    metric/hypervolume.h \
    metric/metric.h

# INCLUDEPATH += $$PWD/../gurobi702/linux64/include
INCLUDEPATH += /home/marcelo/gurobi702/linux64/include
#INCLUDEPATH += /home/marcone/gurobi/gurobi900/linux64/include

# LIBS += -L$$PWD/../gurobi702/linux64/lib/ -lgurobi_g++5.2 -lgurobi70
LIBS += -L /home/marcelo/gurobi702/linux64/lib/ -lgurobi_g++5.2 -lgurobi70
# LIBS += -L /home/marcone/gurobi/gurobi900/linux64/lib/ -lgurobi_g++5.2 -lgurobi90
