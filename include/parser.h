#ifndef PARSER_H
#define PARSER_H
#include <vector>
#include <fstream>
#include <iostream>
#include "Sequence.h"

std::vector<Sequence> parser (int argc, char **argv, std::vector<Sequence>& out);

#endif // PARSER_H
