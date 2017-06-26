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
using namespace std;

vector<Sequence> parser (int argc, char **argv, vector<Sequence>& out)
{
    if (argc <= 1)
    {
        cerr << "Usage: " << argv[0] << " [infile]\n";
    }

    ifstream input(argv[1]);
    if (!input.good() )
    {
        cerr << "Error opening '" << argv[1] << "'. Bailing out.\n";
    }
    Sequence tmp;
    string line;
    while (getline(input,line).good() )
    {
        if (line.empty() || line[0] == '>')
        {
            if (!tmp.header.empty() )
            {
                out.push_back(tmp);
                tmp.clear_header();
                tmp.clear_seq();
            }
            if (!line.empty() )
            {
                if (line.substr(1).size() > 10)
                {
                    string name = line.substr(1).substr(0,9);
                    tmp.set_header(name);
                }
                else if (line.substr(1).size() < 10)
                {
                    string name = line.substr(1).substr(0);
                    while (name.size() < 10)
                    {
                        name.append(" ");
                    }
                    tmp.set_header(name);
                }
                else
                {
                    tmp.set_header(line.substr(1) );
                }
            }
            tmp.clear_seq();
        }
        else if (!tmp.header.empty() )
        {

            for (unsigned int i = 0; i < line.size(); ++i)
            {
                switch (line[i])
                {
                    case 'A': tmp.set_seq(0); break;
                    case 'R': tmp.set_seq(1); break;
                    case 'N': tmp.set_seq(2); break;
                    case 'D': tmp.set_seq(3); break;
                    case 'C': tmp.set_seq(4); break;
                    case 'Q': tmp.set_seq(5); break;
                    case 'E': tmp.set_seq(6); break;
                    case 'G': tmp.set_seq(7); break;
                    case 'H': tmp.set_seq(8); break;
                    case 'I': tmp.set_seq(9); break;
                    case 'L': tmp.set_seq(10); break;
                    case 'K': tmp.set_seq(11); break;
                    case 'M': tmp.set_seq(12); break;
                    case 'F': tmp.set_seq(13); break;
                    case 'P': tmp.set_seq(14); break;
                    case 'S': tmp.set_seq(15); break;
                    case 'T': tmp.set_seq(16); break;
                    case 'W': tmp.set_seq(17); break;
                    case 'Y': tmp.set_seq(18); break;
                    case 'V': tmp.set_seq(19); break;
                    case 'B': tmp.set_seq(20); break;
                    case 'Z': tmp.set_seq(21); break;
                    case 'X': tmp.set_seq(22); break;
                    case '*': tmp.set_seq(23); break;
                }
            }
        }
    }
    if (!tmp.header.empty() )
    {
        out.push_back(tmp);
    }
    return out;
}

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
    if (argc == 6)
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

    for (unsigned int i = 0; i < sequences.size(); ++i)
    {
//		cout << "checking " << i << ". sequence with \n";
        distance[i][i] = 0;
		#pragma omp parallel for
        for (unsigned int j = sequences.size() - 1; j > i; --j)
        {
//			cout << j << ". sequence (" << omp_get_thread_num() << ")\n" << endl;
            double mismatch_rate = calc_matches(sequences[i].sorted_words[0], sequences[j].sorted_words[0], sequences[i].seq, sequences[j].seq, weight, dc, threshold, patterns[0]);
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
            else if (isnan(distance[i][j]) != 0)
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
