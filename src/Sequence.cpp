#include "Sequence.h"

using namespace std;

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

void Sequence::set_seqvec(vector<char> x)
{
    for (unsigned int i = 0; i < x.size(); ++i)
    {
        set_seq(x[i]);
    }
}

void Sequence::clear_seq()
{
    seq.clear();
}

void Sequence::set_words(vector<Word> x)
{
    sort(x.begin(), x.end() );
    sorted_words.push_back(x);
}

void Sequence::set_starts(vector<int> x)
{
    starts = x;
}
