#ifndef SEQUENCE_H
#define SEQUENCE_H
#include "Word.h"
#include <string>
#include <vector>
#include <algorithm>

class Sequence
{
    public:
        std::string header;
        std::vector<char> seq;
        std::vector<std::vector<Word> > sorted_words;
        std::vector<int> mismatches;
        std::vector<int> dc_pos;
        std::vector<int> starts;
        void set_header (std::string);
        void clear_header ();
        void set_seq (char);
        void set_seqvec(std::vector<char>);
        void clear_seq ();
        void set_words (std::vector<Word>);
        void set_starts(std::vector<int>);
    protected:

    private:
};

#endif // SEQUENCE_H
