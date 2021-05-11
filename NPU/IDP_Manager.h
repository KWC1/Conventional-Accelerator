#pragma once
#include <systemc.h>

SC_MODULE(IDP_Manager) {
	///// IDP_Manager input port /////
	sc_in<bool> clk;
	sc_in<bool> mem_update;
	sc_in<sc_int<16>> Dout;
	sc_in<bool> Dout_valid;
	sc_in<bool> IDP_ready;
	sc_in<sc_int<16>> Dout_pixel_value;
	sc_in<sc_int<16>> tracker_addr;
	sc_in<sc_int<16>> tracker_location;
	sc_in<bool> tracker_valid;
	sc_in<bool> response;
	
	///// tb & IDP handshake signal /////
	sc_in<bool> mac_response;

	///// IDP_Manager output port /////
	sc_out<sc_int<8>> IDP_addr;
	sc_out<bool> D_read;
	sc_out<bool> IDP_valid;
	sc_out<bool> Dout_ready;
	sc_out<sc_int<16>> pixel_location;
	sc_out<bool> request;
	sc_out<bool> tracker_ready;

	///// pixel_allocator to IDP input port /////
	sc_in<bool> IDP_data_ready;

	///// IDP to pixel_allocator output port /////
	sc_out<sc_int<16>> IDP_pixel;
	sc_out<sc_int<16>> IDP_loc_x;
	sc_out<sc_int<16>> IDP_loc_y;
	sc_out<bool> IDP_data_valid;


	void IDP_Manager_main();				// IDP_Manager main function
	void IDP_decode(int, int);				// decode function

	SC_CTOR(IDP_Manager) {
		SC_CTHREAD(IDP_Manager_main, clk.pos());
	}
};

sc_bv<16> bv_output_sparsity_map;
sc_int<16> output_sparsity_map;
sc_int<16> PIXEL;

void IDP_Manager::IDP_decode(int addr, int location) {
	///// handshake signal initialize /////
	IDP_valid.write(0);
	Dout_ready.write(0);
	D_read.write(0);
	wait();

	///// request to read data from pixel memory /////
	IDP_addr.write(addr);
	pixel_location.write(location);
	IDP_valid.write(1);
	D_read.write(1);

	do {
		wait();
	} while (!IDP_ready.read())
		;
	IDP_valid.write(0);
	D_read.write(0);
	wait();

	/////// wait until Dout is valid data ///////
	do {
		wait();
	} while (!Dout_valid.read())
		;

	/////// get pixel memory's result ///////
	bv_output_sparsity_map = Dout.read();
	output_sparsity_map = Dout.read();
	PIXEL = Dout_pixel_value.read();
	cout << "[IDP Manager]" << endl;
	cout << "sparsity map: " << bv_output_sparsity_map << endl;
	cout << "pixel value: " << PIXEL << endl << endl;
	

	IDP_pixel.write(PIXEL);
	IDP_loc_x.write(addr);
	IDP_loc_y.write(location);
	IDP_data_valid.write(1);
	do {
		wait();
	} while (!IDP_data_ready.read())
		;
	IDP_data_valid.write(0);

	Dout_ready.write(1);
	wait();

	///// initialize ready signal /////
	Dout_ready.write(0);
}


void IDP_Manager::IDP_Manager_main() {
	int addr, location;
	do {
		wait();
	} while (!mem_update.read())
		;

	for (int i = 0; i < 4; i++) {
		
		request.write(1);
		do {
			wait();
		} while (!response.read())
			;
		request.write(0);
		do {
			wait();
		} while (!tracker_valid.read())
			;
		tracker_ready.write(1);
		wait();
		tracker_ready.write(0);
		addr = tracker_addr.read();
		location = tracker_location.read();

		
		IDP_decode(addr, location);

		do {
			wait();
		} while (!mac_response.read())
			;
	}
}
