#include "pattern.hpp"

pattern::pattern(uint64_t Idx):  _Score(1), _BitPattern(0), _Idx(Idx), _IsBit(false){
}

pattern::pattern(const pattern & Pat){
    _VectorPattern = Pat._VectorPattern;
    _MatchPos = Pat._MatchPos;
    _DCPos = Pat._DCPos;
    _Score = Pat._Score;
    _BitPattern = Pat._BitPattern;
    _Idx = Pat._Idx;
    _IsBit = Pat._IsBit;
}

pattern::pattern(unsigned Weight, unsigned DontCare, unsigned Idx): _Score(1), _BitPattern(0), _Idx(Idx),_IsBit(false){
    random(DontCare, Weight);
}

pattern::pattern(unsigned Weight, unsigned DontCare, uint64_t Seed, unsigned Idx): _Score(1), _BitPattern(0), _Idx(Idx), _IsBit(false){
    random(DontCare, Weight, Seed);
}

pattern::pattern(std::string & StrPat) : _Score(1), _BitPattern(0),_IsBit(false){
    _VectorPattern = std::vector<unsigned char>(StrPat.begin(), StrPat.end());
    _check_pattern();
}


pattern::pattern(std::string && StrPat) : _Score(1), _BitPattern(0),_IsBit(false){
    _VectorPattern = std::vector<unsigned char>(StrPat.begin(), StrPat.end());
    _check_pattern();
}


pattern::pattern(std::vector<char> & StrPat) : _Score(1), _BitPattern(0),_IsBit(false){
    _VectorPattern = std::vector<unsigned char>(StrPat.begin(), StrPat.end());
    _check_pattern();
}


pattern::pattern(std::vector<unsigned char> & StrPat) : _Score(1), _BitPattern(0),_IsBit(false){
    _VectorPattern = std::vector<unsigned char>(StrPat.begin(), StrPat.end());
    _check_pattern();
}

void pattern::push_back(std::string && Chars){
    push_back(Chars);
}

void pattern::push_back(std::string & Chars){
    for(unsigned char C : Chars){
        push_back(C);
    }
}

void pattern::push_back(char C){
    push_back((unsigned char)C);
}

void pattern::push_back(unsigned char C){
    switch(C){
        case '1':
        case 'X':
        case 'x':
        case '#':
        case '*':
            C = 1;
            break;
        case '0':
        case 'O':
        case 'o':
        case '-':
            C = 0;
            break;
    }
    if(C != 1 && C != 0){
        std::cerr << "Illegal character in pattern!\nFormat: match = {1,X,x,#,*,} | don't care = {0,O,o,-,}" << std::endl;
        std::exit(-1);
    }
    _VectorPattern.push_back(C);
    if(C == 1){
        _MatchPos.push_back(_VectorPattern.size()-1);
    }
    else{
        _DCPos.push_back(_VectorPattern.size()-1);
    }
    _BitPattern <<= 1;
    _BitPattern |= C;
}

void pattern::_check_pattern(){
    unsigned Ctr = 0;

    if(_VectorPattern.size() < 64){
        _IsBit = true;
    }

    for(auto & C : _VectorPattern){
        switch(C){
            case '1':
            case 'X':
            case 'x':
            case '#':
            case '*':
                C = 1;
                break;
            case '0':
            case 'O':
            case 'o':
            case '-':
                C = 0;
                break;
        }
        if(C != 1 && C != 0){
            std::cerr << "Illegal character in pattern!\nFormat: match = {1,X,x,#,*,} | don't care = {0,O,o,-,}" << std::endl;
            std::exit(-1);
        }
        if(C == 1){
            _MatchPos.push_back(Ctr);
        }
        else{
            _DCPos.push_back(Ctr);
        }
        _BitPattern <<= 1;
        _BitPattern |= C;
        Ctr++;
    }
}

void pattern::random(unsigned DontCare, unsigned Weight){
    std::random_device RandomBit;
    random(DontCare, Weight, RandomBit());
}

void pattern::random(unsigned DontCare, unsigned Weight, uint64_t Seed){
    unsigned Length = DontCare + Weight;
    if(Length < 64){
        _IsBit = true;
    }
    if(Weight < 2){
        std::cerr << "Illegal value for weight!\nMinimum allowed weight is 2!" << std::endl;
        std::exit(-1);
    }
    std::default_random_engine BitRandom(Seed);
    std::mt19937 BitGenerator(BitRandom());
    std::uniform_int_distribution<unsigned> BitPosition(1, Length-2);

    _MatchPos.clear();
    _DCPos.clear();
    _BitPattern = 0;
    _VectorPattern = std::vector<unsigned char>(Length,0);
    
    _VectorPattern[0] = 1;
    _VectorPattern[Length-1] = 1;
    _BitPattern = ((uint64_t) 1 << (Length-1)) | (uint64_t)1;
    Weight -= 2;

    while(Weight > 0){
        unsigned Pos = BitPosition(BitGenerator);
        if(_VectorPattern[Pos] == 0){
            Weight--;
            _VectorPattern[Pos] = 1;
            _BitPattern |= (uint64_t)1 << (Length -1 - Pos);
        }
    }
    for(unsigned i = 0; i < _VectorPattern.size(); i++){
        if(_VectorPattern[i] == 1){
            _MatchPos.push_back(i);
        }
        else{
            _DCPos.push_back(i);
        }
    }
}

