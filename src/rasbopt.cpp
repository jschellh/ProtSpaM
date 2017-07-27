#include "rasbopt.hpp"

namespace rasb_opt{
    std::string OutFile = "rasbhari_set.pat";
    std::string InFile;
    double P = 0.75;
    double Q = 0.25;
    long int Seed;
    unsigned Size = 10;
    unsigned Weight = 8;
    unsigned SeqLength = 10000;
    unsigned Limit = 25000;
    unsigned H = 64;
    unsigned OptOc = 100;
    unsigned OptSens = 5000;
    unsigned MinDontcare = 10;
    unsigned MaxDontcare = 10;
    unsigned ImproveMode = 0;
    bool Improve = false;
    bool Forcesens = false;
    bool Oc = true;
    bool Quiet = true;
    bool Sens = false;
    bool SetSeed = false;
    bool Silent = true;

    void parse_length(const char* Str){
        std::string Length = std::string(Str);
        unsigned Pos = (unsigned) Length.size();
        unsigned Leng = Pos;

        for(unsigned i = 0; i < Leng; i++){
            if(std::isdigit(Length[i]) == false){
                Pos = i;    
            }
        }

        if(Pos < Leng){
            char *Tmp1, *Tmp2;
            Tmp1 = new char[Pos];
            Tmp2 = new char[Leng-Pos-1];
            for(uint32_t i = 0; i < Leng; i++){
                if(i < Pos){
                    Tmp1[i] = Length[i];
                }
                if(i > Pos){
                    Tmp2[i-Pos-1] = Length[i];
                }
            }
            MinDontcare = std::atoi(Tmp1);
            MaxDontcare = std::atoi(Tmp2);
            delete Tmp1;
            delete Tmp2;
            
        }
        else{
            MinDontcare = std::atoi(Str);
            MaxDontcare = std::atoi(Str);
        }
    }
}