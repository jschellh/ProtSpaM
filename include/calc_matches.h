#ifndef CALC_MATCHES_H
#define CALC_MATCHES_H
#include "Word.h"
#include <vector>
#include <string>
#include <iostream>

double calc_matches(std::vector<Word>& sw1, std::vector<Word>& sw2, std::vector<char>& seq1, std::vector<char>& seq2, int& weight, int& dc, int& threshold, std::vector<char> pattern);

#endif // CALC_MATCHES_H
