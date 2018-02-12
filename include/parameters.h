#ifndef PARAMETERS_H
#define PARAMETERS_H
#include <getopt.h>
#include <string>
#include <iostream>
#include <vector>
#include <fstream>

    void parseParameters(int argc, char *argv[], int& weight, int& dc, int& threshold, int& patterns, int& threads, std::vector<std::string>& inputFilenames);
    void printHelp();
    void printParameters(int& weight, int& dc, int& threshold, int& patterns, int& threads);

#endif // PARAMETERS_H
