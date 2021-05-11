#include <systemc.h>
#include "Pixel_Memory.h"
#include "Input_Tracker.h"
#include "IDP_Manager.h"
#include "NPU_controller.h"
#include "pixel_allocator.h"
#include "ccm_controller.h"
#include "PRE.h"
#include "SYSTEM.h"
#include "tb.h"


SYSTEM *top = NULL;

int sc_main(int argc, char* argv[]) {
	sc_set_time_resolution(1, SC_PS);
	top = new SYSTEM("top");
	sc_start();
	return 0;
}
