#ifndef MISC_H
#define MISC_H
#include <vector>
#include <iostream>
#include <string>
#include <chrono>
#include <random>
#include <omp.h>
#include "Species.h"
#include "Word.h"
#include "SummedMatches.h"

        void print_patterns (std::vector<std::vector<char> > patterns);
        void print_pattern (std::vector<char> pattern);
        std::vector<std::vector<char>> parsePatterns (std::string patternSet);
        std::string read_word (unsigned long long spaced_word, unsigned int weight);
        bool isContext(unsigned long long spaced_word, unsigned int weight);
        void spacedWords (Species& sequence, std::vector<char> const& pattern, std::vector<Word>& out);
        void spacedWords(Species&, std::vector<char> const&);
        void output_pairwise (std::vector<SummedMatches>&, std::string, std::string);
        std::string delete_suffix (std::string);
        std::string delete_prefix (std::string);
        std::vector<std::vector<char> > rand_pattern (int w, int d, int pat_number);
        void time_elapsed(double start);
        void outputDistanceMatrix (const std::vector<Species>& species, const std::string& fileName,
                                   const std::vector<std::vector<double> >& distanceMatrix, const bool& tooDistant);
#endif // MISC_H
