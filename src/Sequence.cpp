#include "Sequence.h"

//class Sequence
//{
//    public:
//        string header;
//        vector<char> seq;
//        vector<Word> sorted_words;
//        void set_header (string);
//        void clear_header ();
//        void set_seq (char);
//        void clear_seq ();
//        void set_words (vector<Word>);
//};
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
    sorted_words = x;
}
