#ifndef PARAMETERS_H
#define PARAMETERS_H
#include <getopt.h>
#include <string>
#include <iostream>
#include <vector>
#include <fstream>

    void parseParameters(int argc, char *argv[], int& weight, int& dc, int& threshold, int& patterns, int& threads, std::vector<std::string>& inputFileNames, std::string& output, bool& sp, std::string& lp, bool& outputScores);
    void printHelp();
    void printParameters(int&, int&, int&, int&, int&, std::string&);

#endif // PARAMETERS_H
