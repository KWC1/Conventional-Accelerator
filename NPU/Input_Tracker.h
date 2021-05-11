#pragma once
#include <systemc.h>

SC_MODULE(Input_tracker) {
	///// Input_tracker input port /////
	sc_in<bool> clk;
	sc_in<bool> request;
	sc_in<bool> tracker_ready;

	///// Input_tracker output port /////
	sc_out<sc_int<16>> tracker_addr;
	sc_out<sc_int<16>> tracker_location;
	sc_out<bool> tracker_valid;
	sc_out<bool> response;


	void Input_tracker_main();				// Input_tracker main function

	SC_CTOR(Input_tracker) {
		SC_CTHREAD(Input_tracker_main, clk.pos());
	}
};

int num = 1;
void Input_tracker::Input_tracker_main() {
	for (int i = 0; i < 4; i++) {
		do {
			wait();
		} while (!request.read())
			;
		
		response.write(1);
		wait();
		cout << "[Input tracker]" << endl;
		cout << "input address: " << num << endl;
		cout << "input pixel's location: " << 1 << endl << endl << endl;
		tracker_addr.write(num);
		tracker_location.write(1);
		tracker_valid.write(1);
		do {
			wait();
		} while (!tracker_ready.read())
			;
		num++;
	}	
}