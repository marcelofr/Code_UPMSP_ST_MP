#ifndef FILE_SOLUTION_H
#define FILE_SOLUTION_H

/*#include "hypervolume.h"
#include <string>
#include <iostream>*/
#include <filesystem>
#include <vector>
#include <fstream>      // std::fstream
#include "experiments.h"

using namespace std;


void FindFilesInFolder(string folder_solution, vector<string> &files);
void ReadFile(algorithm_data &alg_data);

#endif // FILE_SOLUTION_H
