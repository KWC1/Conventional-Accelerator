//
// Created by SHKim on 2020-11-12.
//

#ifndef SYSTEMC_NULLHOP_DEBUG_H
#define SYSTEMC_NULLHOP_DEBUG_H

#include <string>
#include <iostream>

class DebugSim{
    static void printErr(const std::string& where, const std::string& errstr){
        std::cerr << where << ": " << errstr << std::endl;
    }
};

#endif //SYSTEMC_NULLHOP_DEBUG_H
