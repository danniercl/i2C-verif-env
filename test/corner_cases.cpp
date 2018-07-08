#include "../sc_tb.h"

//-----------------------------
void suite_test::reset_test() {
  cout << "***************************************" << endl;
  cout << " TEST: RESET TEST" << endl;
  cout << "***************************************" << endl;
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
   /* Store value in expected fifo */
   env->drv->scb_int->expected_fifo.nb_write(data);
   env->drv->write_data(addr, mem_addr, data);
   wait(4);
   intf_int->wb_rst_i = true;
   wait(4);
   intf_int->wb_rst_i = false;
   env->drv->core_enable();
   wait(4);
  received = env->drv->read_data(addr, mem_addr);
  /* Store received value in received fifo*/
  env->drv->scb_int->received_fifo.nb_write(received);

   wait(10);
   // Request for simulation termination
   cout << "=======================================" << endl;
   cout << " SIMULATION ENDED" << endl;
   cout << "=======================================" << endl;
   wait(1000);
  // Just wait for few cycles
}
