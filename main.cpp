#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <cmath>
#include <omp.h>
#include <math.h>
#include "Sequence.h"
#include "misc.h"
#include "calc_matches.h"
#include "parser.h"

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

    vector<vector<char>> patterns = rand_pattern(weight, dc, pattern_number);
    print_patterns(patterns);

    string filename = delete_suffix(argv[1]);
    filename.append(".dm");
    vector<Sequence> sequences;
    parser(argc, argv, sequences);

    #pragma omp parallel for
    for (unsigned int i = 0; i < sequences.size(); ++i)
    {
//        cout << "Spezies " << sequences[i].header << ":\n";
        vector<Word> sw;
        spacedWords(sequences[i], patterns[0], sw, weight);
//        for (unsigned int i = 0; i < sw.size(); ++i)
//        {
//            cout << sw[i].key << " | " << sw[i].pos;
//            cout << " | " << read_word(sw[i].key, weight) << endl;
//        }
//        cout << endl;
    }

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
                result = calc_matches(sequences[i].sorted_words[0], sequences[j].sorted_words[0], sequences[i].seq, sequences[j].seq, weight, dc, threshold, patterns[pat]);
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
            else if (isnan( (double) distance[i][j]) != 0)
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

    double finished = omp_get_wtime();
    double exec_time = finished - start;

    int hours = exec_time / 3600;
    int minutes = (exec_time - (3600 * hours) ) / 60;
    int seconds = exec_time - (3600 * hours) - (60 * minutes);

    if (hours != 0)
    {
        cout << "Time elapsed: " << hours << "h" << minutes << "m" << seconds << "s\n";
    }
    else if (minutes != 0)
    {
        cout << "Time elapsed: " << minutes << "m" << seconds << "s\n";
    }
    else
    {
        cout << "Time elapsed: " << exec_time << "s\n";
    }
    //cin.get();
    return 0;
}
