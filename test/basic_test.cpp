#include "../sc_tb.h"

// This is a Basic Write/Read test
void suite_test::basic_test() {
   // Generate address (ID) of the slave
   sc_uint<8> addr = 0x2;
   env->drv->reset();
   // Generate random data to send
   sc_uint<8> data = 200;
   cout << "SENT BYTE: " << data << endl;
   sc_uint <8> received = 0x00;
   sc_uint<8> mem_addr = 0x01;

   // R E S E T
   // *********
   wait(10);

   // S E T  T H E  C O R E
   // *********************
   env->drv->core_enable();

   // W R I T E
   // *********
   env->drv->write_data(addr, mem_addr, data);
   wait(1000);

   // R E A D
   // *******
   received = env->drv->read_data(addr, mem_addr);
   wait(10);

   env->mnt->mnt_out(received);
   wait(10);
   // Request for simulation termination
   cout << "=======================================" << endl;
   cout << " SIMULATION END" << endl;
   cout << "=======================================" << endl;
   wait(1000);
  // Just wait for few cycles
}
