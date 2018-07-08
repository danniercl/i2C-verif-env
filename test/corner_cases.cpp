#include "../sc_tb.h"

//-----------------------------
void suite_test::reset_test() {
   sc_uint<8> addr = 0x2; // Slave address
   env->drv->reset();
   sc_uint<8> data = 200;
   cout << "SENT BYTE: " << data << endl;
   sc_uint <8> received = 0x00;
   sc_uint<8> mem_addr = 0x01;

   // S E T  T H E  C O R E
   // *********************
   env->drv->core_enable();

   // W R I T E
   // *********
   env->drv->write_data(addr, mem_addr, data);
   wait(4);
   intf_int->wb_rst_i = true;
   wait(4);
   intf_int->wb_rst_i = false;
   env->drv->core_enable();
   wait(4);
   env->drv->read_data(addr, mem_addr);

   wait(10);
   // Request for simulation termination
   cout << "=======================================" << endl;
   cout << " SIMULATION END" << endl;
   cout << "=======================================" << endl;
   wait(1000);
  // Just wait for few cycles
}
