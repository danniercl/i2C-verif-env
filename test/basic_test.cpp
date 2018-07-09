#include "../sc_tb.h"

// This is a Basic Write/Read test
void suite_test::basic_test() {
  cout << "*****************" << endl;
  cout << " TEST: BASIC TEST" << endl;
  cout << "*****************" << endl;
   // Generate address (ID) of the slave
   sc_uint<8> addr = 0x2;
   env->drv->reset();
   // Generate random data to send
   sc_uint<8> data = 200;
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
   /* Store value in expected fifo */
   env->drv->scb_int->expected_fifo.nb_write(data);
   env->drv->write_data(addr, mem_addr, data);
   wait(1000);

   // R E A D
   // *******
   received = env->drv->read_data(addr, mem_addr);
   /* Store received value in received fifo*/
   env->drv->scb_int->received_fifo.nb_write(received);
   wait(10);

   env->mnt->mnt_out();
   wait(1000);
  // Just wait for few cycles
}
