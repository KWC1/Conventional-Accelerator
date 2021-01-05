/*!
*   @file SDRAM.h
*   @brief SDRAM model for SystemC Simulation
*   @author SooHyun Kim (soohyunkim@kw.ac.kr)
*/

#ifndef SYSTEMC_NULLHOP_SDRAM_H
#define SYSTEMC_NULLHOP_SDRAM_H

#include <systemc.h>
#include "COMMON.h"

SC_MODULE(SDRAM){
private:
    unsigned char* RAM_ARRAY;

public:
    sc_in_clk CLK;
    sc_in<bool> WE;
    sc_in<sc_uint<2>> BA;
    sc_in<sc_uint<13>> A;
    sc_inout<sc_uint<16>> DQ;

    SC_CTOR(SDRAM){
        RAM_ARRAY = new unsigned char[SDRAM_SIZE*1024*1024];//Allocate memory as defined
        if(RAM_ARRAY == nullptr) DebugSim->printErr("SDRAM", "Memory allocation failed");
        memset(RAM_ARRAY, 0, MEM_SIZE*1024*1024);//Init with zero

        SC_CTHREAD(readAction, CLK.pos());
        SC_CTHREAD(writeAction, CLK.pos());
    }

    ~SDRAM(){
        delete[] RAM_ARRAY;//deallocate
    }

    void readAction(){
        wait();
        while(1) {
            if (WE.read() == 0) wait();
            //Read Address
            //Wait Address Strobe Cycle
            //Wait Data
            //Wait Charge Cycle
        }
    }

    void writeAction(){
        wait();
        while(1) {
            if (WE.read() == 1) wait();
            //Read Address
            //Wait Address Strobe Cycle
            //Write Data
        }
    }

}

#endif //SYSTEMC_NULLHOP_SDRAM_H
