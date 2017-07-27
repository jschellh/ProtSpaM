#include "patternset.hpp"

patternset::patternset():_Score(1),_MinWeight(std::numeric_limits<unsigned>::max()),_MaxWeight(0),
    _MinDontCare(std::numeric_limits<unsigned>::max()),_MaxDontCare(0){    
}

patternset::patternset(const patternset & PatSet){
    _PatternSet = PatSet._PatternSet;
    _Score = PatSet._Score;
    _MinWeight = PatSet._MinWeight;
    _MaxWeight = PatSet._MaxWeight;
    _MinDontCare = PatSet._MinDontCare;
    _MaxDontCare = PatSet._MaxDontCare;
}

patternset::patternset(unsigned Numb, unsigned Weight, unsigned DontCare, bool Uniq):
    _Score(1),_MinWeight(Weight),_MaxWeight(Weight),_MinDontCare(DontCare),
    _MaxDontCare(DontCare){
    random(Numb, Weight, DontCare);
}

patternset::patternset(unsigned Numb, unsigned MinW, unsigned MaxW, 
    unsigned MinD, unsigned MaxD, bool Uniq):_Score(1),_MinWeight(MinW),
    _MaxWeight(MaxW),_MinDontCare(MinD),_MaxDontCare(MaxD){
    _adjust(_MinWeight,_MaxWeight);
    _adjust(_MinDontCare,_MaxDontCare);
    random(Numb, MaxW, MinW, MaxD, MinD);
}

patternset::patternset(std::string InputFile):_Score(1),_MinWeight(std::numeric_limits<unsigned>::max()),_MaxWeight(0),
    _MinDontCare(std::numeric_limits<unsigned>::max()),_MaxDontCare(0){
    std::ifstream Input(InputFile);
    assert(Input.is_open());
    while(!Input.eof()){
        std::string Line;
        std::getline(Input,Line);
        if(Line[0] != '#'){
            std::vector<std::string> Splits = _split(Line);
            for(auto & Str : Splits){
                push_back(Str);
            }
        }
    }
    Input.close();
}

void patternset::sort(){
    std::sort(_PatternSet.begin(), _PatternSet.end());
}

void patternset::push_back(pattern & Pat){
    _MaxDontCare = std::max(_MaxDontCare, Pat.dontcare());
    _MinDontCare = std::min(_MinDontCare, Pat.dontcare());
    _MaxWeight = std::max(_MaxWeight, Pat.weight());
    _MinWeight = std::min(_MinWeight, Pat.weight());
    _PatternSet.push_back(Pat);
}

void patternset::push_back(pattern && Pat){
    push_back(Pat);
}

void patternset::push_back(std::string & Pat){
    push_back(pattern(Pat));
}

void patternset::push_back(std::string && Pat){
    push_back(pattern(Pat));
}

void patternset::push_back(std::vector<char> & Pat){
    push_back(pattern(Pat));
}

void patternset::push_back(std::vector<unsigned char> & Pat){
    push_back(pattern(Pat));
}

void patternset::random(unsigned Size, unsigned Weight, unsigned DontCare, bool Uniq){
    random(Size, Weight, Weight, DontCare, DontCare, Uniq);
}

void patternset::random(unsigned Size, unsigned MinWeight, unsigned MaxWeight,
    unsigned DontCare, bool Uniq){
    random(Size, MinWeight, MaxWeight, DontCare, DontCare, Uniq);
}

