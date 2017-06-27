#ifndef MISC_H
#define MISC_H
#include <vector>
#include <iostream>
#include <string>
#include <chrono>
#include <random>
#include "Sequence.h"
#include "Word.h"

        void print_patterns (std::vector<std::vector<char> > patterns);
        void print_pattern (std::vector<char> pattern);
        std::string read_word (unsigned long long spaced_word, unsigned int weight);
        void spacedWords (Sequence& sequence, std::vector<char> const& pattern, std::vector<Word>& out, int weight);
        double calc_distance (double mmr);
        std::string delete_suffix (std::string file);
        std::vector<std::vector<char> > rand_pattern (int w, int d, int pat_number);

#endif // MISC_H
