#include "rasbhari.hpp"

rasbhari::rasbhari():_RasbhariScore(0),_RasbhariSensitivity(-1), _PatNo(0){
}

rasbhari::rasbhari(const rasbhari &RasbObj){
    _CoefMat = RasbObj._CoefMat;
    _PatternList = RasbObj._PatternList;
    _RasbhariPattern = RasbObj._RasbhariPattern;
    _RasbhariScore = RasbObj._RasbhariScore;
    _RasbhariSensitivity = RasbObj._RasbhariSensitivity;
    _PatNo = RasbObj._PatNo;
}

rasbhari::rasbhari(unsigned Size, unsigned Weight, unsigned DontCare):_RasbhariScore(0),_RasbhariSensitivity(-1), _PatNo(0){
    _RasbhariPattern = patternset(Size,Weight,DontCare,true);
    _make_pattern_list();
    _adjust_coef_mat();
    calculate();
    calculate_sensitivity();
}
rasbhari::rasbhari(unsigned Size, unsigned Weight, unsigned MinDontCare, unsigned MaxDontCare):_RasbhariScore(0),_RasbhariSensitivity(-1), _PatNo(0){
    _RasbhariPattern = patternset(Size,Weight,Weight,MinDontCare,MaxDontCare,true);
    _make_pattern_list();
    _adjust_coef_mat();
    calculate();
    calculate_sensitivity();
}
rasbhari::rasbhari(std::string PatternFile):_RasbhariScore(0),_RasbhariSensitivity(-1), _PatNo(0){
    _RasbhariPattern = patternset(PatternFile);
    _make_pattern_list();
    _adjust_coef_mat();
    calculate();
    calculate_sensitivity();
}


void rasbhari::push_back(pattern && Pat){
    push_back(Pat);
}

void rasbhari::push_back(pattern & Pat){
    _PatternList.push_back(Pat);
    _RasbhariPattern.push_back(Pat);
    _RasbhariPattern[_RasbhariPattern.size()-1].set_idx(_RasbhariPattern.size()-1);
    _PatternList[_PatternList.size()-1].set_idx(_PatternList.size()-1);
    _adjust_coef_mat();
    _RasbhariScore = -1;
    _RasbhariSensitivity = -1;
}

void rasbhari::_adjust_coef_mat(){
    _CoefMat = std::vector< std::vector<double> >(_RasbhariPattern.size(),std::vector<double>(_RasbhariPattern.size(),0));
}


void rasbhari::_make_pattern_list(){
    _PatternList = std::vector<pattern>(_RasbhariPattern.size());
    for(unsigned i = 0; i < _RasbhariPattern.size(); i++){
        _RasbhariPattern[i].set_idx(i);
        _PatternList[i] = _RasbhariPattern[i];
    }
}


void rasbhari::calculate(){
    for(unsigned i = 0; i < _PatternList.size(); i++){
        double CoefPat = 0;
        for(unsigned j = 0; j < _PatternList.size(); j++){
            _CoefMat[i][j] = calculate_pair(i,j);
            CoefPat += _CoefMat[i][j];
        }
        _PatternList[i].set_score(CoefPat);
    }
    _RasbhariScore = 0;
    for(unsigned i = 0; i < _PatternList.size(); i++){
        for(unsigned j = i; j < _PatternList.size(); j++){
            _RasbhariScore += _CoefMat[i][j];
        }
    }
    for(auto & Pat : _RasbhariPattern){
        Pat.set_score(_PatternList[Pat.idx()].score());
    }
}

double rasbhari::calculate_pair(size_t Idx1, size_t Idx2){
    if(rasb_opt::Oc){
        return rasbhari_compute::pair_coef_oc(_PatternList[Idx1],_PatternList[Idx2]);
    }
    if(Idx1 == Idx2){
        return rasbhari_compute::pair_coef_var_sym(_PatternList[Idx1],_PatternList[Idx2]);
    }
    return rasbhari_compute::pair_coef_var(_PatternList[Idx1],_PatternList[Idx2]);    
}

void rasbhari::calculate_sensitivity(){
    if(rasb_opt::Sens){
        _RasbhariSensitivity = rasbhari_compute::sensitivity(_RasbhariPattern);
    }
}

void rasbhari::update(unsigned Idx){
    double CoefPat = 0;
    for(unsigned i = 0; i < _PatternList.size(); i++){
        _RasbhariScore -= _CoefMat[i][Idx];
        double PatternScore = _PatternList[i].score() - _CoefMat[i][Idx];
        _CoefMat[i][Idx] = calculate_pair(i,Idx);
        _CoefMat[Idx][i] = _CoefMat[i][Idx];
        CoefPat += _CoefMat[i][Idx];
        _PatternList[i].set_score(PatternScore+_CoefMat[i][Idx]);
        _RasbhariScore += _CoefMat[i][Idx];
    }
    _PatternList[Idx].set_score(CoefPat);
    for(auto & Pat : _RasbhariPattern){
        Pat.set_score(_PatternList[Pat.idx()].score());
    }
}

