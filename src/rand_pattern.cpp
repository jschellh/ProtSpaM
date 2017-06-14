#include "rand_pattern.h"
using namespace std;

string rand_pattern(int w, int d){
    int lg = w + d;
    int carepos = 2;                                //zwei care-positionen stehen zu beginn fest
    vector<int> pat(lg, 0);
    pat[0] = 1;                                     //erste und letzte Stelle des Patterns = 1
    pat[lg-1]= 1;
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    default_random_engine generator (seed);
    uniform_int_distribution<int> position(1,lg-2);

    while (carepos < w){
        int pos = position(generator);
            if (pat[pos] == 0){
            pat[pos] = 1;
            carepos++;
            }
    }

    string pattern = "";
    for (unsigned int i = 0; i < pat.size(); i++){
        pattern += to_string(pat[i]);
    }
    return pattern;
}
