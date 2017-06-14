#include <iostream>
#include <fstream>
#include <vector>
#include <time.h>
#include <algorithm>
#include <tuple>
#include <string>
#include <cmath>
#include "Sequence.h"
#include "score.h"
#include "rand_pattern.h"
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
                tmp.set_header(line.substr(1) );
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

void spacedWords (Sequence& sequence, string const& pattern, vector<Word>& out)
{
    for (unsigned int i = 0; i < (sequence.seq.size() - pattern.length() + 1); ++i)
    {
        Word tmp;
        unsigned long long spacedWord = 0;
        for (unsigned int j = 0; j < pattern.size(); ++j)
        {
            if (pattern[j] == '1')
            {
                spacedWord <<= 5;
                spacedWord |= sequence.seq[i];
            }
            ++i;
        }
        i = i - pattern.length();
        tmp.set_key(spacedWord);
        tmp.set_pos(i);
        out.push_back(tmp);
    }
    sequence.set_words(out);
}

int bucket (vector<Word>& sortedWords, int start)
{
    int bucket_length = 1;
    while (sortedWords[start].key == sortedWords[start+1].key)
    {
        ++bucket_length;
        ++start;
    }
    return bucket_length;
}

void findMatches (vector<Word>& sw1, vector<Word>& sw2, vector<tuple<unsigned long long, int, int> >& out)
{
    int skip = 0;
    for (unsigned int i = 0; i < sw1.size(); ++i)
    {
        int bl1 = bucket(sw1, i);
        if (bl1 > 1)
        {
            unsigned int limit1 = i + bl1 - 1;
            for (i; i <= limit1; ++i)
            {
                for (unsigned int j = 0 + skip; j < sw2.size(); ++j)
                {
                    int bl2 = bucket(sw2, j);
                    if (bl2 > 1)
                    {
                        unsigned int limit2 = j + bl2 - 1;
                        for (j; j <= limit2; ++j)
                        {
                            if (sw1[i].key > sw2[j].key)
                            {
                                skip += bl2 - 1;
                                continue;
                            }
                            if (sw1[i].key < sw2[j].key)
                            {
                                break;
                            }
                            if (sw1[i].key == sw2[j].key)
                            {
                                tuple<unsigned long long,int, int> tmp (sw1[i].key, sw1[i].pos, sw2[j].pos);
                                out.push_back(tmp);
                                continue;
                            }
                        }
                    }
                    else
                    {
                        if (sw1[i].key > sw2[j].key)
                        {
                            skip = j + 1;
                            continue;
                        }
                        if (sw1[i].key < sw2[j].key)
                        {
                            break;
                        }
                        if (sw1[i].key == sw2[j].key)
                        {
                            tuple<unsigned long long,int, int> tmp (sw1[i].key, sw1[i].pos, sw2[j].pos);
                            out.push_back(tmp);
                            break;
                        }
                    }
                }
            }
        }
        else
        {
            for (unsigned int j = 0 + skip; j < sw2.size(); ++j)
            {
                int bl2 = bucket(sw2, j);
                if (bl2 > 1)
                {
                    for (j; j < (j + bl2); ++j)
                    {
                        if (sw1[i].key > sw2[j].key)
                        {
                            skip += bl2 -1;
                            continue;
                        }
                        if (sw1[i].key < sw2[j].key)
                        {
                            break;
                        }
                        if (sw1[i].key == sw2[j].key)
                        {
                            skip = j + 1;
                            tuple<unsigned long long,int, int> tmp (sw1[i].key, sw1[i].pos, sw2[j].pos);
                            out.push_back(tmp);
                            break;
                        }
                    }
                }
                else
                {
                    if (sw1[i].key > sw2[j].key)
                    {
                        skip = j + 1;
                        continue;
                    }
                    if (sw1[i].key < sw2[j].key)
                    {
                        break;

                    }
                    if (sw1[i].key == sw2[j].key)
                    {
                        skip = j + 1;
                        tuple<unsigned long long,int, int> tmp (sw1[i].key, sw1[i].pos, sw2[j].pos);
                        out.push_back(tmp);
                        break;
                    }
                }
            }
        }
    }
}