bool rasbhari::climb_hill(){
    unsigned PatIdx = _PatNo%_RasbhariPattern.size();
    unsigned OrigIdx = _RasbhariPattern[PatIdx].idx();

    double LastCoef = _RasbhariScore;
    pattern LastPattern = _RasbhariPattern[PatIdx];
    std::vector< std::vector<double> > LastCMat = _CoefMat;
    std::vector<pattern> LastPatList = _PatternList;

    _RasbhariPattern.random_swap_uniq(PatIdx);
    _PatternList[OrigIdx] = _RasbhariPattern[PatIdx];
    update(OrigIdx);
    
    if(_RasbhariScore < LastCoef){
        _PatNo = 0;
        _RasbhariPattern.sort();
        return true;
    }

    std::swap(_RasbhariPattern[PatIdx], LastPattern);
    std::swap(_CoefMat,LastCMat);
    std::swap(_PatternList,LastPatList);
    for(auto & Pat : _RasbhariPattern){
        Pat.set_score(_PatternList[Pat.idx()].score());
    }
    _RasbhariScore = LastCoef;
    _PatNo++;
    return false;
}

bool rasbhari::hill_climbing(unsigned Limit){
    double ScoreBest = _RasbhariScore, InitialScore = _RasbhariScore;
    _PatNo = 0;
    unsigned ModeSave = rasb_opt::ImproveMode, Ctr = 0;
    if(rasb_opt::ImproveMode < 1){
        rasb_opt::ImproveMode = 1;
    }
    for(unsigned i = 0; i < Limit; i++){
        if(!rasb_opt::Silent && rasb_opt::ImproveMode == 1){
            std::cout << "\rStep " << i << "/" << Limit << "  Improvement +" << Ctr;
            std::cout.flush();
        }
        climb_hill();
        if(ScoreBest > _RasbhariScore && !rasb_opt::Silent && rasb_opt::ImproveMode == 1){
            ScoreBest = _RasbhariScore;
            Ctr++;
            if(!rasb_opt::Silent && !rasb_opt::Quiet && rasb_opt::ImproveMode == 1){
                std::cout << std::endl;
                print();
            }
        }
    }
    if(!rasb_opt::Silent && rasb_opt::ImproveMode == 1){
        std::cout << "\r"<< std::string(80,' ') << "\r";
        std::cout << "Number of improvements: " << Ctr << std::endl << std::endl;
    }
    rasb_opt::ImproveMode = ModeSave;
    return InitialScore > _RasbhariScore;
}   

bool rasbhari::iterate_hill_climbing(unsigned Limit, unsigned Iteration){
    patternset HillClimbBest = _RasbhariPattern;
    std::vector<pattern> PatListBest = _PatternList;
    std::vector< std::vector<double> > CoefBest = _CoefMat;
    double ScoreBest = _RasbhariScore, InitialScore = ScoreBest;
    unsigned ModeSave = rasb_opt::ImproveMode, Ctr = 0;
    if(rasb_opt::ImproveMode < 2){
        rasb_opt::ImproveMode = 2;
    }
    for(unsigned i = 0; i < Iteration; i++){
        if(!rasb_opt::Silent && rasb_opt::ImproveMode == 2){
            std::cout << "\rStep " << i << "/" << Iteration << "  Improvement +" << Ctr;
            std::cout.flush();
        }
        _RasbhariPattern = patternset(rasb_opt::Size,rasb_opt::Weight,rasb_opt::Weight,rasb_opt::MinDontcare,rasb_opt::MaxDontcare,true);
        _make_pattern_list();
        _adjust_coef_mat();
        calculate();
        hill_climbing(Limit);
        if(ScoreBest > _RasbhariScore){
            Ctr++;
            if(!rasb_opt::Silent && !rasb_opt::Quiet && rasb_opt::ImproveMode == 2){
                std::cout << std::endl;
                print();
            }
            ScoreBest = _RasbhariScore;
            std::swap(HillClimbBest,_RasbhariPattern);
            std::swap(PatListBest,_PatternList);
            std::swap(CoefBest,_CoefMat);
        }
    }
    if(!rasb_opt::Silent && rasb_opt::ImproveMode == 2){
        std::cout << "\r"<<  std::string(80,' ') << "\r";
        std::cout << "Number of improvements: " << Ctr << std::endl << std::endl;
    }
    rasb_opt::ImproveMode = ModeSave;
    _RasbhariScore = ScoreBest;
    std::swap(_RasbhariPattern,HillClimbBest);
    std::swap(_PatternList,PatListBest);
    std::swap(_CoefMat,CoefBest);
    return InitialScore > _RasbhariScore;
}

bool rasbhari::climb_hill_sensitivity(unsigned Limit, unsigned Iteration, bool InitSens){
    if(InitSens){
        calculate_sensitivity();
    }
    double SensBest = _RasbhariSensitivity;
    iterate_hill_climbing(Limit,Iteration);
    calculate_sensitivity();
    return SensBest < _RasbhariSensitivity;
}

