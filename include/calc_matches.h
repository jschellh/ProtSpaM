#ifndef CALC_MATCHES_H
#define CALC_MATCHES_H
#include "Word.h"
#include "Match.h"
#include "Species.h"
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <map>

std::vector<Match> calc_matches(const Species& s1, const Species& s2, const int& weight, const int& dc, const int& threshold, const std::vector<std::vector<char> >& patterns);

#endif // CALC_MATCHES_H
