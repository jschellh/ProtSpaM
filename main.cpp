#include <iostream>
#include <string>
#include <bitset> // output binary
#include <map>
#include <list>
#include <iterator> // advance(iterator, int)

using namespace std;

list<long long> spacedWords (string const& seq, string const& pattern)
{
    list<long long> out;
    long long word = 0;
    long long tmp;
    for (unsigned int i = 0; i < (seq.length() - pattern.length() + 1); ++i)
    {
        for (unsigned int j = 0; j < pattern.length(); j++)
        {
            if (pattern[j] == '1') {
                switch (seq[i]) {
                    case 'A': tmp = 1; break;
                    case 'B': tmp = 2; break;
                    case 'C': tmp = 3; break;
                    case 'D': tmp = 4; break;
                    case 'E': tmp = 5; break;
                    case 'F': tmp = 6; break;
                    case 'G': tmp = 7; break;
                    case 'H': tmp = 8; break;
                    case 'I': tmp = 9; break;
                    case 'K': tmp = 10; break;
                    case 'L': tmp = 11; break;
                    case 'M': tmp = 12; break;
                    case 'N': tmp = 13; break;
                    case 'P': tmp = 14; break;
                    case 'Q': tmp = 15; break;
                    case 'R': tmp = 16; break;
                    case 'S': tmp = 17; break;
                    case 'T': tmp = 18; break;
                    case 'V': tmp = 19; break;
                    case 'W': tmp = 20; break;
                    case 'X': tmp = 21; break;
                    case 'Y': tmp = 22; break;
                    case 'Z': tmp = 23; break;
                    case '*': tmp = 24; break;
                }
                word <<= 5;
                word |= tmp;
            }
            i++;
        }
        i = i - pattern.length();
        out.push_back(word);
        word = 0;
     }
     out.sort();
     return out;
}

list <long long> findMatches (list<long long> const &sw1, list<long long>& sw2)
{
    int skip = 0;
    list <long long> out;
    for (list<long long>::const_iterator it = sw1.begin(); it != sw1.end(); it++)
    {
        for (list<long long>::iterator it2 = sw2.begin(); it2 != sw2.end(); it2++)
        {
            advance(it2,skip);
            if (*it > *it2)
            {
                skip++;
                continue;
            }
            if (*it < *it2)
            {
                break;
            }
            if (*it == *it2)
            {
                skip++;
                out.push_back(*it);
            }
        }
    }
    out.sort();
    return out;
}

int main()
{
    /*Required variables*/
    string sequence1;
    string sequence2;
    string pattern;


    cout << "Enter a protein sequence\n";
    cin >> sequence1;
    cout << "Enter another protein sequence\n";
    cin >> sequence2;
    cout << "Enter a pattern with no more than 12 Care-Positions\n";
    cin >> pattern;

    list<long long> spaced_words1 = spacedWords(sequence1, pattern);
    cout << "First SW-List:\n";
    for(list<long long>::iterator it = spaced_words1.begin(); it != spaced_words1.end(); it++)
    {
        cout << *it << endl;
    }

    list<long long> spaced_words2 = spacedWords(sequence2, pattern);
    cout << "Second SW-List:\n";
    for(list<long long>::iterator it = spaced_words2.begin(); it != spaced_words2.end(); it++)
    {
        cout << *it << endl;
    }

    list<long long> matches = findMatches(spaced_words1, spaced_words2);
    cout << "Matches found:\n";
    for(list<long long>::iterator it = matches.begin(); it != matches.end(); it++)
    {
        cout << *it << endl;
    }

    cin.get();
    return 0;
}