double calc_distance (double mmr)
{
    double distance = -log(1 - mmr - pow( (0.2 * mmr),2.0) );
//    cout << distance << endl;
    return distance;
}

int main(int argc, char **argv)
{
    clock_t t;
    t = clock();

    int weight = atoi(argv[2]);
    int dc = atoi(argv[3]);
    int threshold = atoi(argv[4]);
    string pattern = rand_pattern(weight, dc);
    string filename = argv[1];
    filename.erase(filename.end()-5, filename.end() ); // entfernt fasta Endung
    filename.append("dm"); // fügt dm als Endung an

    vector<Sequence> sequences;
    parser(argc, argv, sequences);
    for (unsigned int i = 0; i < sequences.size(); ++i)
    {
        vector<Word> sw;
        spacedWords(sequences[i], pattern, sw);
    }

//    /* TEST */
//    for (unsigned int i = 0; i < sequences.size(); ++i)
//    {
//        cout << sequences[i].header << endl << "Found Spaced-Words:\n";
//        for (unsigned int j = 0; j < sequences[i].sorted_words.size(); ++j)
//        {
//            cout <<  sequences[i].sorted_words[j].key << " | Pos: " << sequences[i].sorted_words[j].pos << endl;
//        }
//        cout << endl;
//    }
//    /* TEST-ENDE */

    int length = sequences.size();
    double distance[length][length];
    for (unsigned int i = 0; i < sequences.size(); ++i)
    {
        distance[i][i] = 0;
        for (unsigned int j = sequences.size() - 1; j > i; --j)
        {
            vector<tuple<unsigned long long, int, int> > matchVector;
            findMatches(sequences[i].sorted_words, sequences[j].sorted_words, matchVector);
//            cout << "Matches zwischen " << sequences[i].header << " und " << sequences[j].header << " :\n";
            for (unsigned int k = 0; k < matchVector.size(); ++k)
            {
//                tuple<unsigned long long, int, int> print = matchVector[k];
//                cout << get<0>(print) << " | (" << get<1>(print) << "," << get<2>(print) << ")\n";
            }
            double mismatch_rate = score(matchVector, sequences[i], sequences[j], pattern, dc, threshold);
//            cout << mismatch_rate << endl;
            distance[i][j] = calc_distance(mismatch_rate);
//            cout << "calc_distance(mismatch_rate) = " << calc_distance(mismatch_rate) << endl;
        }
    }

    for (unsigned int i = 1; i < sequences.size(); ++i)
    {
        for (unsigned int j = 0; j < i; ++j)
        {
            if (i != j)
            {
//            cout << "distance[i][j] = " << distance[i][j] << "   distance[j][i] = " << distance[j][i] << endl;
            distance[i][j] = distance[j][i];
            }
        }
    }

    ofstream output_distance;
    output_distance.open(filename);
    output_distance << '\t' << sequences.size() << endl;
    for (unsigned int i = 0; i < sequences.size(); ++i)
    {
        output_distance << sequences[i].header;
        if (sequences[i].header.size() < 10)
        {
            for (unsigned int i = 10; i > sequences[i].header.size(); --i)
            {
                output_distance << " ";
            }
        }
        for (unsigned int j = 0; j < sequences.size(); ++j)
        {
//            cout << distance[i][j] << endl;
            if (distance[i][j] == 0)
            {
                output_distance << "  " << "0.000000";
            }
            else if (isnan(distance[i][j]) != 0)
            {
                output_distance << "  " << "5.000000";
            }
            else
            {
                output_distance << "  " << distance[i][j];
            }
        }
        output_distance << endl;
    }
    output_distance.close();

    t = clock() - t;
    cout << "Time elapsed: " << t*1.0/CLOCKS_PER_SEC << "s\n";
    //cin.get();
    return 0;
}
