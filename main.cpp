#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <tuple>
#include <string>
#include <cmath>
#include <omp.h>
#include "Sequence.h"
#include "score.h"
#include "rand_pattern.h"
using namespace std;

vector<string> translate = {"A","R","N","D","C","Q","E","G","H","I","L","K","M","F","P","S","T","W","Y","V","B","Z","X","*"};

string read_word (unsigned long long spaced_word, unsigned int weight)
{
    string out;
    int i = 0;
    while (out.size() < weight)
    {
        int letter = spaced_word & ((1 << 5) - 1);
//        cout << letter << " >>> " << translate[letter] << endl;
        spaced_word >>= 5;
        out.insert(i,translate[letter]);
    }
    return out;
}

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

void spacedWords (Sequence& sequence, string const& pattern, vector<Word>& out, int weight)
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
//        cout << read_word(spacedWord, weight) << endl;
        i = i - pattern.length();
        tmp.set_key(spacedWord);
        tmp.set_pos(i);
        out.push_back(tmp);
    }
    sort(out.begin(), out.end() );
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

void findMatches (vector<Word>& sw1, vector<Word>& sw2, vector<tuple<unsigned long long, int, int> >& out, int weight)
{
    int skip = 0;
    for (unsigned int i = 0; i < sw1.size(); ++i)
    {
        cout << "SpacedWord 1 = " << read_word(sw1[i].key, weight);
        int bl1 = bucket(sw1, i);
        if (bl1 > 1)
        {
            unsigned int limit1 = i + bl1 - 1;
            for (i; i <= limit1; ++i)
            {
                for (unsigned int j = 0 + skip; j < sw2.size(); ++j)
                {
                    cout << "   SpacedWord 2 = " << read_word(sw2[j].key, weight) << endl;
                    int bl2 = bucket(sw2, j);
                    if (bl2 > 1)
                    {
                        cout << " & (bucket-bucket) j = " << j << endl;
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
                        cout << " & (if-single) j = " << j << endl;
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
                cout << "   SpacedWord 2 = " << read_word(sw2[j].key, weight) << endl;
                int bl2 = bucket(sw2, j);
                if (bl2 > 1)
                {
                    cout << " & (else bucket) j = " << j << endl;
                    unsigned int limit = j + bl2 - 1;
                    for (j; j <= limit; ++j)
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
                            skip = j + 1;
                            tuple<unsigned long long,int, int> tmp (sw1[i].key, sw1[i].pos, sw2[j].pos);
                            out.push_back(tmp);
							break;
                        }
                    }
                }
                else
                {
                    cout << " & (else) j = " << j << endl;
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
    return distance;
}

string delete_suffix (string file)
{
    string out;
    unsigned int found = file.find(".");
    if (found != string::npos)
    {
        out = file.substr(0, found);
        cout << out << endl;
        return out;
    }
    else
    {
        return file;
    }
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
    string pattern = rand_pattern(weight, dc);
    cout << pattern << endl;
    string filename = delete_suffix(argv[1]);
    filename.append(".dm");
    vector<Sequence> sequences;
    parser(argc, argv, sequences);

//    #pragma omp parallel for
    for (unsigned int i = 0; i < sequences.size(); ++i)
    {
        cout << "Spezies " << sequences[i].header << ":\n";
        vector<Word> sw;
        spacedWords(sequences[i], pattern, sw, weight);
        for (unsigned int i = 0; i < sw.size(); ++i)
        {
            cout << sw[i].key << " | " << sw[i].pos;
            cout << " | " << read_word(sw[i].key, weight) << endl;
        }
        cout << endl;
    }

    int length = sequences.size();
    double distance[length][length];


    for (unsigned int i = 0; i < sequences.size(); ++i)
    {
		cout << "checking " << i << ". sequence with \n";
        distance[i][i] = 0;
		#pragma omp parallel for
        for (unsigned int j = sequences.size() - 1; j > i; --j)
        {
			cout << j << ". sequence (" << omp_get_thread_num() << ")\n" << endl;
            vector<tuple<unsigned long long, int, int> > matchVector;
            findMatches(sequences[i].sorted_words, sequences[j].sorted_words, matchVector, weight);

			cout << "Matches found:\n";
            for (unsigned int k = 0; k < matchVector.size(); ++k)
            {

                cout << read_word(get<0>(matchVector[k]), weight) << " | (" << get<1>(matchVector[k]) << "," << get<2>(matchVector[k]) << ")\n";
            }
            cout << "Alles ging gut!\n";
            double mismatch_rate = score(matchVector, sequences[i], sequences[j], pattern, dc, threshold);
            cout << "Hier auch noch alles gut!\n";
            distance[i][j] = calc_distance(mismatch_rate);
            distance[j][i] = distance[i][j];
            cout << "Fertig! Alles gut gegangen!\n";
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
