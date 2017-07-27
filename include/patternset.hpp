#ifndef PATTERNSET_HPP_
#define PATTERNSET_HPP_

#include <algorithm>
#include <assert.h>
#include <limits>
#include <iostream>
#include <fstream>
#include <vector>
#include "pattern.hpp"

class patternset{
    public:
        patternset();
        patternset(const patternset & PatSet);
        patternset(unsigned Numb, unsigned Weight, unsigned DontCare, bool Uniq = false);
        patternset(unsigned Numb, unsigned MinW, unsigned MaxW, unsigned MinDc, unsigned MaxDc, bool Uniq = false);
        patternset(std::string InputFile);

        void push_back(pattern & Pat);
        void push_back(pattern && Pat);
        void push_back(std::string & Pat);
        void push_back(std::string && Pat);
        void push_back(std::vector<char> & Pat);
        void push_back(std::vector<unsigned char> & Pat);
        void random(unsigned Size, unsigned Length, unsigned Weight, bool Uniq = false);
        void random(unsigned Size, unsigned MinLength, unsigned MaxLength, unsigned Weight, bool Uniq = false);
        void random(unsigned Size, unsigned MinLength, unsigned MaxLength, unsigned MinWeight, unsigned MaxWeight, bool Uniq = false);
        void sort();
        bool is_uniq(const pattern & Pat) const;
        void random_swap_uniq(unsigned Idx);
        void to_file(std::string OutFile = "patternset.pat");

        pattern operator[](size_t Idx) const;
        pattern & operator[](size_t Idx);
        bool operator<(const patternset & P) const;
        bool operator>(const patternset & P) const;

        unsigned max_weight() const;
        unsigned min_weight() const;
        unsigned weight() const;
        unsigned max_dontcare() const;
        unsigned min_dontcare() const;
        unsigned dontcare() const;
        unsigned max_length() const;
        unsigned min_length() const;
        unsigned length() const;
        unsigned size() const;

        double score() const;
        void set_score(double Score);


        typedef std::vector<pattern>::iterator iterator;
        iterator begin();
        iterator end();

    private:
        template<typename T>
        void _adjust(T & MinValue, T & MaxValue);
        std::vector<std::string> _split(std::string &S);
        std::vector<pattern> _PatternSet;
        double _Score;
        unsigned _MinWeight;
        unsigned _MaxWeight;
        unsigned _MinDontCare;
        unsigned _MaxDontCare;
};
template<typename T>
void patternset::_adjust(T & MinValue, T & MaxValue){
    if(MaxValue < MinValue){
        std::swap(MaxValue,MinValue);
    }
}
#endif