#ifndef PARSER_H
#define PARSER_H
#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include "Species.h"

std::vector<Species> parser (std::string filename, std::vector<Species>& out);

#endif // PARSER_H
