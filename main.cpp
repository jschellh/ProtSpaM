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
#include "Sequence.h"
#include "misc.h"
#include "calc_matches.h"
#include "parser.h"
#include "rasbimp.hpp"
#include "rasbhari.hpp"
#include "patternset.hpp"
#include "parameters.h"
#include "sw_parser.h"

using namespace std;

int main(int argc, char **argv)
{
    double start = omp_get_wtime();
    if(argc < 2)
	{
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

    /* patternsets */
    vector<vector<char> > patterns;
    if (!loadPatterns.empty()) {
        patterns = parsePatterns(loadPatterns);
    }
    else {
        rasbhari rasb_set = rasb_implement::hillclimb_oc(patternNumber, weight, dc, dc);
        patternset PatSet = rasb_set.pattern_set();
        for (unsigned int i = 0; i < PatSet.size(); ++i)
        {
            string pattern = PatSet[i].to_string();
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

    /* Parsing and calculating spacedWords */
    vector<Sequence> sequences;
    if (inFiles.empty() && !input_filename.empty() )
    {
        cout << " --------------\nDetected Multifasta!\n";
        parser(input_filename, sequences);
    }
    else if (!inFiles.empty() )
    {
        cout << " --------------\nDetected input folder!\n";
        sw_parser(inFiles, sequences, patterns);
    }
    /* ----------------------------------- */

    /* calculating spaced-words */
    cout << "Calculating spaced-words...\n";
    double start_sw = omp_get_wtime();
    if (inFiles.empty() && !input_filename.empty() )
    {
        #pragma omp parallel for
        for (unsigned int i = 0; i < sequences.size(); ++i)
        {
            for (unsigned int pat = 0; pat < patterns.size(); ++pat)
            {
                vector<Word> sw;
                spacedWords(sequences[i], patterns[pat], sw);
            }
        }
    }
    else if (!inFiles.empty() )
    {
        #pragma omp parallel for
        for (unsigned int i = 0; i < sequences.size(); ++i)
        {
            for (unsigned int pat = 0; pat < patterns.size(); ++pat)
            {
                spacedWords(sequences[i], patterns[pat]);
            }
        }
    }

    time_elapsed(start_sw);
    /* ------------------------ */

    /* calculating matches */
    cout << " --------------\nCalculating matches...\n";
    double start_matches = omp_get_wtime();
    int length = sequences.size();
    double distance[length][length];
    vector<int> result;
    vector<vector<int> > mismatches_dc;
    for (unsigned int i = 0; i < sequences.size(); ++i)
    {
        distance[i][i] = 0;
	#pragma omp parallel for private (result, mismatches_dc)
        for (unsigned int j = sequences.size() - 1; j > i; --j)
        {
            mismatches_dc.clear();
            for (unsigned int pat = 0; pat < patterns.size(); ++pat)
            {
                result = calc_matches(sequences[i].sorted_words[pat], sequences[j].sorted_words[pat], sequences[i].seq, sequences[j].seq, weight, dc, threshold, patterns[pat]);
                mismatches_dc.push_back(result);
            }
            int mismatch_sum = 0;
            int dc_sum = 0;
            for (unsigned int k = 0; k < mismatches_dc.size(); ++k)
            {
                mismatch_sum += mismatches_dc[k][0];
                dc_sum += mismatches_dc[k][1];
            }
            double mismatch_rate = (double) mismatch_sum / dc_sum;
            distance[i][j] = calc_distance(mismatch_rate);
            if (mismatch_rate > 0.8541)
            {
                tooDistant = true;
            }
            distance[j][i] = distance[i][j];
        }
    }
    time_elapsed(start_matches);
    /* ------------------------ */

    /* Output der Distanzmatrix */
    ofstream output_distance;
    output_distance.open(output_filename);
    output_distance << '\t' << sequences.size() << endl;
    for (unsigned int i = 0; i < sequences.size(); ++i)
    {
        if (sequences[i].header.size() == 10)
        {
            output_distance << sequences[i].header;
        }
        else if (sequences[i].header.size() > 10)
        {
            output_distance << sequences[i].header.substr(0,9);
        }
        else
        {
            output_distance << sequences[i].header;
            for (unsigned int n = 0; n < 10 - sequences[i].header.size(); ++n)
            {
                output_distance << " ";

            }
        }
        for (unsigned int j = 0; j < sequences.size(); ++j)
        {
            if (distance[i][j] == 0)
            {
                output_distance << "0.000000" << "  " ;
            }
            else if (std::isnan(distance[i][j]) != 0)
            {
                output_distance << "10.000000"  << "  " ;
            }
            else
            {
                output_distance << distance[i][j] << "  " ;
            }
        }
        output_distance << endl;
    }
    output_distance.close();
    if (tooDistant)
    {
        cout << "\n\t>>>>>>>>>>>>>>>>>>>> Warning <<<<<<<<<<<<<<<<<<<<\n" <<
        "\tThe distance between at least two organisms is too big resulting in a 'nan' value!\n" <<
        "\tA dummy value (10.0) has been inserted instead for those pairwise distances!\n" <<
        "\tThus the resulting distance matrix is not to be considered reliable!\n" <<
        "\tPossible solutions to resolve this issue:\n" <<
        "\t\t- decrease the weight of the patterns (-w option; default = 6)\n" <<
        "\t\t- lower the threshold to include more matches(-s option; default = 0)\n" <<
        "\t\t- increase the amount of input data for these species (whole-proteomes are advised!)\n"<<
        "\t>>>>>>>>>>>>>>>>>>>> Warning <<<<<<<<<<<<<<<<<<<<\n";
    }
    /* ----------------------- */

    cout << " --------------\nTotal run-time:\n";
    time_elapsed(start);
    return 0;
}
