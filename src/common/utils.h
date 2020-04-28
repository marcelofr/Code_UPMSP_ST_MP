#ifndef UTILS_H
#define UTILS_H

#include <fstream>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <vector>

#define RELEASE
//#define DEBUG
//#define IRACE


#define EPS 0.01
#define F_ZERO 1e-6

using namespace std;

void MakeTrace();
string itos(int i);
string itos(size_t i);
string itos(unsigned i);
string dtos(double i);

void init(int uf[], int N ) ;
void init(vector<int> uf);

/* Find root of set containing x */
int Find( int uf[], int x ) ;

/* Union of sets containing x and y */
void Union( int uf[],int x, int y ) ;

#endif // UTILS_H
