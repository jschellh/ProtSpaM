#include <fstream>
#include "misc.h"

using namespace std;

/* Prints out all the patterns used */
void print_patterns (vector<vector<char> > patterns)
{
    cout << "Patterns used:\n";
    for (unsigned int i = 0; i < patterns.size(); ++i)
    {

        for (unsigned int j = 0; j < patterns[i].size(); ++j)
        {
            cout << patterns[i][j];
        }
        if (i == patterns.size() - 1)
        {
            break;
        }
        cout << endl;
    }
    cout << endl;
}

/* Prints out a given pattern */
void print_pattern (vector<char> pattern)
{
    for (unsigned int i = 0; i < pattern.size(); ++i)
    {
        cout << pattern[i];
    }
    cout << endl;
}

/* Parse pattern set from file */
vector<vector<char>> parsePatterns (string patternSet)
{
    ifstream patIn(patternSet);
    if (!patIn.good() )
    {
        cerr << "Error opening '" << patternSet << "'. Bailing out.\n";
    }
    vector<vector<char>> out;
    string line;
    while (!patIn.eof())
    {
        vector<char> pattern;
        getline(patIn, line);
        if (! line.empty() ) {
            for (auto &c : line)
            {
                pattern.push_back(c);
            }
            out.push_back(pattern);
        }
    }
    patIn.close();
    return out;
}

/* Returns a string of a given long word*/
vector<string> translate = {"A","R","N","D","C","Q","E","G","H","I","L","K","M","F","P","S","T","W","Y","V","B","Z","X","*","J"};
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

/* Returns true if the spaced-word is a real context (i.e. not containing 'J') */
bool isContext (unsigned long long spaced_word, unsigned int weight)
{
    string out;
    int i = 0;
    while (out.size() < weight)
    {
        int letter = spaced_word & ((1 << 5) - 1);
        spaced_word >>= 5;
        if (translate[letter] == "J")
        {
//            cout << "Letter \"" << translate[letter] << "\" found!\n" ;
            return false;
        }
        else
        {
            out.insert(i,translate[letter]);
        }
    }
    return true;
}

/* Calculates every "Spaced Word" for a sequence and a given pattern*/
void spacedWords (Sequence& sequence, vector<char> const& pattern, vector<Word>& out)
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
        i = i - pattern.size();
        tmp.set_key(spacedWord);
        tmp.set_pos(i);
        out.push_back(tmp);
    }
    sequence.set_words(out);
}

void spacedWords(Sequence& sequence, vector<char> const& pattern)
{
    vector<Word> words;
    for (unsigned int i = 0; i < sequence.starts.size(); ++i)
    {
        unsigned int stop;
        if (i < sequence.starts.size() - 1)
        {
            stop = sequence.starts[i+1];
        }
        else
        {
            stop = sequence.seq.size();
        }
        for (unsigned int j = sequence.starts[i]; j <= (stop - pattern.size() ); ++j)
        {
            Word tmp;
            unsigned long long spacedWord = 0;
            for (unsigned int k = 0; k < pattern.size(); ++k)
            {
                if (pattern[k] == '1')
                {
                    spacedWord <<= 5;
                    spacedWord |= sequence.seq[j];
                }
                ++j;
            }
            j = j - pattern.size();
            tmp.set_key(spacedWord);
            tmp.set_pos(j);
            words.push_back(tmp);
        }
    }
    sequence.set_words(words);
}

/* Calculates the distance using Kimura Formula */
double calc_distance (double mmr)
{
    double distance = -log(1 - mmr - (0.2 * pow(mmr,2.0) ) );
    return distance;
}

/* Deletes the file-suffix of the input-filename */
string delete_suffix (string file)
{
    string out;
    unsigned int found = file.find_last_of(".");
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

string delete_prefix (string file)
{
    string out;
    unsigned int found = file.find_last_of("/");
    if (found != string::npos)
    {
        out = file.substr(found + 1);
//        cout << out << endl;
        return out;
    }
    else
    {
        return file;
    }
}

/* Generates patterns (pat_number times) */
vector<vector<char> > rand_pattern(int w, int d, int pat_number)
{
    vector<vector<char> > pattern;
    for (int i = 0; i < pat_number; ++i)
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

void time_elapsed(double start)
{
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
}

void outputDistanceMatrix (const vector<Sequence>& species, const string& fileName,
                           const vector<vector<double> >& distanceMatrix, const bool& tooDistant) {
    ofstream output_distance;
    output_distance.open(fileName);
    output_distance << '\t' << species.size() << endl;
    for (unsigned int i = 0; i < species.size(); ++i) {
        if (species[i].header.size() == 10) {
            output_distance << species[i].header;
        }
        else if (species[i].header.size() > 10) {
            output_distance << species[i].header.substr(0,9);
        }
        else {
            output_distance << species[i].header;
            for (unsigned int n = 0; n < 10 - species[i].header.size(); ++n) {
                output_distance << " ";
            }
        }
        for (unsigned int j = 0; j < species.size(); ++j) {
            if (distanceMatrix[i][j] == 0) {
                output_distance << "0.000000" << "  " ;
            }
            else if (std::isnan(distanceMatrix[i][j]) != 0) {
                output_distance << "10.000000"  << "  " ;
            }
            else {
                output_distance << distanceMatrix[i][j] << "  " ;
            }
        }
        output_distance << endl;
    }
    output_distance.close();
    if (tooDistant) {
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
}
