#include "speedsens.hpp"

/*===SPEED===================================================================*/
/*---Acknowledgements--------------------------------------------------------*/
/**
 * We would like to thank Ilie and Ilie for using there programm part of
 * SpEED to calculate the sensitivity for a patternset off:
 *
 * Lucian Ilie, Silvana Ilie, and Anahita M. Bigvand. SpEED: fast computation
 * of sensitive spaced seeds. Bioinformatics, 27:2433–2434, 2011.
 */
inline long long speedsens::BIN_REVERSED_TO_INT2(char *s)     // converts the reversed of the binary string s into integer
{                                       // works also with * instead of 0
    long long val = 0;
    long long l = std::strlen(s), i = 0, temp = 1;
    for (i = 0; i <= l - 1; i++)
    {
        if (s[i] == '1') val += temp;
        temp *= 2;
    }
    return(val);
}
/**
* Computing sensitivity of a set of SEEDS with the given parameters
* using the dynamic programming of (Li et al., 2004)
*/
double speedsens::MULTIPLE_SENSITIVITY2(char** SEEDS, int NO_SEEDS, long long N, double P)
{
    long long i = 0, j = 0, b = 0, pos = 0, MAX_L = 0, level = 0, prev_level_start = 0, prev_level_end = 0, compatible = 0,
        hit = 0, suffix_link = 0, zero_link = 0, new_i = 0, tmp = 0;
    long long b_zero = 0, b_one = 0;
    double f0 = 0, f1 = 0;
    // compute the lengths of the seeds and MAX_L = the length of the longest seed

    if(!rasb_opt::Forcesens){
        bool stop = sensitivity_memory::check_memory();
        if(stop){
            rasb_opt::Sens = false;
            rasb_opt::OptSens = 1;
            rasb_opt::Forcesens = false;
            sensitivity_memory::MemVal = 1;
            return -1;
        }
    }
    long long* seed_length = new long long[NO_SEEDS];
    for (i = 0; i <= NO_SEEDS - 1; i++) {
        seed_length[i] = strlen(SEEDS[i]);
        //cout << seed_length[i] << endl;
        if (MAX_L < seed_length[i]) MAX_L = seed_length[i];
    }
    // compute the integer values of the reversed seeds INTeger REVersed SEEDS
    if(!rasb_opt::Forcesens){
        bool stop = sensitivity_memory::check_memory();
        if(stop){
            rasb_opt::Sens = false;
            rasb_opt::OptSens = 1;
            rasb_opt::Forcesens = false;
            sensitivity_memory::MemVal = 1;
            return -1;
        }
    }
    long long* INT_REV_SEEDS = new long long[NO_SEEDS];
    for (i = 0; i <= NO_SEEDS - 1; i++)
        INT_REV_SEEDS[i] = BIN_REVERSED_TO_INT2(SEEDS[i]); // !!! this works like * = 0

                                                           // create the tree of BS --- 1..NO_BS-1 *********************************************************
                                                           // BS[i][0] = the integer value of b^r (except for epsilon, any b starts with 1)
                                                           // BS[i][1] = index j in BS of left son: BS[j][0] = integer value of b^r0 = (0b)^r (-1 if it doesn't exist)
                                                           // BS[i][2] = index j in BS of right son: BS[j][0] = integer value of b^r1 = (1b)^r (-1 if it doesn't exist)
                                                           // BS[i][3] = (suffix link) index j in BS of (B(b^r))^r i.e. BS[j][0] = integer value of (B(b^r))^r
                                                           // BS[i][4] = 1 if b is a hit and 0 otherwise            //B(x) is the longest prefix of x that is in B
                                                           //B = set of compatible but not hits b's
                                                           // BS[i][5] = its level = the length of the string
                                                           // BS[i][6] = the longest prefix of 0b which reversed means the longest suffix followed by a 0

    long long MAX_NO_BS = NO_SEEDS;
    for (i = 0; i <= NO_SEEDS - 1; i++) {       // compute maximum possible no of b's
        tmp = 1;
        for (j = strlen(SEEDS[i]) - 1; j >= 0; j--) {
            if (SEEDS[i][j] != '1') tmp *= 2;
            MAX_NO_BS += tmp;        // add previous value if 1 in seed or double (tmp *= 2 above) if a * in seed
        }
    }
    long long NO_BS = MAX_NO_BS;
    //cout << NO_BS << endl;
    //bound for computing sensitivity (not allocate more than 120GB)
    if(!rasb_opt::Forcesens){
        bool stop = sensitivity_memory::check_memory();
        if(stop){
            rasb_opt::Sens = false;
            rasb_opt::OptSens = 1;
            rasb_opt::Forcesens = false;
            sensitivity_memory::MemVal = 1;
            return -1;
        }
    }
    long long **BS = new long long *[NO_BS];

    for (i = 0; i <= NO_BS - 1; i++) {
        if(!rasb_opt::Forcesens){
            bool stop = sensitivity_memory::check_memory();
            if(stop){
                rasb_opt::Sens = false;
                rasb_opt::OptSens = 1;
                rasb_opt::Forcesens = false;
                sensitivity_memory::MemVal = 1;
                return -1;
            }
        }       
        BS[i] = new long long[7];

        BS[i][0] = BS[i][3] = BS[i][4] = BS[i][5] = BS[i][6] = 0; // initialize
        BS[i][1] = BS[i][2] = -1;
    };
    // create the tree by levels: all b's of length i are on level i
    BS[0][0] = 0; //epsilon
    BS[0][1] = -1; //no left son since b=0 is not compatible -- seeds end with 1
    BS[0][2] = 1; //right son is BS[1][0] = 1
    BS[0][3] = 0; //suffix link to itself
    BS[0][4] = 0; //epsilon is not hit
    BS[0][5] = 0;
    BS[0][6] = 0;
    BS[1][0] = 1;
    BS[1][3] = 0;
    BS[1][4] = 0; // assume 1 is not a hit
    BS[1][5] = 1;
    prev_level_start = 1; prev_level_end = 1; // indices in BS between which previous level is found
    pos = 2; //first empty position in BS
    for (level = 2; level <= MAX_L; level++) { // complete level "level"
        for (i = prev_level_start; i <= prev_level_end; i++)
            if (BS[i][4] != 1) { // not a hit
                b = BS[i][0]; //integer value
                b_zero = 2 * b; // try b0
                compatible = 0; hit = 0;
                for (j = 0; j <= NO_SEEDS - 1; j++)     // check long enough seeds to seee if b0 is compat/hit
                    if (seed_length[j] >= level)
                        if (((INT_REV_SEEDS[j] >> (seed_length[j] - level)) & (~b_zero)) == 0) {
                            compatible = 1;
                            if (level == seed_length[j])
                                hit = 1;
                        }
                if (compatible) {
                    BS[i][1] = pos;
                    BS[pos][0] = 2 * b;
                    BS[pos][4] = hit; // hit = 1 if it is hit by a seed
                    BS[pos][5] = level;
                    suffix_link = BS[i][3];
                    while ((suffix_link != 0) && (BS[suffix_link][1] == -1)) {
                        suffix_link = BS[suffix_link][3];
                    }
                    if (suffix_link != 0) {
                        BS[pos][3] = BS[suffix_link][1];
                        if (BS[BS[pos][3]][4] == 1)     // if suffix link is hit then also itself is hit
                            BS[pos][4] = 1;
                    }
                    pos++;
                }
                b_one = 2 * b + 1; // try b1
                compatible = 0; hit = 0;
                for (j = 0; j <= NO_SEEDS - 1; j++)     // check all long enough seed to seee if b0 is compat/hit
                    if (seed_length[j] >= level)
                        if (((INT_REV_SEEDS[j] >> (seed_length[j] - level)) & (~b_one)) == 0) {
                            compatible = 1;
                            if (level == seed_length[j])
                                hit = 1;
                        }
                if (compatible) {
                    BS[i][2] = pos;
                    BS[pos][0] = 2 * b + 1;
                    BS[pos][4] = hit; // hit = 1 if it is hit by a seed
                    BS[pos][5] = level;
                    suffix_link = BS[i][3];
                    while (BS[suffix_link][2] == -1) {
                        suffix_link = BS[suffix_link][3];
                    }
                    BS[pos][3] = BS[suffix_link][2];
                    if (BS[BS[pos][3]][4] == 1)     // if suffix link is hit then also itself is hit
                        BS[pos][4] = 1;

                    pos++;
                }
            }
        prev_level_start = prev_level_end + 1;
        prev_level_end = pos - 1;
    }
    // zero_links -- longest suffix of b0 in the tree
    for (i = 1; i <= NO_BS - 1; i++)
        if (BS[i][1] != -1)     // has left son, that is, 0-son
            BS[i][6] = BS[i][1];
        else {
            zero_link = BS[i][3];
            while ((zero_link != 0) && (BS[zero_link][1] == -1)) {
                zero_link = BS[zero_link][3];
            }
            if (zero_link != 0)
                BS[i][6] = BS[zero_link][1];
        }
        // compute the f's  f[i][j] = probab to hit a prefix of length i that ends with INT_TO_BIN_REVERSED[BS[j][0]]
        double **f;
        if(!rasb_opt::Forcesens){
            bool stop = sensitivity_memory::check_memory();
            if(stop){
                rasb_opt::Sens = false;
                rasb_opt::OptSens = 1;
                rasb_opt::Forcesens = false;
                sensitivity_memory::MemVal = 1;
                return -1;
            }
        }
        f = new double*[N + 1];
    
        for (i = 0; i <= N; i++) {
            if(!rasb_opt::Forcesens){
                bool stop = sensitivity_memory::check_memory();
                if(stop){
                    rasb_opt::Sens = false;
                    rasb_opt::OptSens = 1;
                    rasb_opt::Forcesens = false;
                    sensitivity_memory::MemVal = 1;
                    return -1;
                }
            }
            f[i] = new double[NO_BS];

            for (j = 0; j <= NO_BS - 1; j++) f[i][j] = 0; //initialize
        }


        for (i = 0; i <= N; i++) {
            for (j = NO_BS - 1; j >= 0; j--) {
                if (i == 0) f[i][j] = 0;                    // empty prefix of random region cannot be hit
                else if (i < BS[j][5]) f[i][j] = 0;     // too short
                else if (BS[j][4] == 1) f[i][j] = 1;    // hit
                else {
                    new_i = i - BS[j][5] + BS[BS[j][6]][5] - 1;
                    if (new_i < 0) new_i = 0;
                    f0 = f[new_i][BS[j][6]];
                    if (BS[j][2] < 0)
                        f1 = 1;
                    else
                        f1 = f[i][BS[j][2]];
                    f[i][j] = (1 - P)*f0 + P*f1;
                }
            }
        }
        double result = f[N][0];
        // free memory
        delete[] seed_length; delete[] INT_REV_SEEDS;
        for (i = 0; i <= N; i++)
            delete[] f[i];
        delete[] f;
        for (i = 0; i <= MAX_NO_BS - 1; i++)
            delete[] BS[i];
        delete[] BS;

        return(result);
}
