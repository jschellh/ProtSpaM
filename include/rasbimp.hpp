#ifndef RASBIMP_HPP_
#define RASBIMP_HPP_

#include "rasbhari.hpp"
#include "rasbopt.hpp"

namespace rasb_implement{
    rasbhari hillclimb_var(unsigned Size = rasb_opt::Size, unsigned Weight = rasb_opt::Weight, unsigned MinDontCare = rasb_opt::MinDontcare, unsigned MaxDontCare = rasb_opt::MaxDontcare);
    rasbhari hillclimb_oc(unsigned Size = rasb_opt::Size, unsigned Weight = rasb_opt::Weight, unsigned MinDontCare = rasb_opt::MinDontcare, unsigned MaxDontCare = rasb_opt::MaxDontcare);
    rasbhari hillclimb_var_iterative(unsigned Size = rasb_opt::Size, unsigned Weight = rasb_opt::Weight, unsigned MinDontCare = rasb_opt::MinDontcare, unsigned MaxDontCare = rasb_opt::MaxDontcare);
    rasbhari hillclimb_oc_iterative(unsigned Size = rasb_opt::Size, unsigned Weight = rasb_opt::Weight, unsigned MinDontCare = rasb_opt::MinDontcare, unsigned MaxDontCare = rasb_opt::MaxDontcare);
    rasbhari hillclimb_sens_var(unsigned Size = rasb_opt::Size, unsigned Weight = rasb_opt::Weight, unsigned MinDontCare = rasb_opt::MinDontcare, unsigned MaxDontCare = rasb_opt::MaxDontcare);
    rasbhari hillclimb_sens_oc(unsigned Size = rasb_opt::Size, unsigned Weight = rasb_opt::Weight, unsigned MinDontCare = rasb_opt::MinDontcare, unsigned MaxDontCare = rasb_opt::MaxDontcare);
    
    rasbhari _hillclimb(unsigned Size, unsigned Weight, unsigned MinDontCare, unsigned MaxDontCare);
    rasbhari _hillclimb_iterative(unsigned Size, unsigned Weight, unsigned MinDontCare, unsigned MaxDontCare);
    rasbhari _hillclimb_sens(unsigned Size, unsigned Weight, unsigned MinDontCare, unsigned MaxDontCare);
};

#endif