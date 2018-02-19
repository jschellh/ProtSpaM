#include "sw_parser.h"

using namespace std;

vector<string> translate_here = {"A","R","N","D","C","Q","E","G","H","I","L","K","M","F","P","S","T","W","Y","V","B","Z","X","*","J"};
vector<Sequence> sw_parser(vector<string>& fileNames, vector<Sequence>& out, vector<vector<char>> const& patterns)
{
    for (unsigned int i = 0; i < fileNames.size() -1; ++i)
    {
      	ifstream infile(fileNames[i]);
      	if (!infile.good() )
        {
            cerr << "Error opening '" << fileNames[i] << "'. Bailing out.\n";
        }
        string header = delete_suffix(delete_prefix(fileNames[i]));
        string line;
        vector<int> starts;
        Sequence tmpSequence;
        if (header.size() > 10)
        {
            string name = header.substr(0,9);
            tmpSequence.set_header(name);
        }
        else if (header.size() < 10)
        {
            string name = header.substr(0);
            while (name.size() < 10)
            {
                name.append(" ");
            }
            tmpSequence.set_header(name);
        }
        getline(infile, line, '>');
        while (!infile.eof())
        {
            vector<char> tmp;
            getline(infile, header);
            getline(infile, line, '>');
            for (unsigned int j = 0; j < line.size(); ++j)
            {
                switch (line[j])
                {
                    case 'A': tmp.push_back(0); break;
                    case 'R': tmp.push_back(1); break;
                    case 'N': tmp.push_back(2); break;
                    case 'D': tmp.push_back(3); break;
                    case 'C': tmp.push_back(4); break;
                    case 'Q': tmp.push_back(5); break;
                    case 'E': tmp.push_back(6); break;
                    case 'G': tmp.push_back(7); break;
                    case 'H': tmp.push_back(8); break;
                    case 'I': tmp.push_back(9); break;
                    case 'L': tmp.push_back(10); break;
                    case 'K': tmp.push_back(11); break;
                    case 'M': tmp.push_back(12); break;
                    case 'F': tmp.push_back(13); break;
                    case 'P': tmp.push_back(14); break;
                    case 'S': tmp.push_back(15); break;
                    case 'T': tmp.push_back(16); break;
                    case 'W': tmp.push_back(17); break;
                    case 'Y': tmp.push_back(18); break;
                    case 'V': tmp.push_back(19); break;
                    case 'B': tmp.push_back(20); break;
                    case 'Z': tmp.push_back(21); break;
                    case 'X': tmp.push_back(22); break;
                    case '*': tmp.push_back(23); break;
                }
            }
            if (tmp.size() >= patterns[0].size() )
            {
                starts.push_back(tmpSequence.seq.size() );
                for (unsigned int j = 0; j < tmp.size(); ++j)
                {
                    tmpSequence.set_seq(tmp[j]);
                }
            }
        }
        tmpSequence.set_starts(starts);
        out.push_back(tmpSequence);
    }
    return out;
}
