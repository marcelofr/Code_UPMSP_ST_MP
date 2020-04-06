#include "utils.h"

void MakeTrace(){

    ofstream MyFile(".trace.log", ofstream::app);

    auto t = time(nullptr);
    auto tm = *localtime(&t);

    MyFile << "Inicio: " << put_time(&tm, "%d-%m-%Y %H-%M-%S") << endl;
}

string itos(int i) {
   stringstream s;
   s << i;
   return s.str();
}

string itos(size_t i) {
    stringstream s;
    s << i;
    return s.str();
}

string dtos(double i) {
    stringstream s;
    s << i;
    return s.str();
}

void init(int uf[], int N ) {
    for(int i = 0; i < N; i++)
        uf[i] = -1;
}

void init(vector<int> uf)
{
    for(auto i = uf.begin(); i < uf.end(); i++)
        *i = -1;
}

/* Union of sets containing x and y */
void Union( int uf[],int x, int y ) {
    x = Find(uf, x);
    y = Find(uf, y);

    if(x != y) {
        if(x < y) {
            uf[y] = x;
        }
        else {
            uf[x] = y;
        }
    }
}

/* Find root of set containing x */
int Find( int uf[], int x ) {
    int r = x;

    /* Find root */
    while(uf[r] >= 0)
        r = uf[r];

    /* Compress path to root */
    /*while(uf[x] >= 0) {
        int tmp = uf[x];
        uf[x] = r;
        x = tmp;
    }*/

    return r;

}
