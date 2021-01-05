#pragma once
#include <systemc.h>

#define SRAM_SZ 512

SC_MODULE(SRAM){
    sc_in<sc_int<15>> addr;
    sc_inout<sc_uint<8>> io;

    sc_out<bool> ce_n;
    sc_out<bool> we_n;
    sc_out<bool> oe_n;

    void source(){//give data
        while(true){
            if(ce_n.read() || oe_n.read()){
                wait(70, SC_NS);
            }

        }
    }

    void sink(){//get data

    }

    SC_CTOR(SRAM){
            SC_THREAD(read);
            SC_THREAD(write);
    }

};