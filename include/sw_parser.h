#ifndef SW_PARSER_H
#define SW_PARSER_H
#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include "Species.h"
#include "Word.h"
#include "misc.h"

std::vector<Species> sw_parser(std::vector<std::string>&, std::vector<Species>&, std::vector<std::vector<char>> const&);

#endif // SW_PARSER_H
