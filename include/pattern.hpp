#ifndef PATTERN_HPP_
#define PATTERN_HPP_

#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <random>
#include <string>
#include <utility>
#include <vector>

class pattern{
    public:
        pattern(uint64_t Idx = 0);
        pattern(const pattern & Pat);
        pattern(unsigned Weight, unsigned DontCare,unsigned Idx = 0);
        pattern(unsigned Weight, unsigned DontCare, uint64_t Seed, unsigned Idx = 0);
        pattern(std::string & StrPat);
        pattern(std::string && StrPat);
        pattern(std::vector<char> & StrPat);
        pattern(std::vector<unsigned char> & StrPat);

        void push_back(std::string && Chars);
        void push_back(std::string & Chars);
        void push_back(char C);
        void push_back(unsigned char C);

        std::string to_string() const;
        std::vector<unsigned> match_pos() const;
        std::vector<unsigned> dc_pos() const;
        bool is_match(unsigned Pos) const;
        void bit_swap(unsigned PosA, unsigned PosB);
        void random_swap();
        void random_swap(uint64_t Seed);

        void random(unsigned Weight, unsigned DontCare);
        void random(unsigned Weight, unsigned DontCare, uint64_t Seed);

        unsigned char operator[](size_t Idx) const;
        unsigned char & operator[](size_t Idx);
        bool operator<(const pattern & P) const;
        bool operator>(const pattern & P) const;
        bool operator==(const pattern & P) const;
        bool operator!=(const pattern & P) const;

        double score() const;
        uint64_t bits() const;
        unsigned weight() const;
        unsigned length() const;
        unsigned dontcare() const;
        unsigned get_overlap(const pattern & P, int Shift) const;
        unsigned idx() const;
        void set_score(double Scr);
        void set_idx(unsigned Idx);

        typedef std::vector<unsigned char>::iterator iterator;
        iterator begin();
        iterator end();


    private:
        void _check_pattern();

        std::vector<unsigned char> _VectorPattern;
        std::vector<unsigned> _MatchPos;
        std::vector<unsigned> _DCPos;
        double _Score;
        uint64_t _BitPattern;
        unsigned _Idx;
        bool _IsBit;
};
#endif