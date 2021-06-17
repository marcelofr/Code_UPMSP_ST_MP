TEMPLATE = app
CONFIG += console c++20
CONFIG -= app_bundle
CONFIG -= qt
QMAKE_CXXFLAGS += -std=gnu++1z

SOURCES += \
        common/experiments.cpp \
        common/file_solution.cpp \
        common/generate_intances.cpp \
        common/instance.cpp \
        common/set_solution.cpp \
        common/solution.cpp \
        common/utils.cpp \
        genetic_algorithm/basic_ga.cpp \
        genetic_algorithm/nsga_ii.cpp \
        local_search/basic_ls_multi.cpp \
        local_search/main_ls_multi.cpp \
        local_search/neighborhood_ls_multi.cpp \
        main.cpp \
        math_model/model.cpp \
        math_model/run_model.cpp \
        metric/hypervolume.cpp \
        metric/metric.cpp \
        mono/basic_ls_mono.cpp \
        mono/main_ls_mono.cpp \
        mono/neighborhood_ls_mono.cpp
HEADERS += \
    common/Timer.h \
    common/algorithm_data.h \
    common/experiments.h \
    common/file_solution.h \
    common/generate_intances.h \
    common/instance.h \
    common/set_solution.h \
    common/solution.h \
    common/utils.h \
    genetic_algorithm/basic_ga.h \
    genetic_algorithm/gasolution.h \
    genetic_algorithm/nsga_ii.h \
    local_search/basic_ls_multi.h \
    local_search/lssolution.h \
    local_search/main_ls_multi.h \
    local_search/neighborhood_ls_multi.h \
    math_model/model.h \
    math_model/run_model.h \
    metric/hypervolume.h \
    metric/metric.h \
    mono/basic_ls_mono.h \
    mono/main_ls_mono.h \
    mono/monosolution.h \
    mono/neighborhood_ls_mono.h

# INCLUDEPATH += $$PWD/../gurobi702/linux64/include
# INCLUDEPATH += /opt/gurobi752/linux64/include
INCLUDEPATH += /home/marcelo/gurobi702/linux64/include
# INCLUDEPATH += /home/marcone/gurobi/gurobi900/linux64/include

# LIBS += -L /opt/gurobi752/linux64/lib/ -lgurobi_g++5.2 -lgurobi75
# LIBS += -L$$PWD/../gurobi702/linux64/lib/ -lgurobi_g++5.2 -lgurobi70
LIBS += -L /home/marcelo/gurobi702/linux64/lib/ -lgurobi_g++5.2 -lgurobi70
# LIBS += -L /home/marcone/gurobi/gurobi900/linux64/lib/ -lgurobi_g++5.2 -lgurobi90
