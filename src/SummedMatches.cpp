#include "SummedMatches.h"

SummedMatches::SummedMatches(int scr, int freq, int mism, int totalMism, int totalFreq, int totalDC) {
    score = scr;
    frequency = freq;
    mismatches = mism;
    double mismatchRate = (mism + totalMism) / (double) ((freq + totalFreq) * totalDC);
    dist = -log(1 - mismatchRate - (0.2 * pow(mismatchRate,2.0) ) ); // kimura formula
}

std::vector<SummedMatches> SummedMatches::sumMatches(std::vector<Match> matches, int &dc) {
    std::vector<SummedMatches> out;
    int counts = 0;
    int last = INT32_MAX;
    int mismatches = 0;
    int total_mismatches = 0;
    int total_freq = 0;
    for (auto rit = matches.rbegin(); rit != matches.rend(); ++rit)
    {
        Match current = *rit;
        if (current.score == last)
        {
            ++counts;
            mismatches += current.mismatches;
        }
        else
        {
            if (last != INT32_MAX) {
                out.emplace_back(SummedMatches(last, counts, mismatches, total_mismatches, total_freq, dc));
                total_mismatches += mismatches;
                total_freq += counts;
            }
            counts = 1;
            mismatches = current.mismatches;
            last = current.score;
        }
    }
    out.emplace_back(SummedMatches(last, counts, mismatches, total_mismatches, total_freq, dc));
    return out;
}

double SummedMatches::get_distance(std::vector<SummedMatches> summedMatches, int threshold) {
    for (auto &match : summedMatches)
    {
        if (match.score == threshold)
        {
            return match.dist;
        }
    }
    return NAN;
}
