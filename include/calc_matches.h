#ifndef CALC_MATCHES_H
#define CALC_MATCHES_H
#include "Word.h"
#include "Species.h"
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <map>

double calc_matches(const Species& s1, const Species& s2, const int& weight, const int& dc, const int& threshold, const std::vector<std::vector<char> >& patterns, const bool& outputScores);

#endif // CALC_MATCHES_H
