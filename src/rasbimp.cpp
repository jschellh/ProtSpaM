#include "rasbimp.hpp"
rasbhari rasb_implement::hillclimb_var(unsigned Size, unsigned Weight, unsigned MinDontCare, unsigned MaxDontCare){
    rasb_opt::Oc = false;
    return _hillclimb(Size, Weight, MinDontCare, MaxDontCare);
}


rasbhari rasb_implement::hillclimb_oc(unsigned Size, unsigned Weight, unsigned MinDontCare, unsigned MaxDontCare){
    rasb_opt::Oc = true;
    return _hillclimb(Size, Weight, MinDontCare, MaxDontCare);
}


rasbhari rasb_implement::hillclimb_oc_iterative(unsigned Size, unsigned Weight, unsigned MinDontCare, unsigned MaxDontCare){
    rasb_opt::Oc = true;
    return _hillclimb_iterative(Size,Weight,MinDontCare,MaxDontCare);
}


rasbhari rasb_implement::hillclimb_var_iterative(unsigned Size, unsigned Weight, unsigned MinDontCare, unsigned MaxDontCare){
    rasb_opt::Oc = false;
    return _hillclimb_iterative(Size,Weight,MinDontCare,MaxDontCare);
}


rasbhari rasb_implement::hillclimb_sens_oc(unsigned Size, unsigned Weight, unsigned MinDontCare, unsigned MaxDontCare){
    rasb_opt::Oc = true;
    return _hillclimb_sens(Size,Weight,MinDontCare,MaxDontCare);
}


rasbhari rasb_implement::hillclimb_sens_var(unsigned Size, unsigned Weight, unsigned MinDontCare, unsigned MaxDontCare){
    rasb_opt::Oc = false;
    return _hillclimb_sens(Size,Weight,MinDontCare,MaxDontCare);
}


rasbhari rasb_implement::_hillclimb(unsigned Size, unsigned Weight, unsigned MinDontCare, unsigned MaxDontCare){
    rasb_opt::ImproveMode = 1;
    rasbhari RasbSet;
    if(rasb_opt::InFile.size() != 0){
        RasbSet = rasbhari(rasb_opt::InFile);
        rasb_opt::Size = RasbSet.pattern_set().size();
        rasb_opt::Weight = RasbSet.pattern_set().max_weight();
        rasb_opt::MinDontcare = RasbSet.pattern_set().min_dontcare();
        rasb_opt::MaxDontcare = RasbSet.pattern_set().max_dontcare();
    }
    else{
        RasbSet = rasbhari(Size,Weight,MinDontCare,MaxDontCare);
    }
    unsigned FillSizeBegin = 0, FillSizeEnd = 0;
    if(MaxDontCare + Weight > 13){
        FillSizeBegin = (MaxDontCare+Weight-13)/2;
        FillSizeEnd = FillSizeBegin;
        if((MaxDontCare+Weight-13)%2 != 0){
            FillSizeEnd++;
        }
    }
    if(!rasb_opt::Silent){
        std::cout << " #" << std::string(FillSizeBegin,'=') << " Initial Set " << std::string(FillSizeEnd,'=') << "#" << std::endl;
        RasbSet.print();
    }
    RasbSet.hill_climbing();
    if(MaxDontCare + Weight > 15){
        FillSizeBegin = (MaxDontCare+Weight-15)/2;
        FillSizeEnd = FillSizeBegin;
        if((MaxDontCare+Weight-15)%2 != 0){
            FillSizeEnd++;
        }
    }
    if(!rasb_opt::Silent){
        std::cout << " #" << std::string(FillSizeBegin,'=') << " Optimised Set " << std::string(FillSizeEnd,'=') << "#" << std::endl;
        RasbSet.print();
    }
    return RasbSet;
}

rasbhari rasb_implement::_hillclimb_iterative(unsigned Size, unsigned Weight, unsigned MinDontCare, unsigned MaxDontCare){
    rasb_opt::ImproveMode = 2;
    rasbhari RasbSet;
    if(rasb_opt::InFile.size() != 0){
        RasbSet = rasbhari(rasb_opt::InFile);
        rasb_opt::Size = RasbSet.pattern_set().size();
        rasb_opt::Weight = RasbSet.pattern_set().max_weight();
        rasb_opt::MinDontcare = RasbSet.pattern_set().min_dontcare();
        rasb_opt::MaxDontcare = RasbSet.pattern_set().max_dontcare();
    }
    else{
        RasbSet = rasbhari(Size,Weight,MinDontCare,MaxDontCare);
    }
    unsigned FillSizeBegin = 0, FillSizeEnd = 0;
    if(MaxDontCare + Weight > 13){
        FillSizeBegin = (MaxDontCare+Weight-13)/2;
        FillSizeEnd = FillSizeBegin;
        if((MaxDontCare+Weight-13)%2 != 0){
            FillSizeEnd++;
        }
    }
    if(!rasb_opt::Silent){
        std::cout << " #" << std::string(FillSizeBegin,'=') << " Initial Set " << std::string(FillSizeEnd,'=') << "#" << std::endl;
        RasbSet.print();
    }
    RasbSet.iterate_hill_climbing();
    if(MaxDontCare + Weight > 15){
        FillSizeBegin = (MaxDontCare+Weight-15)/2;
        FillSizeEnd = FillSizeBegin;
        if((MaxDontCare+Weight-15)%2 != 0){
            FillSizeEnd++;
        }
    }
    if(!rasb_opt::Silent){
        std::cout << " #" << std::string(FillSizeBegin,'=') << " Optimised Set " << std::string(FillSizeEnd,'=') << "#" << std::endl;
        RasbSet.print();
    }
    return RasbSet;
}

rasbhari rasb_implement::_hillclimb_sens(unsigned Size, unsigned Weight, unsigned MinDontCare, unsigned MaxDontCare){
    rasb_opt::ImproveMode = 3;
    rasbhari RasbSet;
    if(rasb_opt::InFile.size() != 0){
        RasbSet = rasbhari(rasb_opt::InFile);
        rasb_opt::Size = RasbSet.pattern_set().size();
        rasb_opt::Weight = RasbSet.pattern_set().max_weight();
        rasb_opt::MinDontcare = RasbSet.pattern_set().min_dontcare();
        rasb_opt::MaxDontcare = RasbSet.pattern_set().max_dontcare();
    }
    else{
        RasbSet = rasbhari(Size,Weight,MinDontCare,MaxDontCare);
    }
    unsigned FillSizeBegin = 0, FillSizeEnd = 0;
    if(MaxDontCare + Weight > 13){
        FillSizeBegin = (MaxDontCare+Weight-13)/2;
        FillSizeEnd = FillSizeBegin;
        if((MaxDontCare+Weight-13)%2 != 0){
            FillSizeEnd++;
        }
    }
    if(!rasb_opt::Silent){
        std::cout << " #" << std::string(FillSizeBegin,'=') << " Initial Set " << std::string(FillSizeEnd,'=') << "#" << std::endl;
        RasbSet.print();
    }
    if(MaxDontCare + Weight > 15){
        FillSizeBegin = (MaxDontCare+Weight-15)/2;
        FillSizeEnd = FillSizeBegin;
        if((MaxDontCare+Weight-15)%2 != 0){
            FillSizeEnd++;
        }
    }
    RasbSet.hill_climbing_sensitivity();
    if(!rasb_opt::Silent){
        std::cout << " #" << std::string(FillSizeBegin,'=') << " Optimised Set " << std::string(FillSizeEnd,'=') << "#" << std::endl;
        RasbSet.print();
    }
    return RasbSet;
}