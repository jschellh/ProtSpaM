#ifndef RASBCOMP_HPP_
#define RASBCOMP_HPP_

#include <cstdint>
#include <string>
#include "pattern.hpp"
#include "patternset.hpp"
#include "rasbopt.hpp"
#include "speedsens.hpp"
#include "sensmem.hpp"

namespace rasbhari_compute{
    double pair_coef_var(pattern & Pat1, pattern & Pat2);
    double pair_coef_var_sym(pattern & Pat1, pattern & Pat2);
    double pair_coef_oc(pattern & Pat1, pattern & Pat2);
    double sensitivity(patternset & Pattern);
    extern bool DoOnce;
};
#endif