#include "../sc_tb.h"

void suite_test::two_slave() {
  cout << "*********************" << endl;
  cout << " TEST: TWO SLAVE TEST" << endl;
  cout << "*********************" << endl;

  env->drv->reset();

  /////////////////////////////////////
  // FIRST SLAVE TEST
  /////////////////////////////////////
  sc_uint<8> addr = 0x02; // First slave address
  sc_uint<8> data = env->drv->stim_gen_inst->data_rnd_gen();
  sc_uint<8> received = 0x00;
  sc_uint<8> mem_addr = env->drv->stim_gen_inst->mem_addr_rnd_gen();
  sc_uint<8> ack_mask = 1 << SR_RXACK;

  // R E S E T
  // *********
  wait(10);

  // S E T  T H E  C O R E
  // *********************
  env->drv->core_enable();

  // W R I T E
  // *********
  // Verification ack slave
  env->drv->write((addr << 1) | WR, TXR); // present slave address, set write-bit
  env->drv->write(0x90, CR);              // set command (start, write)
  wait(4);
  env->drv->read(SR);

  if((addr != 2) && ((sc_uint<8>) intf_int->wb_dat_o & ack_mask)){
    cout << "========================================" << endl;
    cout << "ERROR CATCHED: Slave addr " << addr << " is not present" << endl;
    cout << "========================================" << endl;
    env->mnt->mnt_out();
    return;
  }
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
  wait(100);
 // Just wait for few cycles

 /////////////////////////////////////
 // SECOND SLAVE TEST
 /////////////////////////////////////
 addr = 0x04; // Second slave address
 data = env->drv->stim_gen_inst->data_rnd_gen();
 received = 0x00;
 mem_addr = env->drv->stim_gen_inst->mem_addr_rnd_gen();
 ack_mask = 1 << SR_RXACK;

 // R E S E T
 // *********
 wait(10);

 // S E T  T H E  C O R E
 // *********************
 env->drv->core_enable();

 // W R I T E
 // *********
 // Verification ack slave
 env->drv->write((addr << 1) | WR, TXR); // present slave address, set write-bit
 env->drv->write(0x90, CR);              // set command (start, write)
 wait(4);
 env->drv->read(SR);

 if((addr != 2) && ((sc_uint<8>) intf_int->wb_dat_o & ack_mask)){
   cout << "========================================" << endl;
   cout << "ERROR CATCHED: Slave addr " << addr << " is not present" << endl;
   cout << "========================================" << endl;
   env->mnt->mnt_out();
   return;
 }
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
 wait(100);
// Just wait for few cycles
}
