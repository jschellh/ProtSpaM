#ifndef PARSER_H
#define PARSER_H
#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include "Sequence.h"

std::vector<Sequence> parser (std::string filename, std::vector<Sequence>& out);

#endif // PARSER_H
