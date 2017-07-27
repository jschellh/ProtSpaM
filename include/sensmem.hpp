#ifndef SENSMEM_HPP_
#define SENSMEM_HPP_

#include <cstdint>
#include <iostream>
#include <string>
#include "sys/sysinfo.h"
#include "rasbopt.hpp"

namespace sensitivity_memory{
    bool check_memory();
    double available_memory();
    void security_message(std::string errmsg, int pos);
    extern double MemVal;
};

#endif