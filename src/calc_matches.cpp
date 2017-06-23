#include "calc_matches.h"

using namespace std;

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

double calc_matches(vector<Word>& sw1, vector<Word>& sw2, vector<char>& seq1, vector<char>& seq2, int dc, string pattern)
{
    int skip = 0;
    for (unsigned int i = 0; i < sw1.size(); ++i)
    {
//        cout << "SpacedWord 1 = " << read_word(sw1[i].key, weight);
        int bl1 = bucket(sw1, i);
        if (bl1 > 1)
        {
            unsigned int limit1 = i + bl1 - 1;
            for (i; i <= limit1; ++i)
            {
                for (unsigned int j = 0 + skip; j < sw2.size(); ++j)
                {
//                    cout << "   SpacedWord 2 = " << read_word(sw2[j].key, weight) << endl;
                    int bl2 = bucket(sw2, j);
                    if (bl2 > 1)
                    {
//                        cout << " & (bucket-bucket) j = " << j << endl;
                        unsigned int limit2 = j + bl2 - 1;
                        for (j; j <= limit2; ++j)
                        {
                            if (sw1[i].key > sw2[j].key)
                            {
                                skip += bl2 - 1;
                                continue;
                            }
                            if (sw1[i].key < sw2[j].key)
                            {
                                break;
                            }
                            if (sw1[i].key == sw2[j].key)
                            {
                                // Hier noch unbedingt Multi-Match beachten!!!
                                for (unsigned int pat = 0; pat < pattern.size(); ++pat)
                                {
                                    if (pattern[pat] == 0)
                                    {
                                        score += blosum62[ (int) seq1[sw1[i].pos + pat] ][ (int) seq2[sw2[j].pos + pat] ];
                                        if (seq1[sw1[i].pos + pat] != seq2[sw2[j].pos + pat])
                                        {
                                            ++mismatches;
                                        }
                                    }
                                }
                                if (score > threshold)
                                {
                                    total_mismatches += mismatches;
                                    total_dc += dc;
                                }
								continue;
                            }
                        }
                    }
                    else
                    {
//                        cout << " & (if-single) j = " << j << endl;
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
                            // Hier noch unbedingt Multi-Match beachten!!!
                            for (unsigned int pat = 0; pat < pattern.size(); ++pat)
                            {
                                if (pattern[pat] == 0)
                                {
                                    score += blosum62[ (int) seq1[sw1[i].pos + pat] ][ (int) seq2[sw2[j].pos + pat] ];
                                    if (seq1[sw1[i].pos + pat] != seq2[sw2[j].pos + pat])
                                    {
                                        ++mismatches;
                                    }
                                }
                            }
                            if (score > threshold)
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
        else
        {
            for (unsigned int j = 0 + skip; j < sw2.size(); ++j)
            {
//                cout << "   SpacedWord 2 = " << read_word(sw2[j].key, weight) << endl;
                int bl2 = bucket(sw2, j);
                if (bl2 > 1)
                {
//                    cout << " & (else bucket) j = " << j << endl;
                    unsigned int limit = j + bl2 - 1;
                    for (j; j <= limit; ++j)
                    {
                        if (sw1[i].key > sw2[j].key)
                        {
                            skip += bl2 - 1;
							continue;
                        }
                        if (sw1[i].key < sw2[j].key)
                        {
                            break;
                        }
                        if (sw1[i].key == sw2[j].key)
                        {
                            skip = j + 1;
                            // Hier noch unbedingt Multi-Match beachten!!!
                            for (unsigned int pat = 0; pat < pattern.size(); ++pat)
                            {
                                if (pattern[pat] == 0)
                                {
                                    score += blosum62[ (int) seq1[sw1[i].pos + pat] ][ (int) seq2[sw2[j].pos + pat] ];
                                    if (seq1[sw1[i].pos + pat] != seq2[sw2[j].pos + pat])
                                    {
                                        ++mismatches;
                                    }
                                }
                            }
                            if (score > threshold)
                            {
                                total_mismatches += mismatches;
                                total_dc += dc;
                            }
							break;
                        }
                    }
                }
                else
                {
//                    cout << " & (else) j = " << j << endl;
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
                        skip = j + 1;
                        for (unsigned int pat = 0; pat < pattern.size(); ++pat)
                        {
                            if (pattern[pat] == 0)
                            {
                                score += blosum62[ (int) seq1[sw1[i].pos + pat] ][ (int) seq2[sw2[j].pos + pat] ];
                                if (seq1[sw1[i].pos + pat] != seq2[sw2[j].pos + pat])
                                {
                                    ++mismatches;
                                }
                            }
                        }
                        if (score > threshold)
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
}
