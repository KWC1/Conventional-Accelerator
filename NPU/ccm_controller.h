#pragma once
#include <systemc.h>
#include <string>

SC_MODULE(ccm_controller) {
	sc_in<bool> clk;

	///// pixel_allocator to CCM controller input port /////
	sc_in<sc_int<16>> PA_pixel;
	sc_in<sc_int<16>> PA_loc_x;
	sc_in<sc_int<16>> PA_loc_y;
	sc_in<bool> PA_data_valid;

	///// CCM controller to pixel_allocator output port /////
	sc_out<bool> PA_data_ready;

	///// tb to MAC input port /////
	sc_in<bool> mac_result_ready;

	///// MAC to tb output port /////
	sc_out<sc_int<32>> mac_result;
	sc_out<bool> mac_result_valid;

	///// PRE to MAC input port /////
	sc_in<bool> DAT_in_ready;

	///// MAC to PRE output port /////
	sc_out<sc_int<16>> DAT_in;
	sc_out<bool> DAT_in_valid;

	///// ccm local variable /////
	sc_int<32> buff = 0;

	///// kernel memory global variable /////
	int mem_num = 0;

	///// mac global variable /////
	int mac_num = 0;

	///// ccm_controller internal function /////
	void ccm_controller_main();						// ccm_controller function

	void mac_main(sc_int<16>);						// mac controller function
	sc_int <32> buffer(sc_int <32>, bool);			// buffer module
	sc_int <32> adder(sc_int <32>);					// 32-bit fixed point adder module
	sc_int <32> multiplier(sc_int<16>, sc_int<16>);	// 16-bit fixed point multiplier
	sc_int <16> quantize(float);

	float kernel_memory();							// kernel memory function

	SC_CTOR(ccm_controller) {
		SC_CTHREAD(ccm_controller_main, clk.pos());
	}
};

sc_int <16> ccm_controller::quantize(float result) {
	sc_int <16> quantized_result;
	quantized_result = round(result * (2 ^ 15));
	quantized_result = quantized_result >> 15;

	return quantized_result;
}


sc_int <32> ccm_controller::multiplier(sc_int<16>pixel, sc_int<16>kernel) {
	sc_int <32> mul_result;

	mul_result = pixel * kernel;
	return mul_result;
}

sc_int <32> ccm_controller::buffer(sc_int <32>add_result, bool r) {

	if (r == 1) {
		return buff;
	}
	else if (r == 0) {
		buff = add_result;
	}
}

sc_int <32> ccm_controller::adder(sc_int <32>mul_result) {
	sc_int <32> add_result;
	sc_int <32> buff_result;


	buff_result = buffer(0, 1);					// read buffer's data

	add_result = mul_result + buff_result;		// adder

	buffer(add_result, 0);						// write data to buffer

	return add_result;
}

void ccm_controller::mac_main(sc_int<16> pixel) {
	float kernel;
	sc_int<32> input_result;
	sc_int<32> buff_result;

	mac_num++;

	////// get kernel value from kernel memory //////
	kernel = kernel_memory();

	////// MAC caculation //////
	cout << "[MAC]" << endl;
	cout << "buffer data: " << buff << endl;

	input_result = multiplier(pixel, kernel);
	buff_result = adder(input_result);

	cout << "MAC" << mac_num << "result: " << buff_result << endl << endl << endl;

	sc_int<16> quantized_data = quantize(buff_result);


	////// write MAC result to tb //////
	mac_result.write(buff_result);
	mac_result_valid.write(1);
	do {
		wait();
	} while (!mac_result_ready.read())
		;
	mac_result_valid.write(0);

	////// write MAC result to PRE //////
	DAT_in.write(buff_result);
	DAT_in_valid.write(1);
	do {
		wait();
	} while (!DAT_in_ready.read())
		;
	DAT_in_valid.write(0);

}

float ccm_controller::kernel_memory() {
	///// kernel_memory to MAC /////
	float kernel_value;
	mem_num += 2;
	kernel_value = mem_num;
	cout << "[Kernel memory]" << endl;
	cout << "Kernel memory result: " << kernel_value << endl << endl << endl;
	return kernel_value;
}


void ccm_controller::ccm_controller_main() {
	sc_int<16> PA_input_pixel;
	sc_int<16> PA_input_loc_x;
	sc_int<16> PA_input_loc_y;

	for (int i = 0; i < 4; i++) {
		///// get data from pixel allocator /////
		do {
			wait();
		} while (!PA_data_valid.read())
			;

		PA_input_pixel = PA_pixel.read();
		PA_input_loc_x = PA_loc_x.read();
		PA_input_loc_y = PA_loc_y.read();
		PA_data_ready.write(1);
		cout << endl << "[CCM controller]" << endl;
		cout << "Location of pixel: " << "(" << PA_input_loc_x << ", " << PA_input_loc_y << ") = " << PA_input_pixel << endl << endl << endl;

		///// write data(pixel value) to MAC /////
		for (int k = 0; k < 16; k++) {
			mac_main(PA_input_pixel);
		}
		
	}

}