#ifndef CALC_MATCHES_H
#define CALC_MATCHES_H
#include "Word.h"
#include "Species.h"
#include <vector>
#include <string>
#include <iostream>

double calc_matches(Sequence& s1, Sequence& s2, int& weight, int& dc, int& threshold, std::vector<std::vector<char> >& patterns);

#endif // CALC_MATCHES_H
