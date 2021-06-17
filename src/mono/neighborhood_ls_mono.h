#ifndef NEIGHBORHOOD_LS_MONO_H
#define NEIGHBORHOOD_LS_MONO_H

#include "mono/basic_ls_mono.h"
#include "common/set_solution.h"

bool SwapInsideLSMono_FI(MonoSolution *my_solution);
bool SwapInsideLSMono_BI(MonoSolution *my_solution);
bool SwapOutsideLSMono_FI(MonoSolution *my_solution);
bool SwapOutsideLSMono_BI(MonoSolution *my_solution);
bool InsertInsideLSMono_FI(MonoSolution *my_solution);
bool InsertInsideLSMono_BI(MonoSolution *my_solution);
bool InsertOutsideLSMono_FI(MonoSolution *my_solution);
bool InsertOutsideLSMono_BI(MonoSolution *my_solution);
bool ChangeOpModeLSMono_FI(MonoSolution *my_solution);
bool ChangeOpModeLSMono_BI(MonoSolution *my_solution);

#endif // NEIGHBORHOOD_LS_MONO_H
