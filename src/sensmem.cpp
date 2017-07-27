#include "sensmem.hpp"

double sensitivity_memory::MemVal = 0;

/**
 * A function, which organizes the check of the memory usage and
 *      interacts with the user, if a lot of memory is currently used.
 * 
 * @return  Possible user decision, if the calculation has to stop.
 */
bool sensitivity_memory::check_memory(){
    std::string Input;
    uint32_t Stry;
    bool Answer, InputErr;
    MemVal = available_memory();
    Answer = false;
    Stry = 0;
    InputErr = false;
    if(MemVal< 0.25){
        security_message("lowmemory",-1);
        while(Stry < 3){
            Stry++;
            InputErr = false;
            std::cin >> Input;
            switch(Input[0]){
                case 'Y':
                case 'y':
                    rasb_opt::Forcesens = true;
                    Stry = 4;
                    Answer = false;
                    break;
                case 'N':
                case 'n':
                    Stry = 4;
                    Answer = true;
                    break;
                default:
                    security_message("inputconf",-1);
                    Answer = true;
                    InputErr = true;
                    break;
            }
        }
    }

    if(InputErr && Stry < 4){
        security_message("inputerr",-1);
    }
    return Answer;
}


/**
 * Calculates the amount of available memory in total and returns the amount as percentage.
 *
 * @return  The percentage of available memory in total.
 */
double sensitivity_memory::available_memory(){
    struct sysinfo MemInfo;
    double IsAvailable;
    uint64_t RamTotal, RamFree;

    sysinfo (&MemInfo);

    RamTotal = MemInfo.totalram;
    RamFree = MemInfo.freeram;
    RamTotal *= MemInfo.mem_unit;
    RamFree *= MemInfo.mem_unit;

    IsAvailable = (double)RamFree/(double)RamTotal;
    return IsAvailable;
}

void sensitivity_memory::security_message(std::string errmsg, int pos){
    if (errmsg == "noimprove") {
        printf("%c[1;33m", 27);
        std::cerr << "Using your pattern conditions it is not sensible to improve your pattern, sorry!" << std::endl;
        std::cerr << "Deactivating improve mode\n" << std::endl;
        printf("%c[0m", 27);
        return;
    }
    else if (errmsg == "update"){
        printf("%c[1;31m\n####IMPROTANT####\n", 27);
        printf("%c[0m", 27);
        std::cerr << "Due to some configuration errors (see above), your submitted parameters have been updated!\n" << std::endl;
        return;
    }
    else if (errmsg == "bitmode") {
        printf("%c[1;33m", 27);
        std::cerr << "A patternlength is over 63, leaving bitmode ..." << std::endl;
        std::cerr << "Using your pattern conditions it is not possible to calculate the sensitivity!" << std::endl;
        std::cerr << "Deactivating sensitivity calculation\n" << std::endl;
        printf("%c[0m", 27);
        return;
    }
    else if (errmsg == "lowmemory"){
        std::cerr << "Sensitivity Calculation consume a lot of memory (>75% RAM is used in total). If you want to continue, ";
        printf("%c[1;33m", 27);
        std::cerr << "PLEASE SAVE ALL RELEVANT DATA!";
        printf("%c[0m", 27);
        std::cout << std::endl;
        std::cout << std::endl << "Do you want to continue calculating the sensitivity on your own risk (PC crash)? [Y/N]\t";
    }
    else if (errmsg == "inputconf"){
        std::cerr << "Please enter 'Y' to continue with the sensitivity calculating or 'N' to abort the calculation!" << std::endl;
    }
    else if (errmsg == "inputerr"){
        std::cerr << "Could not read answer in three attempts, abort sensitivity calculation!" << std::endl;
    }
    else if (errmsg == "memerror"){
        std::cerr << "There is not enough memory! Aborting sensitivity calculation!" << std::endl;
    }
}