std::string pattern::to_string() const{
    std::string Pattern(_VectorPattern.begin(), _VectorPattern.end());
    std::transform(Pattern.begin(), Pattern.end(), Pattern.begin(),[](unsigned char C) { return (C+'0'); });
    return Pattern;
}


bool pattern::is_match(unsigned Pos) const{
    return _VectorPattern[Pos] == 1;
}

std::vector<unsigned> pattern::match_pos() const{
    return _MatchPos;
}

std::vector<unsigned> pattern::dc_pos() const{
    return _DCPos;
}

void pattern::bit_swap(unsigned PosA, unsigned PosB){
    std::swap(_VectorPattern[PosA],_VectorPattern[PosB]);
    if(_IsBit && _VectorPattern[PosA] !=_VectorPattern[PosB]){
        _BitPattern ^= ((uint64_t)1 << (_VectorPattern.size()-PosA-1)) | ((uint64_t)1 << (_VectorPattern.size()-PosB-1));
            if(_VectorPattern[PosA] != 0){
                std::swap(PosA,PosB);
            }
            _MatchPos.erase(std::find(_MatchPos.begin(), _MatchPos.end(), PosA));
            _DCPos.erase(std::find(_DCPos.begin(), _DCPos.end(), PosB));
            _MatchPos.push_back(PosB);
            _DCPos.push_back(PosA);
            std::sort(_MatchPos.begin(), _MatchPos.end());
            std::sort(_DCPos.begin(), _DCPos.end());
    }
}

void pattern::random_swap(){
    std::random_device RandomBit;
    random_swap(RandomBit());
}

void pattern::random_swap(uint64_t Seed){
    if(_DCPos.size() != 0){
        std::default_random_engine BitRandom(Seed);
        std::mt19937 BitGenerator(BitRandom());
        std::uniform_int_distribution<unsigned> SwapPosition(1, _MatchPos.size()-2);
        unsigned MatchPosition = _MatchPos[SwapPosition(BitGenerator)];
        SwapPosition = std::uniform_int_distribution<unsigned>(0, _DCPos.size()-1);
        unsigned DCPosition = _DCPos[SwapPosition(BitGenerator)];
        bit_swap(MatchPosition, DCPosition);
    }
}


unsigned char pattern::operator[](size_t Idx) const{
    return _VectorPattern[Idx];
}

unsigned char & pattern::operator[](size_t Idx){
    return _VectorPattern[Idx];
}

bool pattern::operator<(const pattern & P) const{
    return _Score < P._Score;
}

bool pattern::operator>(const pattern & P) const{
    return _Score > P._Score;
}

bool pattern::operator==(const pattern & P) const{
    if(_IsBit){
        return _BitPattern == P._BitPattern;
    }
    return _VectorPattern == P._VectorPattern;
}
bool pattern::operator!=(const pattern & P) const{
    if(_IsBit){
        return _BitPattern != P._BitPattern;
    }
    return _VectorPattern != P._VectorPattern;
}


double pattern::score() const{
    return _Score;
}

uint64_t pattern::bits() const{
    return _BitPattern;
}

unsigned pattern::weight() const{
    return _MatchPos.size();
}

unsigned pattern::length() const{
    return _VectorPattern.size();
}
unsigned pattern::dontcare() const{
    return _VectorPattern.size() - _MatchPos.size();
}

unsigned pattern::idx() const{
    return _Idx;
}

void pattern::set_score(double Scr){
    _Score = Scr;
}

void pattern::set_idx(unsigned Idx){
    _Idx = Idx;
}


unsigned pattern::get_overlap(const pattern & P, int OldShift) const{
    unsigned Overlap = 0;
    int Shift =  (int)length() - ((int)P.length()+OldShift);
    if(P._IsBit && _IsBit){
        uint64_t ShiftPattern = bits();
        uint64_t BitPat = P.bits();
        if(Shift < 0){
            std::swap(ShiftPattern, BitPat);
            Shift = std::abs(Shift);
        }
        ShiftPattern >>= Shift;
        ShiftPattern &= BitPat;
        unsigned ShiftSize = std::log2(ShiftPattern)+1;
        for(unsigned i = 0; i < ShiftSize; i++){
            Overlap += (ShiftPattern & (uint64_t)1);
            ShiftPattern >>= 1;
        }
    }
    else{
        auto PatAIt = _VectorPattern.begin();
        auto PatBIt = P._VectorPattern.begin();
        Shift += (int)P._VectorPattern.size()-(int)_VectorPattern.size();
        if(Shift > 0){
            PatBIt += Shift;
        }
        else{
            PatAIt -= Shift;
        }
        for(;PatAIt != _VectorPattern.end() && PatBIt != P._VectorPattern.end(); PatBIt++, PatAIt++){
            Overlap += *PatAIt * *PatBIt;
        }
    }
    return Overlap;
}

pattern::iterator pattern::begin(){
    return _VectorPattern.begin();
}

pattern::iterator pattern::end(){
    return _VectorPattern.end();
}