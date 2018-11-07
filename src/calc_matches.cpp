#include "calc_matches.h"

using namespace std;

const int rows_col = 24;
int blosum62 [rows_col][rows_col] =
    {
        {4,  -1, -2, -2,  0, -1, -1,  0, -2, -1, -1, -1, -1, -2, -1,  1,  0, -3, -2,  0, -2, -1,  0, -4},
        {-1,  5,  0, -2, -3,  1,  0, -2,  0, -3, -2,  2, -1, -3, -2, -1, -1, -3, -2, -3, -1,  0, -1, -4},
        {-2,  0,  6,  1, -3,  0,  0,  0,  1, -3, -3,  0, -2, -3, -2,  1,  0, -4, -2, -3,  3,  0, -1, -4},
        {-2, -2,  1,  6, -3,  0,  2, -1, -1, -3, -4, -1, -3, -3, -1,  0, -1, -4, -3, -3,  4,  1, -1, -4},
        {0,  -3, -3, -3,  9, -3, -4, -3, -3, -1, -1, -3, -1, -2, -3, -1, -1, -2, -2, -1, -3, -3, -2, -4},
        {-1,  1,  0,  0, -3,  5,  2, -2,  0, -3, -2,  1,  0, -3, -1,  0, -1, -2, -1, -2,  0,  3, -1, -4},
        {-1,  0,  0,  2, -4,  2,  5, -2,  0, -3, -3,  1, -2, -3, -1,  0, -1, -3, -2, -2,  1,  4, -1, -4},
        {0,  -2,  0, -1, -3, -2, -2,  6, -2, -4, -4, -2, -3, -3, -2,  0, -2, -2, -3, -3, -1, -2, -1, -4},
        {-2,  0,  1, -1, -3,  0,  0, -2,  8, -3, -3, -1, -2, -1, -2, -1, -2, -2,  2, -3,  0,  0, -1, -4},
        {-1, -3, -3, -3, -1, -3, -3, -4, -3,  4,  2, -3,  1,  0, -3, -2, -1, -3, -1,  3, -3, -3, -1, -4},
        {-1, -2, -3, -4, -1, -2, -3, -4, -3,  2,  4, -2,  2,  0, -3, -2, -1, -2, -1,  1, -4, -3, -1, -4},
        {-1,  2,  0, -1, -3,  1,  1, -2, -1, -3, -2,  5, -1, -3, -1,  0, -1, -3, -2, -2,  0,  1, -1, -4},
        {-1, -1, -2, -3, -1,  0, -2, -3, -2,  1,  2, -1,  5,  0, -2, -1, -1, -1, -1,  1, -3, -1, -1, -4},
        {-2, -3, -3, -3, -2, -3, -3, -3, -1,  0,  0, -3,  0,  6, -4, -2, -2,  1,  3, -1, -3, -3, -1, -4},
        {-1, -2, -2, -1, -3, -1, -1, -2, -2, -3, -3, -1, -2, -4,  7, -1, -1, -4, -3, -2, -2, -1, -2, -4},
        {1,  -1,  1,  0, -1,  0,  0,  0, -1, -2, -2,  0, -1, -2, -1,  4,  1, -3, -2, -2,  0,  0,  0, -4},
        {0,  -1,  0, -1, -1, -1, -1, -2, -2, -1, -1, -1, -1, -2, -1,  1,  5, -2, -2,  0, -1, -1,  0, -4},
        {-3, -3, -4, -4, -2, -2, -3, -2, -2, -3, -2, -3, -1,  1, -4, -3, -2, 11,  2, -3, -4, -3, -2, -4},
        {-2, -2, -2, -3, -2, -1, -2, -3,  2, -1, -1, -2, -1,  3, -3, -2, -2,  2,  7, -1, -3, -2, -1, -4},
        {0,  -3, -3, -3, -1, -2, -2, -3, -3,  3,  1, -2,  1, -1, -2, -2,  0, -3, -1,  4, -3, -2, -1, -4},
        {-2, -1,  3,  4, -3,  0,  1, -1,  0, -3, -4,  0, -3, -3, -2,  0, -1, -4, -3, -3,  4,  1, -1, -4},
        {-1,  0,  0,  1, -3,  3,  4, -2,  0, -3, -3,  1, -1, -3, -1,  0, -1, -3, -2, -2,  1,  4, -1, -4},
        {0,  -1, -1, -1, -2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -2,  0,  0, -2, -1, -1, -1, -1, -1, -4},
        {-4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4,  1}
    };

