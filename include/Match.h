#ifndef MATCH_H
#define MATCH_H


class Match {
public:
    int score;
    int mismatches;

    Match(int score, int mismatches);
    bool operator< (const Match& w) const { return score < w.score; }
};

#endif //MATCH_H
