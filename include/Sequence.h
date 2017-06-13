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
        std::vector<Word> sorted_words;
        void set_header (std::string);
        void clear_header ();
        void set_seq (char);
        void clear_seq ();
        void set_words (std::vector<Word>);
    protected:

    private:
};

#endif // SEQUENCE_H
