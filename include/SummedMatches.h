#ifndef SUMMEDMATCHES_H
#define SUMMEDMATCHES_H

#include <cstdint>
#include <vector>
#include <cmath>
#include "Match.h"

class SummedMatches {
public:

public:
    int score;
    int frequency;
    int mismatches;
    double dist;
    SummedMatches(int, int, int, int, int, int);
    static std::vector<SummedMatches> sumMatches (std::vector<Match>, int&);
    static double get_distance(std::vector<SummedMatches> vector, int threshold);
};
#endif //SUMMEDMATCHES_H
