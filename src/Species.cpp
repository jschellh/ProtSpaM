#include "Species.h"

using namespace std;

void Species::set_header(string x)
{
    header = x;
}

void Species::clear_header()
{
    header.clear();
}

void Species::set_seq(char x)
{
    seq.push_back(x);
}

void Species::clear_seq()
{
    seq.clear();
}

void Species::set_words(vector<Word>& x)
{
    sort(x.begin(), x.end() );
    sorted_words.push_back(x);
}

void Species::set_starts(vector<int>& x)
{
    starts = x;
}
