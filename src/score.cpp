#include "score.h"

using namespace std;

int bucket (vector<tuple<unsigned long long, int, int> > matches, int start)
{
    int bucket_length = 1;
    while (get<0>(matches[start]) == get<0>(matches[start+1]) )
    {
        ++bucket_length;
        ++start;
    }
    return bucket_length;
}

int score(vector<tuple<unsigned long long,int, int> > &matches, Sequence& seq1, Sequence& seq2, string pattern, int& dc, int& threshold)
{
//        cout << "Welcome inside Score-Function =)\n";
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

        int total_mismatches = 0;
        int dc_pos = 0;
        for (unsigned int i = 0; i < matches.size(); ++i)
        {
            int mismatches = 0;
            int score = 0;
            int bl = bucket(matches, i);
            if (bl > 1)
            {
                tuple<int, int> best;
                unsigned int limit = i + bl;
                for (i; i < limit; ++i)
                {
                    int pos_1 = get<1>(matches[i]);
                    int limit_1 = pos_1 + pattern.length();
                    int pos_2 = get<2>(matches[i]);
                    int limit_2 = pos_2 + pattern.length();
                    vector<char> match_1 (seq1.seq.begin() + pos_1, seq1.seq.begin() + limit_1);
                    vector<char> match_2 (seq2.seq.begin() + pos_2, seq2.seq.begin() + limit_2);
                    for (unsigned int j = 0; j < pattern.size(); ++j)
                    {
                        if (pattern[j] == '0')
                        {
                            score += blosum62[ match_1[j] - '0' ][ match_2[j] - '0' ];
                            if (match_1[j] != match_2[j])
                            {
                               ++mismatches;
                            }
                        }
                    }
                    if (score >= threshold)
                    {
                        if (score > get<0>(best) )
                        {
                            best = make_tuple(score, mismatches);
                        }
                    }
//                cout << i << ". Durchgang erfolgreich!\n";
                }
                total_mismatches += get<1>(best);
                dc_pos += dc;
            }
            else
            {
                int pos_1 = get<1>(matches[i]);
                int limit_1 = pos_1 + pattern.length();
                int pos_2 = get<2>(matches[i]);
                int limit_2 = pos_2 + pattern.length();
                vector<char> match_1 (seq1.seq.begin() + pos_1, seq1.seq.begin() + limit_1);
                vector<char> match_2 (seq2.seq.begin() + pos_2, seq2.seq.begin() + limit_2);
                for (unsigned int j = 0; j < pattern.size(); ++j)
                {
                    if (pattern[j] == '0')
                    {
                        score += blosum62[ match_1[j] - '0' ][ match_2[j] - '0' ];
                        if (match_1[j] != match_2[j])
                        {
                           ++mismatches;
                        }
                    }
                }
                if (score >= threshold)
                {
                    total_mismatches += mismatches;
                    dc_pos += dc;
                }
             }
//             cout << i << ". Durchgang erfolgreich!\n";
         }
         int mismatch_rate = total_mismatches / dc_pos;
         return mismatch_rate;
}
