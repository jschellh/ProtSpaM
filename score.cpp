using namespace std;

vector<int> dc_score(map <string, map<vector<string>, vector<int> > >& dc_map1, map <string, map<vector<string>, vector<int> > >& dc_map2, const int& weight, string name_1, string name_2, const int& thr, const int& dc){
        //cout << endl << "Berechnung des Scores der Dontcare Wörter:" << endl;
        const int rows_col = 24;
        int blosum62 [rows_col][rows_col] = {   {4,  -1, -2, -2,  0, -1, -1,  0, -2, -1, -1, -1, -1, -2, -1,  1,  0, -3, -2,  0, -2, -1,  0, -4},    // Array, das mit der Matrix befüllt wird
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
        map <string, int> b62;
        //A  R  N  D  C  Q  E  G  H  I  L  K  M  F  P  S  T  W  Y  V  B  Z  X  *
        {b62.insert(pair<string, int>("A", 0));
        b62.insert(pair<string, int>("R", 1));
        b62.insert(pair<string, int>("N", 2));
        b62.insert(pair<string, int>("D", 3));
        b62.insert(pair<string, int>("C", 4));
        b62.insert(pair<string, int>("Q", 5));
        b62.insert(pair<string, int>("E", 6));
        b62.insert(pair<string, int>("G", 7));
        b62.insert(pair<string, int>("H", 8));
        b62.insert(pair<string, int>("I", 9));
        b62.insert(pair<string, int>("L", 10));
        b62.insert(pair<string, int>("K", 11));
        b62.insert(pair<string, int>("M", 12));
        b62.insert(pair<string, int>("F", 13));
        b62.insert(pair<string, int>("P", 14));
        b62.insert(pair<string, int>("S", 15));
        b62.insert(pair<string, int>("T", 16));
        b62.insert(pair<string, int>("W", 17));
        b62.insert(pair<string, int>("Y", 18));
        b62.insert(pair<string, int>("V", 19));
        b62.insert(pair<string, int>("B", 20));
        b62.insert(pair<string, int>("Z", 21));
        b62.insert(pair<string, int>("X", 22));
        b62.insert(pair<string, int>("*", 23));}

        list<tuple<int, int, int, int> > matches;                              //speichert den score und die startpositionen für matches

        int score = 0;                                      //speichert den Score des aktuellen dc-wortes
        map<string, map<vector<string>, vector<int> > >::iterator it1 = dc_map1.begin() ;
        map<string, map<vector<string>, vector<int> > >::iterator iter = dc_map2.begin();

        string word1;                                       //speichert das aktuelle dc_wort von sequenz1
        string word2;                                       //speichert das aktuelle dc_wort von sequenz2
        string::iterator word_it1;
        string::iterator word_it2;

        vector<string> dc_vec1;
        vector<string> dc_vec2;
        vector<int> dc_int1;
        vector<int> dc_int2;

        int mismatch_no;

        for (it1; it1 != dc_map1.end(); it1++, iter++){                                                         // For-Loop über erste dc_map, da beide gleich lang
            //cout << "Aktuelles Wordmatch: " << it1->first << endl;
            map<vector<string>, vector<int> >  inner_map1 = it1->second;                                        // innere Map enthält vector<dc-wörter> und vector<startpositionen>
            map<vector<string>, vector<int> >  inner_map2 = iter->second;
            map<vector<string>, vector<int> > ::iterator mapit1 = inner_map1.begin();                           // iteriert über innere Map
            map<vector<string>, vector<int> > ::iterator mapit2 = inner_map2.begin();
            for (mapit1; mapit1 != inner_map1.end(); mapit1++, mapit2++){
                dc_vec1 = mapit1 -> first;                                                                      // vector mit allen dc-Wörtern des aktuellen Matches für Sequenz 1
                dc_vec2 = mapit2 -> first;
                dc_int1 = mapit1 -> second;                                                                     // vector mit allen Startpositionen der dc-Wörter für Sequenz 1
                dc_int2 = mapit2 -> second;
                for(unsigned int i = 0; i < dc_vec1.size(); i++){                                               // iteriere über alle dc-Wörter des aktuellen Matches für Sequenz 1
                    for (unsigned int j = 0; j < dc_vec2.size(); j++){
                        score = 0;
                        mismatch_no = 0;                                                                        // initialisiere die Mismatches mit 0
                        word1 = dc_vec1[i];                                                                     // lade das i-te dc-Wort für Sequenz 1
                        word2 = dc_vec2[j];                                                                     // lade das j-te dc-Wort für Sequenz 2
                        int pos1 = dc_int1[i];                                                                  // lade Startposition des i-ten Wortes
                        int pos2 = dc_int2[j];
                        word_it2 = word2.begin();
                         //cout << "Das dc_word aus Sequenz 1 lautet: " << word1 << endl;
                         //cout << "Das dc_word aus Sequenz 2 lautet: " << word2 << endl;
                        for (word_it1 = word1.begin(); word_it1 != word1.end(); word_it1++, word_it2++){        // iteriere über jeden Buchstaben der dc-Wörter
                            string tmp_a(1,*word_it1);                                                          // speichere aktuellen Buchstaben beider Wörter
                            string tmp_b(1,*word_it2);
                            //cout << tmp_a << " , " << tmp_b << endl;
                            score = score + blosum62[b62.at(tmp_a)] [b62.at(tmp_b)];                            // hole Score aus blosum62-matrix und addiere ihn auf
                            if (tmp_a != tmp_b){
                                mismatch_no++;                                                                  // falls ein Mismatch vorliegt, erhöhe mismatch_no
                            }
                        }
                        if (score >= thr){                                                                      // wenn der Score höher ist als der Threshold...
                            tuple<int, int, int, int> temp = make_tuple(score, pos1, pos2, mismatch_no);
                            matches.push_back(temp);                                                            // ...speichere das aktuelle Wordmatch in matches
                            //if (homolog.find(score) != homolog.end() ){
                            //homolog[score] = homolog[score] + 1;
                            //cout << "Der Score beider homologen Wörter beträgt: " << score << endl << endl;
                        }
                    }
                }
            }
        }

        /*cout << "Matches-List vor unique:" << endl;
        for (list<tuple<int, int, int, int> >::iterator mit = matches.begin(); mit != matches.end(); mit++){
            cout << get<0>(*mit) << " " << get<1>(*mit) << " " << get<2>(*mit) << " " << get<3>(*mit) << " " << endl;
        }*/

        matches = unique_match(matches);                                                                        // behalte nur das Match mit höchstem Score für Positionen i bzw. j
        /*cout << "Matches-List nach unique:" << endl;
        for (list<tuple<int, int, int, int> >::iterator mit = matches.begin(); mit != matches.end(); mit++){
            cout << get<0>(*mit) << " " << get<1>(*mit) << " " << get<2>(*mit) << " " << get<3>(*mit) << " " << endl;
        }*/


        vector<int> output;
        unsigned int dc_no = matches.size() * dc;
        int mismatches = 0;
        map<int,int> out;
        for (list<tuple<int, int, int, int> >::iterator it = matches.begin(); it != matches.end(); it++){
            mismatches = mismatches + get<3>(*it);
            if (out.find(get<0>(*it) ) != out.end() ){
                out.at(get<0>(*it) ) += 1;
            }
            else{
                out.insert(pair<int, int>(get<0>(*it), 1) );
            }
        }
        output.push_back(mismatches);
        output.push_back(dc_no);

        /*
        ofstream scorehl_out;                                                    // Anlegen des Output-Streams
        map<int, int>::iterator scit;
        string filename_hl = "k";
        filename_hl += to_string(weight);
        filename_hl += "_" + name_1 + "|" + name_2;
        scorehl_out.open(filename_hl);
        scit = out.begin();
        for (scit; scit != out.end(); scit++){
            scorehl_out << scit->first << " " << scit->second << endl;
        }
        scorehl_out.close(); */

        return output;

}
