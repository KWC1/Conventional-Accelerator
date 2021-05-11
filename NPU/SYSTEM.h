#pragma once
#include <systemc.h>
#include "Pixel_Memory.h"
#include "Input_Tracker.h"
#include "IDP_Manager.h"
#include "NPU_controller.h"
#include "pixel_allocator.h"
#include "ccm_controller.h"
#include "PRE.h"
#include "tb.h"


SC_MODULE(SYSTEM) {
	///// pointer of module /////
	NPU_cont* NPU_cont0;
	Pixel_Memory* pixel_memory0;
	IDP_Manager* idp0;
	Input_tracker* tracker0;
	pixel_allocator* PA0;
	ccm_controller* CCM0;
	PRE* PRE0;
	tb* tb0;

	///// Externel DRAM, Pixel Memory, IDP Manager /////
	sc_signal<sc_int<8>> sig_DRAM_addr;
	sc_signal<sc_int<8>> sig_IDP_addr;
	sc_signal<sc_int<16>> sig_Din;
	sc_signal<bool> sig_D_write;
	sc_signal<bool> sig_D_read;
	sc_signal<bool> sig_DRAM_valid;
	sc_signal<bool> sig_IDP_valid;
	sc_signal<bool> sig_Dout_ready;
	sc_signal<sc_int<16>> sig_Dout;
	sc_signal<bool> sig_Dout_valid;
	sc_signal<bool> sig_DRAM_ready;
	sc_signal<bool> sig_IDP_ready;
	sc_signal<bool> sig_mem_update;
	sc_signal<sc_int<16>> sig_Dout_pixel_value;
	sc_signal<sc_int<16>> sig_pixel_location;
	sc_signal<sc_int<16>> sig_tracker_addr;
	sc_signal<sc_int<16>> sig_tracker_location;
	sc_signal<bool> sig_tracker_valid;
	sc_signal<bool> sig_tracker_ready;
	sc_signal<bool> sig_request;
	sc_signal<bool> sig_response;

	///// signal for connect IDP and pixel allocator /////
	sc_signal<sc_int<16>> sig_IDP_pixel;
	sc_signal<sc_int<16>> sig_IDP_loc_x;
	sc_signal<sc_int<16>> sig_IDP_loc_y;
	sc_signal<bool> sig_IDP_data_ready;
	sc_signal<bool> sig_IDP_data_valid;

	///// signal for connect pixel allocator and CCM controller /////
	sc_signal<sc_int<16>> sig_PA_pixel;
	sc_signal<sc_int<16>> sig_PA_loc_x;
	sc_signal<sc_int<16>> sig_PA_loc_y;
	sc_signal<bool> sig_PA_data_ready;
	sc_signal<bool> sig_PA_data_valid;

	///// signal for connect CCM controller and MAC /////
	sc_signal<sc_int<16>> sig_CCM_pixel;
	sc_signal<bool> sig_CCM_data_ready;
	sc_signal<bool> sig_CCM_data_valid;

	///// signal for connect kernel memory and MAC /////
	sc_signal<sc_int<16>> sig_SRAM_kernel;
	sc_signal<bool> sig_SRAM_kernel_ready;
	sc_signal<bool> sig_SRAM_kernel_valid;

	///// signal for connect MAC and tb /////
	sc_signal<sc_int<32>> sig_mac_result;
	sc_signal<bool> sig_mac_result_ready;
	sc_signal<bool> sig_mac_result_valid;

	///// signal for IDP and testbench(mac) /////
	sc_signal<bool> sig_mac_response;

	///// signal for PRE and MAC /////
	sc_signal<bool> sig_DAT_in_ready;
	sc_signal<bool> sig_DAT_in_valid;
	sc_signal<sc_int<16>> sig_DAT_in;

	///// constructor /////
	sc_clock clk_sig;
	SC_CTOR(SYSTEM) : clk_sig("clk_sig", 1, SC_NS) {

		///// connecting module /////
		NPU_cont0 = new NPU_cont("NPU_cont0");
		NPU_cont0->clk(clk_sig);
		NPU_cont0->init_data_ready(sig_DRAM_ready);
		NPU_cont0->init_data_addr(sig_DRAM_addr);
		NPU_cont0->init_Din(sig_Din);
		NPU_cont0->init_data_write(sig_D_write);
		NPU_cont0->init_data_valid(sig_DRAM_valid);
		NPU_cont0->mem_update(sig_mem_update);

		pixel_memory0 = new Pixel_Memory("pixel_memory0");
		pixel_memory0->clk(clk_sig);
		pixel_memory0->DRAM_addr(sig_DRAM_addr);
		pixel_memory0->IDP_addr(sig_IDP_addr);
		pixel_memory0->Din(sig_Din);
		pixel_memory0->D_write(sig_D_write);
		pixel_memory0->D_read(sig_D_read);
		pixel_memory0->DRAM_valid(sig_DRAM_valid);
		pixel_memory0->IDP_valid(sig_IDP_valid);
		pixel_memory0->Dout_ready(sig_Dout_ready);
		pixel_memory0->Dout(sig_Dout);
		pixel_memory0->Dout_valid(sig_Dout_valid);
		pixel_memory0->DRAM_ready(sig_DRAM_ready);
		pixel_memory0->IDP_ready(sig_IDP_ready);
		pixel_memory0->pixel_location(sig_pixel_location);
		pixel_memory0->Dout_pixel_value(sig_Dout_pixel_value);

		idp0 = new IDP_Manager("idp0");
		idp0->clk(clk_sig);
		idp0->mem_update(sig_mem_update);
		idp0->Dout(sig_Dout);
		idp0->Dout_valid(sig_Dout_valid);
		idp0->IDP_ready(sig_IDP_ready);
		idp0->IDP_addr(sig_IDP_addr);
		idp0->D_read(sig_D_read);
		idp0->IDP_valid(sig_IDP_valid);
		idp0->Dout_ready(sig_Dout_ready);
		idp0->pixel_location(sig_pixel_location);
		idp0->Dout_pixel_value(sig_Dout_pixel_value);
		idp0->tracker_addr(sig_tracker_addr);
		idp0->tracker_location(sig_tracker_location);
		idp0->tracker_valid(sig_tracker_valid);
		idp0->request(sig_request);
		idp0->tracker_ready(sig_tracker_ready);
		idp0->response(sig_response);
		idp0->IDP_pixel(sig_IDP_pixel);
		idp0->IDP_loc_x(sig_IDP_loc_x);
		idp0->IDP_loc_y(sig_IDP_loc_y);
		idp0->IDP_data_ready(sig_IDP_data_ready);
		idp0->IDP_data_valid(sig_IDP_data_valid);
		idp0->mac_response(sig_mac_response);

		tracker0 = new Input_tracker("tracker0");
		tracker0->clk(clk_sig);
		tracker0->tracker_addr(sig_tracker_addr);
		tracker0->tracker_location(sig_tracker_location);
		tracker0->tracker_valid(sig_tracker_valid);
		tracker0->request(sig_request);
		tracker0->tracker_ready(sig_tracker_ready);
		tracker0->response(sig_response);

		PA0 = new pixel_allocator("PA0");
		PA0->clk(clk_sig);
		PA0->IDP_pixel(sig_IDP_pixel);
		PA0->IDP_loc_x(sig_IDP_loc_x);
		PA0->IDP_loc_y(sig_IDP_loc_y);
		PA0->IDP_data_ready(sig_IDP_data_ready);
		PA0->IDP_data_valid(sig_IDP_data_valid);

		PA0->PA_pixel(sig_PA_pixel);
		PA0->PA_loc_x(sig_PA_loc_x);
		PA0->PA_loc_y(sig_PA_loc_y);
		PA0->PA_data_ready(sig_PA_data_ready);
		PA0->PA_data_valid(sig_PA_data_valid);

		CCM0 = new ccm_controller("CCM0");
		CCM0->clk(clk_sig);
		CCM0->PA_pixel(sig_PA_pixel);
		CCM0->PA_loc_x(sig_PA_loc_x);
		CCM0->PA_loc_y(sig_PA_loc_y);
		CCM0->PA_data_ready(sig_PA_data_ready);
		CCM0->PA_data_valid(sig_PA_data_valid);
		CCM0->mac_result(sig_mac_result);
		CCM0->mac_result_ready(sig_mac_result_ready);
		CCM0->mac_result_valid(sig_mac_result_valid);
		CCM0->DAT_in(sig_DAT_in);
		CCM0->DAT_in_ready(sig_DAT_in_ready);
		CCM0->DAT_in_valid(sig_DAT_in_valid);

		PRE0 = new PRE("PRE0");
		PRE0->clk(clk_sig);
		PRE0->DAT_in(sig_DAT_in);
		PRE0->DAT_in_ready(sig_DAT_in_ready);
		PRE0->DAT_in_valid(sig_DAT_in_valid);

		tb0 = new tb("tb0");
		tb0->clk(clk_sig);
		tb0->mac_result(sig_mac_result);
		tb0->mac_result_ready(sig_mac_result_ready);
		tb0->mac_result_valid(sig_mac_result_valid);
		tb0->mac_response(sig_mac_response);

	}

	///// destructor /////
	~SYSTEM() {
		delete NPU_cont0;
		delete pixel_memory0;
		delete idp0;
		delete tracker0;
		delete PA0;
		delete CCM0;
		delete PRE0;
		delete tb0;
	}
};

