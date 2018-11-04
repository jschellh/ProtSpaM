#ifndef SPECIES_H
#define SPECIES_H
#include "Word.h"
#include <string>
#include <vector>
#include <algorithm>

class Species
{
    public:
        std::string header;
        std::vector<char> seq;
        std::vector<std::vector<Word> > sorted_words;
        std::vector<int> starts;
        void set_header (std::string);
        void clear_header ();
        void set_seq (char);
        void clear_seq ();
        void set_words (std::vector<Word>&);
        void set_starts(std::vector<int>&);
    protected:

    private:
};

#endif // SPECIES_H
