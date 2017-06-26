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

void Sequence::clear_seq()
{
    seq.clear();
}

void Sequence::set_words(vector<Word> x)
{
    sort(x.begin(), x.end() );
    sorted_words.push_back(x);
}
