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

using namespace std;

int main(int argc, char **argv)
{
    double start = omp_get_wtime();

    int weight = atoi(argv[2]);
    if (weight > 12)
    {
        cerr << "weight > 12 not supported; you entered a weight of: " << weight << endl;
        return 1;
    }

    int dc = atoi(argv[3]);
    int threshold = atoi(argv[4]);
    int pattern_number;

    if (argc == 5)
    {
        pattern_number = 1;
    }
    else if (argc == 6)
    {
        pattern_number = atoi(argv[5]);
    }

    string filename = delete_suffix(argv[1]);
    filename.append(".dm");
    vector<Sequence> sequences;
    parser(argc, argv, sequences);


    /* creating patternsets */

    rasbhari rasb_set = rasb_implement::hillclimb_oc(pattern_number, weight, dc, dc);
    patternset PatSet = rasb_set.pattern_set();
    vector<vector<char> > patterns;
    for (unsigned int i = 0; i < PatSet.size(); ++i)
    {
        string pattern = PatSet[i].to_string();
        vector<char> tmp(pattern.begin(), pattern.end());
        patterns.push_back(tmp);
    }
    print_patterns(patterns);
    /* -------------------- */

    /* calculating spaced-words */
    cout << " --------------\nCalculating spaced-words...\n";
    double start_sw = omp_get_wtime();
    #pragma omp parallel for
    for (unsigned int i = 0; i < sequences.size(); ++i)
    {
//        cout << "Spezies " << sequences[i].header << ":\n";
        for (unsigned int pat = 0; pat < patterns.size(); ++pat)
        {
            vector<Word> sw;
            spacedWords(sequences[i], patterns[pat], sw, weight);
    //        for (unsigned int i = 0; i < sw.size(); ++i)
    //        {
    //            cout << sw[i].key << " | " << sw[i].pos;
    //            cout << " | " << read_word(sw[i].key, weight) << endl;
    //        }
    //        cout << endl;

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
//		cout << "checking " << i << ". sequence with \n";
        distance[i][i] = 0;
	#pragma omp parallel for private (result, mismatches_dc)
        for (unsigned int j = sequences.size() - 1; j > i; --j)
        {
//			cout << j << ". sequence (" << omp_get_thread_num() << ")" << endl;
            mismatches_dc.clear();
//            result.clear();
            for (unsigned int pat = 0; pat < patterns.size(); ++pat)
            {
//                cout << "Pattern: ";
//                print_pattern(patterns[pat]);
//                cout << endl;
                result = calc_matches(sequences[i].sorted_words[pat], sequences[j].sorted_words[pat], sequences[i].seq, sequences[j].seq, weight, dc, threshold, patterns[pat]);
                mismatches_dc.push_back(result);
            }
            int mismatch_sum = 0;
            int dc_sum = 0;
            for (unsigned int k = 0; k < mismatches_dc.size(); ++k)
            {
//                cout << "mismatches_dc[k][0] = " << mismatches_dc[k][0] << endl;
                mismatch_sum += mismatches_dc[k][0];
//                cout << "mismatches_dc[k][1] = " << mismatches_dc[k][1] << endl;
                dc_sum += mismatches_dc[k][1];
//                cout << "mismatch_sum = " << mismatch_sum << " | dc_sum = " << dc_sum << endl;
            }
            double mismatch_rate = (double) mismatch_sum / dc_sum;
//            cout << "mismatch_rate = " << mismatch_rate << endl;
            distance[i][j] = calc_distance(mismatch_rate);
            distance[j][i] = distance[i][j];
        }
    }
    time_elapsed(start_matches);
    /* ------------------------ */

    /* Output der Distanzmatrix */
    ofstream output_distance;
    output_distance.open(filename);
    output_distance << '\t' << sequences.size() << endl;
    for (unsigned int i = 0; i < sequences.size(); ++i)
    {
        output_distance << sequences[i].header;
        for (unsigned int j = 0; j < sequences.size(); ++j)
        {
            if (distance[i][j] == 0)
            {
                output_distance << "0.000000" << "  " ;
            }
            else if (std::isnan(distance[i][j]) != 0)
            {
                output_distance << "5.000000"  << "  " ;
            }
            else
            {
                output_distance << distance[i][j] << "  " ;
            }
        }
        output_distance << endl;
    }
    output_distance.close();
    /* ----------------------- */

    cout << " --------------\nTotal run-time:\n";
    time_elapsed(start);
    //cin.get();
    return 0;
}
