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

/* Calculates every "Spaced Word" for a sequence and a given pattern*/
void spacedWords (Species& sequence, vector<char> const& pattern, vector<Word>& out)
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

void spacedWords(Species& sequence, vector<char> const& pattern)
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

/* outputs the file containing all pairwise matches */
void output_pairwise (vector<SummedMatches>& matches, string firstName, string secondName)
{
    ofstream out;
    string filename = "results/";
    firstName.erase(firstName.find_last_not_of(" \n\r\t")+1);
    filename.append(firstName);
    filename.append("_vs_");
    secondName.erase(secondName.find_last_not_of(" \n\r\t")+1);
    filename.append(secondName);
    out.open(filename);
    out << firstName << "," << secondName << endl;
    for (SummedMatches &current : matches)
    {
        if (current.score >= INT32_MIN + 1) {
            out << current.score << "," << current.frequency << "," << current.dist << endl;
        }
    }
    out.close();
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

void outputDistanceMatrix (const vector<Species>& species, const string& fileName,
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
