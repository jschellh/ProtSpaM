#include "misc.h"

using namespace std;

/* Prints out all the patterns used */
void print_patterns (vector<vector<char> > patterns)
{
    cout << "Patterns used: ";
    for (unsigned int i = 0; i < patterns.size(); ++i)
    {

        for (unsigned int j = 0; j < patterns[i].size(); ++j)
        {
            cout << patterns[i][j];
        }
        cout << " | ";
    }
    cout << endl;
}


/* Returns a string of a given long word*/
vector<string> translate = {"A","R","N","D","C","Q","E","G","H","I","L","K","M","F","P","S","T","W","Y","V","B","Z","X","*"};
string read_word (unsigned long long spaced_word, unsigned int weight)
{
    string out;
    int i = 0;
    while (out.size() < weight)
    {
        int letter = spaced_word & ((1 << 5) - 1);
        spaced_word >>= 5;
        out.insert(i,translate[letter]);
    }
    return out;
}

/* Calculates every "Spaced Word" for a sequence and a given pattern*/
void spacedWords (Sequence& sequence, vector<char> const& pattern, vector<Word>& out, int weight)
{
    for (unsigned int i = 0; i < (sequence.seq.size() - pattern.size() + 1); ++i)
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
        i = i - pattern.size();
        tmp.set_key(spacedWord);
        tmp.set_pos(i);
        out.push_back(tmp);
    }
    sequence.set_words(out);
}

/* Calculates the distance using Kimura Formula */
double calc_distance (double mmr)
{
    double distance = -log(1 - mmr - pow( (0.2 * mmr),2.0) );
    return distance;
}

/* Deletes the file-suffix of the input-filename */
string delete_suffix (string file)
{
    string out;
    unsigned int found = file.find(".");
    if (found != string::npos)
    {
        out = file.substr(0, found);
//        cout << out << endl;
        return out;
    }
    else
    {
        return file;
    }
}

/* Generates patterns (pat_number times) */
vector<vector<char> > rand_pattern(int w, int d, int pat_number){
    vector<vector<char> > pattern;

    for (unsigned int i = 0; i < pat_number; ++i)
    {
        int lg = w + d;
        int carepos = 2;                                //zwei care-positionen stehen zu beginn fest
        vector<char> tmp(lg, '0');
        tmp[0] = '1';                                     //erste und letzte Stelle des Patterns = 1
        tmp[lg-1]= '1';
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        default_random_engine generator (seed);
        uniform_int_distribution<int> position(1,lg-2);

        while (carepos < w){
            int pos = position(generator);
                if (tmp[pos] == '0'){
                tmp[pos] = '1';
                carepos++;
                }
        }
        pattern.push_back(tmp);
    }
    return pattern;
}
