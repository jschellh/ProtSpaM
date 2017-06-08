#include <iostream>
#include <string>
#include <bitset>
#include <map>

using namespace std;

long long to_binary (string seq, string pattern) {
    long long word;
    long long tmp;
    unsigned int length = seq.length();
    for (unsigned int i = 0; i < length; ++i) {
        int shift = (length - i - 1)*5;
        if (pattern[i] == '1') {
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
            tmp <<= shift;
            word |= tmp;
        }
        else word >>=5;
     }
     return word;
}

int main()
{
    cout << "Enter a protein sequence no longer than 12aa\n";
    string sequence;
    cin >> sequence;
    cout << "Enter a pattern\n";
    string pattern;
    cin >> pattern;

    long long test = to_binary(sequence, pattern);
    cout << bitset<64>(test) << endl;

    cin.get();
    return 0;
}

