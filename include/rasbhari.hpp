#ifndef RASBHARI_HPP_
#define RASBHARI_HPP_

#include <ios>
#include <fstream>
#include "patternset.hpp"
#include "pattern.hpp"
#include "rasbcomp.hpp"
#include "rasbopt.hpp"

class rasbhari{
    public:
        rasbhari();
        rasbhari(const rasbhari &RasbObj);
        rasbhari(unsigned Size, unsigned Weight, unsigned DontCare);
        rasbhari(unsigned Size, unsigned Weight, unsigned MinDontCare, unsigned MaxDontCare);
        rasbhari(std::string PatternFile);

        void push_back(pattern & Pat);
        void push_back(pattern && Pat);

        void calculate();
        double calculate_pair(size_t Idx1, size_t Idx2);
        void calculate_sensitivity();
        void update(unsigned Idx);
        bool climb_hill();
        bool hill_climbing(unsigned Limit = rasb_opt::Limit);
        bool iterate_hill_climbing(unsigned Limit = rasb_opt::Limit, unsigned Iteration = rasb_opt::OptOc);
        bool climb_hill_sensitivity(unsigned Limit = rasb_opt::Limit, unsigned Iteration = rasb_opt::OptOc, bool InitSens = true);
        bool hill_climbing_sensitivity(unsigned Limit = rasb_opt::Limit, unsigned Iteration = rasb_opt::OptOc, unsigned Loop = rasb_opt::OptSens);

        patternset pattern_set() const;
        patternset & pattern_set();
        double score() const;
        double sensitivity() const;
        unsigned size() const;
        void print();
        void to_file(std::string OutFile = rasb_opt::OutFile);

        typedef std::vector<pattern>::iterator iterator;
        iterator begin();
        iterator end();

    private:
        void _debug();
        void _make_pattern_list();
        void _adjust_coef_mat();

        std::vector< std::vector<double> > _CoefMat;
        std::vector<pattern> _PatternList;
        patternset _RasbhariPattern;
        double _RasbhariScore;
        double _RasbhariSensitivity;
        unsigned long _PatNo;
};

#endif