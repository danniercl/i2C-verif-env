#include "../sc_tb.h"

// This is a Basic Write/Read test
void base_test::test() {
   // Generate address (ID) of the slave
   sc_uint<8> addr = env->drv->stim_gen_inst->addr_rnd_gen(); // Slave address
   env->drv->reset();
   sc_uint<8> data = env->drv->stim_gen_inst->data_rnd_gen(); // Generate random data to send
   cout << "SENT BYTE: " << data << endl;
   sc_uint <8> received = 0x00;
   sc_uint<8> mem_addr = 0x01;

   // R E S E T
   // *********
   intf_int->done = 0;
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
   intf_int->done = 1;
  // Just wait for few cycles
}
