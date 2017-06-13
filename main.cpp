#include <iostream>
#include <fstream>
#include <vector>
#include <time.h>
#include <algorithm>
#include <tuple>
using namespace std;

class Word
{
    public:
        unsigned long long key;
        unsigned int pos;
        void set_key (unsigned long long);
        void set_pos (unsigned int);
        bool operator< (const Word& w) const { return key < w.key; }
};

void Word::set_key (unsigned long long x)
{
    key = x;
}

void Word::set_pos (unsigned int x)
{
    pos = x;
}

class Sequence
{
    public:
        string header;
        vector<char> seq;
        vector<Word> sorted_words;
        void set_header (string);
        void clear_header ();
        void set_seq (char);
        void clear_seq ();
        void set_words (vector<Word>);
};

void Sequence::set_header(string x)
{
    header = x;
}

void Sequence::clear_header()
{
    header.clear();
}

void Sequence::set_seq(char x)
{
    seq.push_back(x);
}

void Sequence::clear_seq()
{
    seq.clear();
}

void Sequence::set_words(vector<Word> x)
{
    sort(x.begin(), x.end() );
    sorted_words = x;
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
                tmp.set_header(line.substr(1) );
            }
            tmp.clear_seq();
        }
        else if (!tmp.header.empty() )
        {

            for (unsigned int i = 0; i < line.size(); ++i)
            {
                switch (line[i]) {
                    case 'A': tmp.set_seq(1); break;
                    case 'B': tmp.set_seq(2); break;
                    case 'C': tmp.set_seq(3); break;
                    case 'D': tmp.set_seq(4); break;
                    case 'E': tmp.set_seq(5); break;
                    case 'F': tmp.set_seq(6); break;
                    case 'G': tmp.set_seq(7); break;
                    case 'H': tmp.set_seq(8); break;
                    case 'I': tmp.set_seq(9); break;
                    case 'K': tmp.set_seq(10); break;
                    case 'L': tmp.set_seq(11); break;
                    case 'M': tmp.set_seq(12); break;
                    case 'N': tmp.set_seq(13); break;
                    case 'P': tmp.set_seq(14); break;
                    case 'Q': tmp.set_seq(15); break;
                    case 'R': tmp.set_seq(16); break;
                    case 'S': tmp.set_seq(17); break;
                    case 'T': tmp.set_seq(18); break;
                    case 'V': tmp.set_seq(19); break;
                    case 'W': tmp.set_seq(20); break;
                    case 'X': tmp.set_seq(21); break;
                    case 'Y': tmp.set_seq(22); break;
                    case 'Z': tmp.set_seq(23); break;
                    case '*': tmp.set_seq(24); break;
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
    while(sortedWords[start].key == sortedWords[start+1].key)
    {
        ++bucket_length;
        ++start;
    }
    return bucket_length;
}

void findMatches (vector<Word>& sw1, vector<Word>& sw2, vector<tuple<unsigned long long, int, int> >& out)
{
    cout << "Welcome inside findMatches!\n";
    int skip = 0;
    for (unsigned int i = 0; i < sw1.size(); ++i)
    {
        cout << sw1[i].key << "  >>>   " << sw1[i+1].key << endl;
        int bl1 = bucket(sw1, i);
        cout << "bl1 = " << bl1 << endl;
        if (bl1 > 1)
        {
            unsigned int limit1 = i + bl1 - 1;
            for (i; i <= limit1; ++i)
            {
                cout << "skip = " << skip << endl;
                for (unsigned int j = 0 + skip; j < sw2.size(); ++j)
                {
                    cout << sw2[j].key << "  >>>   " << sw2[j+1].key << endl;
                    int bl2 = bucket(sw2, j);
                    cout << "bl2 = " << bl2 << endl;
                    if (bl2 > 1)
                    {
                        cout << "1. bucket ; 2. bucket\n";
                        unsigned int limit2 = j + bl2 - 1;
                        for (j; j <= limit2; ++j)
                        {
                            cout << "j = " << j << " | bl2 = " << bl2 << endl;
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
                                cout << "match found =)\n" << endl; //kein skip, wird hoch gezählt, wenn 1.single
                                tuple<unsigned long long, int, int> tmp (sw1[i].key, sw1[i].pos, sw2[j].pos);
                                out.push_back(tmp);
                                continue;
                            }
                        }
                    }
                    else
                    {
                        cout << "1. bucket ; 2. single\n";
                        if (sw1[i].key > sw2[j].key)
                        {
                            skip = j + 1;
                            cout << "continue\n" << endl;
                            continue;
                        }
                        if (sw1[i].key < sw2[j].key)
                        {
                            cout << "break\n" << endl;
                            break;
                        }
                        if (sw1[i].key == sw2[j].key)
                        {
                            cout << "match found =)\n" << endl; //kein skip, wird hoch gezählt, wenn 1.single
                            tuple<unsigned long long, int, int> tmp (sw1[i].key, sw1[i].pos, sw2[j].pos);
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
                cout << "j = " << j << endl;
                cout << sw2[j].key << "  >>>   " << sw2[j+1].key << endl;
                int bl2 = bucket(sw2, j);
                cout << "bl2 = " << bl2 << endl;
                if (bl2 > 1)
                {
                    cout << "1. single ; 2. bucket\n";
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
                            tuple<unsigned long long, int, int> tmp (sw1[i].key, sw1[i].pos, sw2[j].pos);
                            out.push_back(tmp);
                            break;
                        }
                    }
                }
                else
                {
                    cout << "1. single ; 2.single\n";
                    if (sw1[i].key > sw2[j].key)
                    {
                        skip = j + 1;
                        cout << "continue\n" << endl;
                        continue;
                    }
                    if (sw1[i].key < sw2[j].key)
                    {
                        cout << "break\n" << endl;
                        break;

                    }
                    if (sw1[i].key == sw2[j].key)
                    {
                        skip = j + 1;
                        cout << "match found =)\n" << endl;
                        tuple<unsigned long long, int, int> tmp (sw1[i].key, sw1[i].pos, sw2[j].pos);
                        out.push_back(tmp);
                        break;
                    }
                }
            }
        }
    }
}

int main(int argc, char **argv)
{
    clock_t t;
    t = clock();

    string pattern = argv[2];

    vector<Sequence> sequences;
    parser(argc, argv, sequences);
    for (unsigned int i = 0; i < sequences.size(); ++i)
    {
        vector<Word> sw;
        spacedWords(sequences[i], pattern, sw);
    }

    /* TEST */
    for (unsigned int i = 0; i < sequences.size(); ++i)
    {
        cout << sequences[i].header << endl << "Found Spaced-Words:\n";
        for (unsigned int j = 0; j < sequences[i].sorted_words.size(); ++j)
        {
            cout <<  sequences[i].sorted_words[j].key << " | Pos: " << sequences[i].sorted_words[j].pos << endl;
        }
        cout << endl;
    }
    /* TEST-ENDE */

    for (unsigned int i = 0; i < sequences.size(); ++i)
    {
        for (unsigned int j = sequences.size() - 1; j > i; --j)
        {
            vector<tuple<unsigned long long, int, int> > matchVector;
            findMatches(sequences[i].sorted_words, sequences[j].sorted_words, matchVector);
            cout << "Matches zwischen " << sequences[i].header << " und " << sequences[j].header << " :\n";
            for (unsigned int k = 0; k < matchVector.size(); ++k)
            {
                tuple<unsigned long long, int, int> print = matchVector[k];
                cout << get<0>(print) << " | (" << get<1>(print) << "," << get<2>(print) << ")\n";
            }
        }
    }


    t = clock() - t;
    cout << "Time elapsed: " << t*1.0/CLOCKS_PER_SEC << "s\n";
    //cin.get();
    return 0;
}
