#include "rasbcomp.hpp"

bool rasbhari_compute::DoOnce = false;

double rasbhari_compute::pair_coef_var(pattern & Pat1, pattern & Pat2){
    double CoEf = 0;
    double LengthMean = (Pat1.length() + 1 + Pat2.length()) / 2;
    for(int s = -(int)Pat2.length()+1; s < (int)Pat1.length(); s++){
        unsigned Overlap = Pat1.get_overlap(Pat2,s);
        CoEf += (rasb_opt::SeqLength - LengthMean + 1)*(pow(rasb_opt::P, 2*rasb_opt::Weight-Overlap) - pow(rasb_opt::P, 2 * rasb_opt::Weight))+(rasb_opt::SeqLength - LengthMean + 1)*(rasb_opt::SeqLength - LengthMean)*(pow(rasb_opt::Q, 2*rasb_opt::Weight-Overlap) - pow(rasb_opt::Q, 2 * rasb_opt::Weight));
    }
    return CoEf;
}

double rasbhari_compute::pair_coef_var_sym(pattern & Pat1, pattern & Pat2){
    double CoEf = 0;
    double LengthMean = (Pat1.length() + 1 + Pat2.length()) / 2;
    for(int s = 0; s < (int)Pat1.length(); s++){
        unsigned Overlap = Pat1.get_overlap(Pat2,s);
        CoEf += (rasb_opt::SeqLength - LengthMean + 1)*(pow(rasb_opt::P, 2*rasb_opt::Weight-Overlap) - pow(rasb_opt::P, 2 * rasb_opt::Weight))+(rasb_opt::SeqLength - LengthMean + 1)*(rasb_opt::SeqLength - LengthMean)*(pow(rasb_opt::Q, 2*rasb_opt::Weight-Overlap) - pow(rasb_opt::Q, 2 * rasb_opt::Weight));
    }
    return CoEf;
}

double rasbhari_compute::pair_coef_oc(pattern & Pat1, pattern & Pat2){
    double CoEf = 0;
    for(int s = -(int)Pat2.length()+1; s < (int)Pat1.length(); s++){
        CoEf += (uint64_t)1 << Pat1.get_overlap(Pat2,s);
    }
    return (double) CoEf;
}

/**
 * Interface function to create the right pattern format for the speed functions.
 *
 * @param Pattern   Contains the patternset, for which the sensitivity has to
 *                  be calculated.
 *
 * @return          Sensitivity of the patterset.
 *
 */
double rasbhari_compute::sensitivity(patternset & Pattern){
    std::vector<char> Clean;
    double SensVal = -1;
    unsigned PSize = Pattern.size();
    if(!DoOnce){
        std::cout << "\rCalculating sensitivity ...";
        std::cout.flush();
    }
    char** Pats = new char*[PSize];   //set of seeds
    for (unsigned i = 0; i < PSize; i++) {
        unsigned Length = Pattern[i].length();
        Pats[i] = new char[Length + 1];
    }

    for (unsigned i = 0; i < PSize; i++) {
        unsigned Length = Pattern[i].length();
        for (unsigned j = 0; j < Length; j++) {
            if (Pattern[i][j] == 1) {
                Pats[i][j] = '1';
            }
            else {
                Pats[i][j] = '0';
            }
        }
        Pats[i][Length] = '\0';
    }
    try{
        SensVal = speedsens::MULTIPLE_SENSITIVITY2(Pats, rasb_opt::Size, rasb_opt::H, rasb_opt::P);
        if(!DoOnce){
            std::cout << "\r" << std::string(80,' ') << "\r";
            std::cout.flush();
            DoOnce = true;
        }
    }
    catch(std::bad_alloc){
        if(!DoOnce){
            std::cout << "\r" << std::string(80,' ') << "\r";
            std::cout.flush();
            DoOnce = true;
        }
        std::cout << std::endl;
        sensitivity_memory::security_message("memerror",-1);
        rasb_opt::Sens = false;
        rasb_opt::OptSens = 1;
        SensVal = -1;
    }
    return SensVal;
}