/**
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 3 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * http://www.gnu.org/copyleft/gpl.html
 *
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <cmath>
#include <omp.h>
#include "Species.h"
#include "misc.h"
#include "calc_matches.h"
#include "parser.h"
#include "rasbimp.hpp"
#include "rasbhari.hpp"
#include "patternset.hpp"
#include "parameters.h"
#include "sw_parser.h"

using namespace std;

int main(int argc, char **argv) {
    double start = omp_get_wtime();
    if(argc < 2) {
		printHelp();
		exit (EXIT_FAILURE);
	}
    int weight = 6;
    int dc = 40;
    int threshold = 0;
    int patternNumber = 5;
    int threads = omp_get_max_threads();
    string loadPatterns;
    bool savePatterns = false;
    vector<string> inFiles;
    string output_filename = "DMat";
    bool tooDistant = false;
    parseParameters(argc, argv, weight, dc, threshold, patternNumber, threads, inFiles, output_filename, savePatterns, loadPatterns);
    string input_filename(argv[argc-1]);
    printParameters(weight, dc, threshold, patternNumber, threads, output_filename);
    omp_set_num_threads(threads);

    /* pattern sets */
    vector<vector<char> > patterns;
    if (!loadPatterns.empty()) {
        patterns = parsePatterns(loadPatterns);
    }
    else {
        rasbhari rasb_set = rasb_implement::hillclimb_oc(patternNumber, weight, dc, dc);
        patternset PatSet = rasb_set.pattern_set();
        for (const auto &i : PatSet) {
            string pattern = i.to_string();
            vector<char> tmp(pattern.begin(), pattern.end());
            patterns.push_back(tmp);
        }
    }
    print_patterns(patterns);
    // output pattern set
    if (savePatterns) {
        ofstream patOut;
        patOut.open("patterns.txt");
        for (auto &p : patterns) {
            for (auto &c : p) {
                patOut << c;
            }
            if (p != patterns[patterns.size() - 1]) {
                patOut << endl;
            }
        }
        patOut.close();
    }

    /* -------------------- */

    /* parsing and calculating spacedWords */
    vector<Species> species;
    if (inFiles.empty() && !input_filename.empty() ) {
        cout << " --------------\nDetected Multifasta!\n";
        parser(input_filename, species);
    }
    else if (!inFiles.empty() ) {
        cout << " --------------\nDetected input folder!\n";
        sw_parser(inFiles, species, patterns);
    }
    /* ----------------------------------- */

    /* calculating spaced-words */
    cout << "Calculating spaced-words...\n";
    double start_sw = omp_get_wtime();
    if (inFiles.empty() && !input_filename.empty() ) {
        #pragma omp parallel for
        for (unsigned int i = 0; i < species.size(); ++i) {
            for (const auto &pattern : patterns) {
                vector<Word> sw;
                spacedWords(species[i], pattern, sw);
            }
        }
    }
    else if (!inFiles.empty() ) {
        #pragma omp parallel for
        for (unsigned int i = 0; i < species.size(); ++i) {
            for (const auto &pattern : patterns) {
                spacedWords(species[i], pattern);
            }
        }
    }

    time_elapsed(start_sw);
    /* ------------------------ */

    /* calculating matches */
    cout << " --------------\nCalculating matches...\n";
    double start_matches = omp_get_wtime();
    vector<vector<double>> distance(species.size(), vector<double>(species.size()) );
    for (unsigned int i = 0; i < species.size(); ++i) {
        distance[i][i] = 0;
	#pragma omp parallel for
        for (auto j = species.size() - 1; j > i; --j) {
            double mismatch_rate = calc_matches(species[i], species[j], weight, dc, threshold, patterns, outputScores);
            distance[i][j] = calc_distance(mismatch_rate);
            if (mismatch_rate > 0.8541) tooDistant = true;
            distance[j][i] = distance[i][j];
        }
    }
    time_elapsed(start_matches);
    /* ------------------------ */

    outputDistanceMatrix(species, output_filename, distance, tooDistant);
    cout << " --------------\nTotal run-time:\n";
    time_elapsed(start);

    return 0;
}