void patternset::random(unsigned Size, unsigned MinWeight, unsigned MaxWeight,
    unsigned MinDontCare, unsigned MaxDontCare, bool Uniq){
    _PatternSet.clear();
    _adjust(MinWeight,MaxWeight);
    _adjust(MinDontCare,MaxDontCare);
    if(Size >= 2){
        _PatternSet.push_back(pattern(MinWeight,MinDontCare));
        double StepW = (MaxWeight - MinWeight + 1)/(double)(Size);
        double StepD = (MaxDontCare - MinDontCare + 1)/(double)(Size);
        double CurrentW = MinWeight + StepW;
        double CurrentD = MinDontCare + StepD;
        for(unsigned i = 0; i < Size - 2; i++){
            pattern Pat((unsigned)CurrentW, (unsigned)CurrentD,(uint64_t)i+1);
            while(is_uniq(Pat) == false){
                Pat.random((unsigned)CurrentW, (unsigned)CurrentD);
            }
            _PatternSet.push_back(Pat);
            CurrentW += StepW;
            CurrentD += StepD;
        }
        pattern Pat = pattern(MaxWeight,MaxDontCare,_PatternSet.size());
        while(is_uniq(Pat) == false){
            Pat.random(MaxWeight,MaxDontCare);
        }
        _PatternSet.push_back(Pat);
    }
    else{
        _PatternSet.push_back(pattern((MinWeight + MaxWeight)/2,(MinDontCare + MaxDontCare)/2));
    }
}

bool patternset::is_uniq(const pattern & Pat) const{
    return (std::find_if(_PatternSet.begin(),_PatternSet.end(), [& Pat](const pattern & P){
        if( & Pat != & P){
            return (Pat == P);
        }
        return false;
    }) == _PatternSet.end());
}


void patternset::random_swap_uniq(unsigned Idx){
    pattern PatSave = _PatternSet[Idx];
    for(unsigned i = 0; i < (size()+10)*(size()+10); i++){
        _PatternSet[Idx].random_swap();
        if(is_uniq(_PatternSet[Idx]) == true){
            return;
        }
        _PatternSet[Idx] = PatSave;
    }
}


void patternset::to_file(std::string OutFile){
    std::ofstream Output(OutFile);
    for(auto & Pat : _PatternSet){
        Output << Pat.to_string() << std::endl;
    }
    Output.close();
}


pattern patternset::operator[](size_t Idx) const{
    return _PatternSet[Idx];
}

pattern & patternset::operator[](size_t Idx){
    return _PatternSet[Idx];
}

bool patternset::operator<(const patternset & P) const{
    return _Score < P._Score;
}

bool patternset::operator>(const patternset & P) const{
    return _Score > P._Score;
}

unsigned patternset::max_weight() const{
    return _MaxWeight;
}
unsigned patternset::min_weight() const{
    return _MinWeight;
}
unsigned patternset::weight() const{
    unsigned MeanWeight = 0;
    for(auto & Pat : _PatternSet){
        MeanWeight += Pat.weight();
    }
    return MeanWeight/_PatternSet.size();
}

unsigned patternset::max_dontcare() const{
    return _MaxDontCare;
}
unsigned patternset::min_dontcare() const{
    return _MinDontCare;
}
unsigned patternset::dontcare() const{
    unsigned MeanDC = 0;
    for(auto & Pat : _PatternSet){
        MeanDC += Pat.dontcare();
    }
    return MeanDC/_PatternSet.size();
}

unsigned patternset::max_length() const{
    return _MaxWeight + _MaxDontCare;
}

unsigned patternset::min_length() const{
    return _MinWeight + _MinDontCare;
}
unsigned patternset::length() const{
    unsigned MeanLength = 0;
    for(auto & Pat : _PatternSet){
        MeanLength += Pat.length();
    }
    return MeanLength/_PatternSet.size();
}

unsigned patternset::size() const{
    return _PatternSet.size();
}

double patternset::score() const{
    return _Score;
}

void patternset::set_score(double Score){
    _Score = Score;
}


patternset::iterator patternset::begin(){
    return _PatternSet.begin();
}

patternset::iterator patternset::end(){
    return _PatternSet.end();
}

std::vector<std::string> patternset::_split(std::string &S){
    std::vector<std::string> Splits;
    std::string::iterator SPos = S.begin();
    for(auto Iter = S.begin(); Iter != S.end(); Iter++){
        switch(*Iter){
            case ';':
            case ',':
            case '.':
            case ' ':
            case '\t':
                std::string Tmp(SPos,Iter);
                if(Tmp.size() != 0){
                    Splits.push_back(Tmp);
                }
                SPos = Iter+1;
                break;
        }
    }
    std::string Tmp(SPos,S.end());
    if(Tmp.size() != 0){
        Splits.push_back(Tmp);
    }
    return Splits;
}