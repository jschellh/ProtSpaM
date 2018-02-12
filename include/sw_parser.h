#ifndef SW_PARSER_H
#define SW_PARSER_H
#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include "Sequence.h"
#include "Word.h"
#include "misc.h"

std::vector<Sequence> sw_parser(std::vector<std::string>&, std::vector<Sequence>&, std::vector<std::vector<char>> const&);

#endif // SW_PARSER_H
