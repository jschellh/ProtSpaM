#ifndef SCORE_H
#define SCORE_H
#include <vector>
#include <string>
#include <list>
#include <tuple>
#include <iostream>
#include "Sequence.h"

int score(std::vector<std::tuple<unsigned long long,int, int> >& matches, Sequence& seq1, Sequence& seq2, std::string pattern, int& dc, int& threshold);

#endif // SCORE_H
