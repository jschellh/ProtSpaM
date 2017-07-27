#ifndef SPEEDSENS_HPP_
#define SPEEDSENS_HPP_

#include <cstring>

#include "sensmem.hpp"
#include "rasbopt.hpp"

namespace speedsens{
    inline long long BIN_REVERSED_TO_INT2(char *s);
    double MULTIPLE_SENSITIVITY2(char** SEEDS, int NO_SEEDS, long long N, double P);
};
#endif