int multiMatch(const vector<Word> &sortedWords, int start) {
    int multiMatch_length = 1;
    while (sortedWords[start].key == sortedWords[start+1].key) {
        ++multiMatch_length;
        ++start;
    }
    return multiMatch_length;
}

void scoreOutput(map<int,int> &scores, string header1, string header2) {
    header1.erase(header1.find_last_not_of(" \n\r\t") + 1);
    header2.erase(header2.find_last_not_of(" \n\r\t") + 1);
    string fileName = "scores/" + header1 + "_" + header2 + ".scr";

    vector<pair<int, int>> scoreVector(scores.size());
    int i = 0;
    for (const auto &pair : scores) {
        scoreVector[i++] = pair;
    }
    sort(scoreVector.begin(), scoreVector.end());

    ofstream scoresOut;
    scoresOut.open(fileName);
    scoresOut << "score,freq\n";
    for (auto &s : scoreVector) {
        if (s.first > INT32_MIN) {
            scoresOut << s.first << "," << s.second << "\n";
        }
    }
}

double calc_mismatchRate (vector<pair<int,int> >& mismatchesDC) {
    int mismatches = 0;
    int dontCare = 0;
    for (auto pair : mismatchesDC) {
        mismatches += pair.first;
        dontCare += pair.second;
    }
    return (double) mismatches / dontCare;
}

