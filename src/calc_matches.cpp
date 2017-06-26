#include "calc_matches.h"

using namespace std;

vector<string> trans = {"A","R","N","D","C","Q","E","G","H","I","L","K","M","F","P","S","T","W","Y","V","B","Z","X","*"};

string read_sw (unsigned long long spaced_word, unsigned int weight)
{
    string out;
    int i = 0;
    while (out.size() < weight)
    {
        int letter = spaced_word & ((1 << 5) - 1);
        spaced_word >>= 5;
        out.insert(i,trans[letter]);
    }
    return out;
}

const int rows_col = 24;
int blosum62 [rows_col][rows_col] =
    {
        {4,  -1, -2, -2,  0, -1, -1,  0, -2, -1, -1, -1, -1, -2, -1,  1,  0, -3, -2,  0, -2, -1,  0, -4},    // Array, das mit der Matrix befüllt wird
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

int multimatch (vector<Word>& sortedWords, int start)
{
    int multimatch_length = 1;
    while (sortedWords[start].key == sortedWords[start+1].key)
    {
        ++multimatch_length;
        ++start;
    }
    return multimatch_length;
}

double calc_matches(vector<Word>& sw1, vector<Word>& sw2, vector<char>& seq1, vector<char>& seq2, int& weight, int& dc, int& threshold, vector<char> pattern)
{
    int skip = 0;
    int total_mismatches = 0;
    int total_dc = 0;
    int score;
    int mismatches;
    bool multi_done = false;

//    cout << "sw1.size() = " << sw1.size() << " | " << "sw2.size() = " << sw2.size() << endl;
    for (unsigned int i = 0; i < sw1.size(); ++i)
    {
//        cout << "SpacedWord 1 = " << read_sw(sw1[i].key, weight);
        int bl1 = multimatch(sw1, i);
        if (bl1 > 1)
        {
            vector<int> best = {threshold - 1, 0};
            unsigned int limit1 = i + bl1 - 1;
            for (i; i <= limit1; ++i)
            {
                bool singlematch = true;
                for (unsigned int j = 0 + skip; j < sw2.size() && singlematch; ++j)
                {
//                    cout << "   SpacedWord 2 = " << read_sw(sw2[j].key, weight) << endl;
                    int bl2 = multimatch(sw2, j);
                    if (bl2 > 1)
                    {
                        singlematch = false;
                        unsigned int limit2 = j + bl2 - 1;
                        for (j; j <= limit2; ++j)
                        {
//                            cout << " & (multiword - multiword) j = " << j << endl;
                            score = 0;
                            mismatches = 0;
                            if (sw1[i].key > sw2[j].key)
                            {
                                skip += bl2;
                                continue;
                            }
                            if (sw1[i].key < sw2[j].key)
                            {
                                break;
                            }
                            if (sw1[i].key == sw2[j].key)
                            {
//                                cout << "found a match!\n";
                                for (unsigned int pat = 0; pat < pattern.size(); ++pat)
                                {
//                                    cout << "pattern[pat] = " << pattern[pat] << endl;
                                    if (pattern[pat] == '0')
                                    {
//                                        cout << "pat = " << pat << " | sw1[i].pos = " << sw1[i].pos << " | sw2[j].pos = " << sw2[j].pos << endl;
//                                        cout << "score += blosum62[ " << (int) seq1[sw1[i].pos + pat] << " ][ " << (int) seq2[sw2[j].pos + pat] << " ]  | " << blosum62[ (int) seq1[sw1[i].pos + pat] ][ (int) seq2[sw2[j].pos + pat] ] << endl;
                                        score += blosum62[ (int) seq1[sw1[i].pos + pat] ][ (int) seq2[sw2[j].pos + pat] ];
                                        if ( (int) seq1[sw1[i].pos + pat] != (int) seq2[sw2[j].pos + pat])
                                        {
                                            ++mismatches;
                                        }
                                    }
                                }
//                                cout << score << " | " << mismatches << endl;
                                if (score > threshold && score > best[0])
                                {
                                    best[0] = score;
                                    best[1] = mismatches;
                                }
                                if (i == limit1 && j == limit2)
                                {
                                    skip += bl2;
                                    multi_done = true;
                                    break;
                                }
                            }
                        }
                    }
                    else
                    {

//                        cout << " & (multiword - singleword) j = " << j << endl;
                        if (sw1[i].key > sw2[j].key)
                        {
                            skip = j + 1;
                            continue;
                        }
                        if (sw1[i].key < sw2[j].key)
                        {
							break;
                        }
                        if (sw1[i].key == sw2[j].key)
                        {
//                            cout << "found a match!\n";
                            score = 0;
                            mismatches = 0;
                            for (unsigned int pat = 0; pat < pattern.size(); ++pat)
                            {
//                                cout << "pattern[pat] = " << pattern[pat] << endl;
                                if (pattern[pat] == '0')
                                {
//                                    cout << "pat = " << pat << " | sw1[i].pos = " << sw1[i].pos << " | sw2[j].pos = " << sw2[j].pos << endl;
//                                    cout << "score += blosum62[ " << (int) seq1[sw1[i].pos + pat] << " ][ " << (int) seq2[sw2[j].pos + pat] << " ]  | " << blosum62[ (int) seq1[sw1[i].pos + pat] ][ (int) seq2[sw2[j].pos + pat] ] << endl;
                                    score += blosum62[ (int) seq1[sw1[i].pos + pat] ][ (int) seq2[sw2[j].pos + pat] ];
                                    if ( (int) seq1[sw1[i].pos + pat] != (int) seq2[sw2[j].pos + pat])
                                    {
                                        ++mismatches;
                                    }
                                }
                            }
//                            cout << score << " | " << mismatches << endl;
                            if (score >= threshold && score > best[0])
                            {
                                best[0] = score;
                                best[1] = mismatches;
                            }
                            if (i == limit1)
                            {
                                skip = j + 1;
                                multi_done = true;
                                break;
                            }
                            break;
                        }
                    }
                }
            }
            if (best[0] >= threshold)
            {
                total_mismatches += best[1];
                total_dc += dc;
            }
            if (bl1 > 1 && multi_done)
            {
                --i;
                multi_done = false;
            }
        }
        else
        {
            bool go_on = true;
            for (unsigned int j = 0 + skip; j < sw2.size() && go_on; ++j)
            {
//                cout << "   SpacedWord 2 = " << read_sw(sw2[j].key, weight) << endl;
                int bl2 = multimatch(sw2, j);
                if (bl2 > 1)
                {
                    go_on = false;
                    vector<int> best = {threshold - 1, 0};
                    unsigned int limit = j + bl2 - 1;
                    for (j; j <= limit; ++j)
                    {
//                        cout << " & (singleword - multiword) j = " << j << endl;
                        score = 0;
                        mismatches = 0;
                        if (sw1[i].key > sw2[j].key)
                        {
                            skip += bl2;
							continue;
                        }
                        if (sw1[i].key < sw2[j].key)
                        {
                            break;
                        }
                        if (sw1[i].key == sw2[j].key)
                        {
//                            cout << "found a match!\n";
                            for (unsigned int pat = 0; pat < pattern.size(); ++pat)
                            {
//                                cout << "pattern[pat] = " << pattern[pat] << endl;
                                if (pattern[pat] == '0')
                                {
//                                    cout << "pat = " << pat << " | sw1[i].pos = " << sw1[i].pos << " | sw2[j].pos = " << sw2[j].pos << endl;
//                                    cout << "score += blosum62[ " << (int) seq1[sw1[i].pos + pat] << " ][ " << (int) seq2[sw2[j].pos + pat] << " ]  | " << blosum62[ (int) seq1[sw1[i].pos + pat] ][ (int) seq2[sw2[j].pos + pat] ] << endl;
                                    score += blosum62[ (int) seq1[sw1[i].pos + pat] ][ (int) seq2[sw2[j].pos + pat] ];
                                    if ( (int) seq1[sw1[i].pos + pat] != (int) seq2[sw2[j].pos + pat])
                                    {
                                        ++mismatches;
                                    }
                                }
                            }
//                            cout << score << " | " << mismatches << endl;
                            if (score > threshold)
                            {
                                best[0] = score;
                                best[1] = mismatches;
                            }
							if (j == limit)
                            {
                                if (best[0] > threshold)
                                {
                                    total_mismatches += best[1];
                                    total_dc += dc;
                                }
                                skip += bl2;
                            }
                        }
                    }

                }
                else
                {
//                    cout << " & (singleword - singleword) j = " << j << endl;
                    if (sw1[i].key > sw2[j].key)
                    {
                        skip = j + 1;
						continue;
                    }
                    if (sw1[i].key < sw2[j].key)
                    {
                        break;

                    }
                    if (sw1[i].key == sw2[j].key)
                    {
//                        cout << "found a match!\n";
                        skip = j + 1;
                        score = 0;
                        mismatches = 0;
                        for (unsigned int pat = 0; pat < pattern.size(); ++pat)
                        {
//                            cout << "pattern[pat] = " << pattern[pat] << endl;
                            if (pattern[pat] == '0')
                            {
//                                cout << "pat = " << pat << " | sw1[i].pos = " << sw1[i].pos << " | sw2[j].pos = " << sw2[j].pos << endl;
//                                cout << "score += blosum62[ " << (int) seq1[sw1[i].pos + pat] << " ][ " << (int) seq2[sw2[j].pos + pat] << " ]  | " << blosum62[ (int) seq1[sw1[i].pos + pat] ][ (int) seq2[sw2[j].pos + pat] ] << endl;
                                score += blosum62[ (int) seq1[sw1[i].pos + pat] ][ (int) seq2[sw2[j].pos + pat] ];
                                if ( (int) seq1[sw1[i].pos + pat] != (int) seq2[sw2[j].pos + pat])
                                {
                                    ++mismatches;
                                }
                            }
                        }
//                        cout << score << " | " << mismatches << endl;
                        if (score >= threshold)
                        {
                            total_mismatches += mismatches;
                            total_dc += dc;
                        }
                        break;
                    }
                }
            }
        }
    }
    double mismatch_rate = (double) total_mismatches / total_dc;
//    cout << "mismatch_rate = " << total_mismatches << " / " << total_dc << " | " << mismatch_rate << endl;
    return mismatch_rate;
}
