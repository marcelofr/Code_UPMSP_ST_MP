#include "set_solution.h"


void SortByMakespan(vector<Solution*> &set_solution)
{
    sort(set_solution.begin(), set_solution.end(), CompareMakespan);
}

void SortByTEC(vector<Solution*> &set_solution)
{
    sort(set_solution.begin(), set_solution.end(), CompareTEC);
}

/*bool CompareMakespanGA(GASolution * l, GASolution * r) //(2)
{
    return CompareMakespan((Solution*)l, (Solution*)r);
}

bool CompareTECGA(GASolution * l, GASolution * r) //(2)
{
    return CompareTEC((Solution*)l, (Solution*)r);
}*/

void SortByMakespan(vector<GASolution *> &set_solution)
{
    sort(set_solution.begin(), set_solution.end(), CompareMakespan);
}

void SortByTEC(vector<GASolution*> &set_solution)
{
    sort(set_solution.begin(), set_solution.end(), CompareTEC);
}