double calc_matches (const Species& species1, const Species& species2, const int& weight, const int& dc,
                     const int& threshold, const vector<vector<char> >& patterns, const bool& outputScores) {
    unsigned int skip = 0;
    int total_mismatches = 0;
    int total_dc = 0;
    int score;
    int mismatches;
    map<int, int> scores;
    bool multi_done = false;
    const vector<char>& sequence1 = species1.seq;
    const vector<char>& sequence2 = species2.seq;
    vector<pair<int, int>> mismatchDontCare;

    for (unsigned int currentPattern = 0; currentPattern < patterns.size(); ++currentPattern) {
        const vector<Word>& spacedWords1 = species1.sorted_words[currentPattern];
        const vector<Word>& spacedWords2 = species2.sorted_words[currentPattern];
        const vector<char>& pattern = patterns[currentPattern];
        for (unsigned int i = 0; i < spacedWords1.size(); ++i) {
            int bl1 = multiMatch(spacedWords1, i);
            if (bl1 > 1) {
                vector<int> best = {threshold - 1, 0};
                unsigned int limit1 = i + bl1 - 1;
                for (i; i <= limit1; ++i) {
                    bool singleMatch = true;
                    for (unsigned int j = 0 + skip; j < spacedWords2.size() && singleMatch; ++j) {
                        int bl2 = multiMatch(spacedWords2, j);
                        if (bl2 > 1) {
                            singleMatch = false;
                            unsigned int limit2 = j + bl2 - 1;
                            for (j; j <= limit2; ++j) {
                                score = 0;
                                mismatches = 0;
                                if (spacedWords1[i].key > spacedWords2[j].key) {
                                    skip += bl2;
                                    continue;
                                }
                                if (spacedWords1[i].key < spacedWords2[j].key) {
                                    break;
                                }
                                if (spacedWords1[i].key == spacedWords2[j].key) {
                                    for (unsigned int pat = 0; pat < pattern.size(); ++pat) {
                                        if (pattern[pat] == '0') {
                                            score += blosum62[ (int) sequence1[spacedWords1[i].pos + pat] ][ (int) sequence2[spacedWords2[j].pos + pat] ];
                                            if ( (int) sequence1[spacedWords1[i].pos + pat] != (int) sequence2[spacedWords2[j].pos + pat]) {
                                                ++mismatches;
                                            }
                                        }
                                    }
                                }
                                if (score >= threshold && score > best[0]) {
                                    best[0] = score;
                                    best[1] = mismatches;
                                }
                                if (i == limit1 && j == limit2) {
                                    skip += bl2;
                                    multi_done = true;
                                    break;
                                }
                            }
                        }
                        else {
                            if (spacedWords1[i].key > spacedWords2[j].key) {
                                skip = j + 1;
                                continue;
                            }
                            if (spacedWords1[i].key < spacedWords2[j].key) {
                                break;
                            }
                            if (spacedWords1[i].key == spacedWords2[j].key) {
                                score = 0;
                                mismatches = 0;
                                for (unsigned int pat = 0; pat < pattern.size(); ++pat) {
                                    if (pattern[pat] == '0') {
                                        score += blosum62[ (int) sequence1[spacedWords1[i].pos + pat] ][ (int) sequence2[spacedWords2[j].pos + pat] ];
                                        if ( (int) sequence1[spacedWords1[i].pos + pat] != (int) sequence2[spacedWords2[j].pos + pat]) {
                                            ++mismatches;
                                        }
                                    }
                                }
                                if (score >= threshold && score > best[0]) {
                                    best[0] = score;
                                    best[1] = mismatches;
                                }
                                if (i == limit1) {
                                    skip = j + 1;
                                    multi_done = true;
                                    break;
                                }
                                break;
                            }
                        }
                    }
                }
                if (outputScores) {
                    auto exists = scores.find(best[0]);
                    if (exists != scores.end() ) {
                        scores[best[0]] += 1;
                    }
                    else if (best[0]){
                        scores.insert(pair<int, int>(best[0], 1));
                    }
                }
                if (best[0] >= threshold) {
                    total_mismatches += best[1];
                    total_dc += dc;
                }
                if (bl1 > 1 && multi_done) {
                    --i;
                    multi_done = false;
                }
            }
            else {
                bool go_on = true;
                for (unsigned int j = 0 + skip; j < spacedWords2.size() && go_on; ++j) {
                    int bl2 = multiMatch(spacedWords2, j);
                    if (bl2 > 1) {
                        go_on = false;
                        vector<int> best = {threshold - 1, 0};
                        unsigned int limit = j + bl2 - 1;
                        for (j; j <= limit; ++j) {
                            score = 0;
                            mismatches = 0;
                            if (spacedWords1[i].key > spacedWords2[j].key) {
                                skip += bl2;
                                continue;
                            }
                            if (spacedWords1[i].key < spacedWords2[j].key) {
                                break;
                            }
                            if (spacedWords1[i].key == spacedWords2[j].key) {
                                for (unsigned int pat = 0; pat < pattern.size(); ++pat) {
                                    if (pattern[pat] == '0') {
                                        score += blosum62[ (int) sequence1[spacedWords1[i].pos + pat] ][ (int) sequence2[spacedWords2[j].pos + pat] ];
                                        if ( (int) sequence1[spacedWords1[i].pos + pat] != (int) sequence2[spacedWords2[j].pos + pat]) {
                                            ++mismatches;
                                        }
                                    }
                                }
                                if (score >= threshold && score > best[0]) {
                                    best[0] = score;
                                    best[1] = mismatches;
                                }
                                if (j == limit) {
                                    if (outputScores) {
                                        auto exists = scores.find(best[0]);
                                        if (exists != scores.end() ) {
                                            scores[best[0]] += 1;
                                        }
                                        else {
                                            scores.insert(pair<int, int>(best[0], 1));
                                        }
                                    }
                                    if (best[0] > threshold) {
                                        total_mismatches += best[1];
                                        total_dc += dc;
                                    }
                                    skip += bl2;
                                }
                            }
                        }
                    }
                    else {
                        if (spacedWords1[i].key > spacedWords2[j].key) {
                            skip = j + 1;
                            continue;
                        }
                        if (spacedWords1[i].key < spacedWords2[j].key) {
                            break;
                        }
                        if (spacedWords1[i].key == spacedWords2[j].key) {
                            skip = j + 1;
                            score = 0;
                            mismatches = 0;
                            for (unsigned int pat = 0; pat < pattern.size(); ++pat) {
                                if (pattern[pat] == '0') {
                                    score += blosum62[ (int) sequence1[spacedWords1[i].pos + pat] ][ (int) sequence2[spacedWords2[j].pos + pat] ];
                                    if ( (int) sequence1[spacedWords1[i].pos + pat] != (int) sequence2[spacedWords2[j].pos + pat]) {
                                        ++mismatches;
                                    }
                                }
                            }
                            if (outputScores) {
                                auto exists = scores.find(score);
                                if (exists != scores.end() ) {
                                    scores[score] += 1;
                                }
                                else {
                                    scores.insert(pair<int, int>(score, 1));
                                }
                            }
                            if (score >= threshold) {
                                total_mismatches += mismatches;
                                total_dc += dc;
                            }
                            break;
                        }
                    }
                }
            }
        }
        mismatchDontCare.emplace_back(total_mismatches, total_dc);
    }

    if (outputScores) {
        scoreOutput(scores, species1.header, species2.header);
    }

    return calc_mismatchRate(mismatchDontCare);
}