bool rasbhari::hill_climbing_sensitivity(unsigned Limit, unsigned Iteration, unsigned Loop){
    patternset HillClimbBest = _RasbhariPattern;
    std::vector<pattern> PatListBest = _PatternList;
    std::vector< std::vector<double> > CoefBest = _CoefMat;
    double SensBest = _RasbhariSensitivity, InitialSens = _RasbhariSensitivity;
    unsigned Ctr = 0;
    unsigned ModeSave = rasb_opt::ImproveMode;
    if(rasb_opt::ImproveMode < 3){
        rasb_opt::ImproveMode = 3;
    }
    for(unsigned i = 0; i < Loop; i++){
        if(!rasb_opt::Silent && rasb_opt::ImproveMode == 3){
            std::cout << "\rStep " << i << "/" << Loop << "  Improvement +" << Ctr;
            std::cout.flush();
        }
        _RasbhariPattern = patternset(rasb_opt::Size,rasb_opt::Weight,rasb_opt::Weight,rasb_opt::MinDontcare,rasb_opt::MaxDontcare,true);
        _make_pattern_list();
        _adjust_coef_mat();
        calculate();
        climb_hill_sensitivity(Limit, Iteration);
        if(SensBest < _RasbhariSensitivity){
            Ctr++;
            if(!rasb_opt::Silent && !rasb_opt::Quiet && rasb_opt::ImproveMode == 3){
                std::cout << std::endl;
                print();
            }
            SensBest = _RasbhariSensitivity;
            std::swap(HillClimbBest,_RasbhariPattern);
            std::swap(PatListBest,_PatternList);
            std::swap(CoefBest,_CoefMat);
        }
    }
    if(!rasb_opt::Silent && rasb_opt::ImproveMode == 3){
        std::cout << "\r" << std::string(80,' ') << "\r";
        std::cout << "Number of improvements: " << Ctr << std::endl << std::endl;
    }
    rasb_opt::ImproveMode = ModeSave;
    _RasbhariSensitivity = SensBest;
    std::swap(_RasbhariPattern,HillClimbBest);
    std::swap(_PatternList,PatListBest);
    std::swap(_CoefMat,CoefBest);
    return InitialSens < _RasbhariSensitivity;
}

patternset rasbhari::pattern_set() const{
    return _RasbhariPattern;
}
patternset & rasbhari::pattern_set(){
    return _RasbhariPattern;
}

double rasbhari::score() const{
    return _RasbhariScore;
}

double rasbhari::sensitivity() const{
    return _RasbhariSensitivity;
}

void rasbhari::print(){
    for(auto Pat : _RasbhariPattern){
        std::cout << Pat.idx() << " " << Pat.to_string() << std::endl;
    }
    std::cout << "rasbhari coefficient        : " << _RasbhariScore << std::endl;
    std::cout << "rasbhari coefficient (norm) : " << _RasbhariScore / ((_RasbhariPattern.size()*(_RasbhariPattern.size()+1))/(double)2) << std::endl;
    if(rasb_opt::Sens){
        std::cout << "rasbhari sensitivity        : " << _RasbhariSensitivity << std::endl;
    }
    std::cout << std::endl;
    std::cout.flush();
}

void rasbhari::to_file(std::string OutFile){
    if(OutFile.size() == 0){
        return;
    }
    _RasbhariPattern.to_file(OutFile);
    std::ofstream Output(OutFile, std::ios::app);
    Output << "#rasbhari coefficient        : " << _RasbhariScore << std::endl;
    Output << "#rasbhari coefficient (norm) : " << _RasbhariScore / ((_RasbhariPattern.size()*(_RasbhariPattern.size()+1))/(double)2) << std::endl;
    if(rasb_opt::Sens){
        Output << "#rasbhari sensitivity        : " << _RasbhariSensitivity << std::endl;
    }
    Output.close();
    if(!rasb_opt::Silent){
        std::cout << "rasbhari set written to file:\n    '" << OutFile << "'" << std::endl;
    }
}


rasbhari::iterator rasbhari::begin(){
    return _PatternList.begin();
}

rasbhari::iterator rasbhari::end(){
    return _PatternList.end();
}


void rasbhari::_debug(){
    std::cout << "RasbPat Score Idx\tListPat Score Idx\t\tpairwise idx identic?" << std::endl;
    for(unsigned i = 0; i < _RasbhariPattern.size(); i++){
        std::cout << _RasbhariPattern[i].to_string() << " " << _RasbhariPattern[i].score() << " " << _RasbhariPattern[i].idx() << " \t";
        std::cout << _PatternList[i].to_string() << " " << _PatternList[i].score() << " " << _PatternList[i].idx() << std::endl;
    }
    std::cout << std::endl;
    std::cout << "Coef Mat " << _RasbhariPattern.size() << "x" << _RasbhariPattern.size() << "\tSymmetric?" << std::endl;
    for(unsigned i = 0; i < _PatternList.size(); i++){
        for(unsigned j = 0; j < _PatternList.size(); j++){
            std::cout << _CoefMat[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
    std::cout << "_RasbhariScore: " << _RasbhariScore/((rasb_opt::Size*(rasb_opt::Size+1))/2) << std::endl;
    std::cout << "_RasbhariSensitivity: " << _RasbhariSensitivity << std::endl << std::endl << std::endl;
}