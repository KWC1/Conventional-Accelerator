#pragma once
#include <systemc.h>

SC_MODULE(tb) {
	sc_in<bool> clk;

	///// MAC to tb input port /////
	sc_in<sc_int<32>> mac_result;
	sc_in<bool> mac_result_valid;

	///// tb to MAC output port /////
	sc_out<bool> mac_result_ready;

	///// tb & IDP handshake signal /////
	sc_out<bool> mac_response;

	void tb_main();

	SC_CTOR(tb) {
		SC_CTHREAD(tb_main, clk.pos())
	}

};

sc_int<32> mac_result_value;

void tb::tb_main(void) {
	sc_int<32> mac_result_value;

	for (int i = 0; i < 4; i++) {
		mac_response.write(0);
		for (int k = 0; k < 16; k++) {
			do {
				wait();
			} while (!mac_result_valid.read())
				;
			mac_result_value = mac_result.read();
			mac_result_ready.write(1);

			cout << "[Testbench] " << endl << mac_result_value << endl << endl;
			wait();
			mac_result_ready.write(0);
		}
		mac_response.write(1);
		wait();
	}
	
}