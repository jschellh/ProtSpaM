#ifndef RASBOPT_HPP_
#define RASBOPT_HPP_

#include <cstdlib>
#include <string>

namespace rasb_opt{
    extern std::string OutFile;
    extern std::string InFile;
    extern double P;
    extern double Q;
    extern long int Seed;
    extern unsigned Size;
    extern unsigned Weight;
    extern unsigned MinDontcare;
    extern unsigned MaxDontcare;
    extern unsigned SeqLength;
    extern unsigned Limit;
    extern unsigned H;
    extern unsigned OptOc;
    extern unsigned OptSens;
    extern unsigned ImproveMode;
    extern bool Improve;
    extern bool Forcesens;
    extern bool Oc; 
    extern bool Quiet;
    extern bool Sens;
    extern bool SetSeed;
    extern bool Silent;

    void parse_length(const char* Str);
};
#endif
