#include "parser.h"

using namespace std;

vector<Sequence> parser (string filename, vector<Sequence>& out)
{
    ifstream input(filename);
    if (!input.good() )
    {
        cerr << "Error opening '" << filename << "'. Bailing out.\n";
    }
    Sequence tmp;
    string line;
    while (getline(input,line).good() )
    {
        if (line.empty() || line[0] == '>')
        {
            if (!tmp.header.empty() )
            {
                out.push_back(tmp);
                tmp.clear_header();
                tmp.clear_seq();
            }
            if (!line.empty() )
            {
                if (line.substr(1).size() > 10)
                {
                    string name = line.substr(1).substr(0,9);
                    tmp.set_header(name);
                }
                else if (line.substr(1).size() < 10)
                {
                    string name = line.substr(1).substr(0);
                    while (name.size() < 10)
                    {
                        name.append(" ");
                    }
                    tmp.set_header(name);
                }
                else
                {
                    tmp.set_header(line.substr(1) );
                }
            }
            tmp.clear_seq();
        }
        else if (!tmp.header.empty() )
        {

            for (unsigned int i = 0; i < line.size(); ++i)
            {
                switch (line[i])
                {
                    case 'A': tmp.set_seq(0); break;
                    case 'R': tmp.set_seq(1); break;
                    case 'N': tmp.set_seq(2); break;
                    case 'D': tmp.set_seq(3); break;
                    case 'C': tmp.set_seq(4); break;
                    case 'Q': tmp.set_seq(5); break;
                    case 'E': tmp.set_seq(6); break;
                    case 'G': tmp.set_seq(7); break;
                    case 'H': tmp.set_seq(8); break;
                    case 'I': tmp.set_seq(9); break;
                    case 'L': tmp.set_seq(10); break;
                    case 'K': tmp.set_seq(11); break;
                    case 'M': tmp.set_seq(12); break;
                    case 'F': tmp.set_seq(13); break;
                    case 'P': tmp.set_seq(14); break;
                    case 'S': tmp.set_seq(15); break;
                    case 'T': tmp.set_seq(16); break;
                    case 'W': tmp.set_seq(17); break;
                    case 'Y': tmp.set_seq(18); break;
                    case 'V': tmp.set_seq(19); break;
                    case 'B': tmp.set_seq(20); break;
                    case 'Z': tmp.set_seq(21); break;
                    case 'X': tmp.set_seq(22); break;
                    case '*': tmp.set_seq(23); break;
                }
            }
        }
    }
    if (!tmp.header.empty() )
    {
        out.push_back(tmp);
    }
    return out;
}
