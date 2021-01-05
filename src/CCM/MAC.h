#include "../COMMON/COMMON.h"
#include <systemc.h>

SC_MODULE(MAC){
private:
    sc_in<sc_uint<32>> opA;
    sc_in<sc_uint<32>> opB;
    sc_out<sc_uint<32>> res;

    sc_in<sc_uint<1>> op_val;
    sc_out<sc_uint<1>> op_rdy;

    sc_uint<1> res_val;
    sc_uint<1> res_rdy;


public:
  MAC(){
  }

  ~MAC(){
  }

  void calc(){
      int a = sc_uint<;
      int b = (read port);

  }
};