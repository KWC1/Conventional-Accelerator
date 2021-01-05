/*!
 *   @file BRAM.h
 *   @brief Block RAM model for SystemC Simulation
 *   @author SooHyun Kim (soohyunkim@kw.ac.kr)
 *
 *   @note Assume 1 cycle read/write, and separated in/out port
*/

#ifndef SYSTEMC_NULLHOP_BRAM_H
#define SYSTEMC_NULLHOP_BRAM_H

#pragma once
#include <systemc.h>

//Memory Configuration
SC_MODULE(BRAM) {
    sc_in_clk clk;
    sc_in<bool> rst;

    sc_in<sc_uint<ADDR_WIDTH>> inAddr;
    sc_in<bool> access;
    sc_out<sc_uint<DATA_WIDTH>> dOut;

    sc_uint<DATA_WIDTH> *memArray;

    void memoryAction() {
        while (1) {
            wait();//wait for clock
            sc_uint<ADDR_WIDTH> readAddr;
            if (!access.read()) continue;//nothing to do when no access
            readAddr = inAddr.read();//load address to internal register
            wait(1);//wait for memory access
            dOut.write(memArray[readAddr.range(31, 2) << 2].to_uint());//write data to output port
        }
    }

    SC_CTOR(MEMORY) {
        SC_CTHREAD(memoryAction, clk.pos());
        memArray = new sc_uint<DATA_WIDTH>[MEM_SIZE];

        for (int i = 0; i < MEM_SIZE; i++) {
            memArray[i] = rand();
        }

        async_reset_signal_is(rst, true);
    }

    ~MEMORY() {
        delete[] memArray;
    }

};



#endif //SYSTEMC_NULLHOP_BRAM_H