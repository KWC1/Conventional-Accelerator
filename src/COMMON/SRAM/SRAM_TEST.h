#pragma once
#include <systemc.h>
#include "SRAM.h"
#define TEST_SZ 512

SC_MODULE(SRAM_TEST){
  sc_in<sc_int<15>> addr;
  sc_inout<sc_uint<8>> io;

  sc_out<bool> ce_n;
  sc_out<bool> we_n;
  sc_out<bool> oe_n;


  SC_CTOR(SRAM_TEST){
      SC_THREAD(operation);//포트가 하나밖에 없기 때문에 Read/Write 동시에 진행하지 못함.
  }

  void operation() {
      //메모리 초기화
      ce_n.write(1);
      we_n.write(1);
      oe_n.write(1);
      io.write(0);//주의: 상대방이 쓸 때 값을 쓰면 안됨, 반대도 마찬가지

      //Write cycle 1(WE control)에 따라 Memory 값 초기화
      ce_n.write(0);//칩 활성화
      for(int i = 0; i < TEST_SZ; i++) {
          addr.write(i);//주소 Write (순차적으로)
          we_n.write(1);//Write Off
          wait(0);//tSA 시간 대기
          we_n.write(0);//Write On
          io.write(0);//0으로 초기화 한다.
          //wait(tPWE)
          //wait(70, SC_NS);//tWC 대기
      }

      //Read cycle에 따라 값 초기화 확인
      //Read cycle의 경우 간단히 Write만 off시킨 다음 tAA를 기다림으로써 데이터 읽기 가능
      sc_uint<8> tmp;
      we_n.write(0);//write off
      for(int i = 0; i < TEST_SZ; i++){
        addr.write(i);//주소 Write
        wait(tAA);//tAA 시간 대기
        tmp = io.read();//데이터 읽기
        if(tmp != 0) {
            cerr << "Read error, data is not zero when resetting at address:" << i << endl;
        }
        wait(tRC - tAA);//나머지 시간 대기
      }

      //전체에 대해 임의 Write (Write cycle 2)

      //다시 전체 읽음

      //테스트 종료
      sc_stop();
 }

 SC_MODULE(SRAM_TEST_SYS){
     sc_signal<sc_int<15>, SC_MANY_WRITERS> addr;
     sc_signal<sc_uint<8>> io;

     sc_signal<bool> ce_n;
     sc_signal<bool> we_n;
     sc_signal<bool> oe_n;

     SRAM* uudSRAM;
     SRAM_TEST* uudSRAM_TEST;

     SC_CTOR(SRAM_TEST_SYS) {
         uudSRAM = new SRAM("SRAM");
         uudSRAM_TEST = new uudSRAM_TEST("SRAM_TEST");

         uudSRAM->addr(addr);
         uudSRAM->io(io);
         uudSRAM->ce_n(ce_n);
         uudSRAM->we_n(we_n);
         uudSRAM->oe_n(oe_n);

         uudSRAM_TEST->addr(addr);
         uudSRAM_TEST->io(io);
         uudSRAM_TEST->ce_n(ce_n);
         uudSRAM_TEST->we_n(we_n);
         uudSRAM_TEST->oe_n(oe_n);
     }

     ~SRAM_TEST_SYS(){
         delete SRAM_TEST;
         delete SRAM;
     }
  }

};