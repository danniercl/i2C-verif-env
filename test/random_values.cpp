#include "../sc_tb.h"

// This is a test using random address slave
void suite_test::random_addr() {
  cout << "*********************" << endl;
  cout << " TEST: RANDOM ADDRESS" << endl;
  cout << "*********************" << endl;
   env->drv->reset();
   // Generate address (ID) of the slave
   sc_uint<8> addr = env->drv->stim_gen_inst->addr_rnd_gen(); // Slave address
   sc_uint<8> data = 250;
   sc_uint<8> received = 0x00;
   sc_uint<8> mem_addr = 0x01;
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
     cout << "=====================================================" << endl;
     cout << "ERROR CATCHED: Slave addr " << addr << " is not present" << endl;
     cout << "=====================================================" << endl;
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
   wait(1000);
  // Just wait for few cycles
}


// This is a test using random memory address for the RAM in the slave
void suite_test::random_mem_addr() {
  cout << "****************************" << endl;
  cout << " TEST: RANDOM MEMORY ADDRESS" << endl;
  cout << "****************************" << endl;
   sc_uint<8> addr = 0x2; // Slave address
   env->drv->reset();
   sc_uint<8> data = 250;
   sc_uint <8> received = 0x00;
   sc_uint<8> mem_addr =  env->drv->stim_gen_inst->mem_addr_rnd_gen();

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


// This is a test using random data to send
void suite_test::random_data() {
  cout << "******************" << endl;
  cout << " TEST: RANDOM DATA" << endl;
  cout << "******************" << endl;
   sc_uint<8> addr = 0x2; // Slave address
   env->drv->reset();
   sc_uint<8> data = env->drv->stim_gen_inst->data_rnd_gen();
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

// This is a test using random all the values
void suite_test::random_all() {
  cout << "*****************" << endl;
  cout << " TEST: RANDOM ALL" << endl;
  cout << "*****************" << endl;
   env->drv->reset();
   // Generate address (ID) of the slave
   sc_uint<8> addr = env->drv->stim_gen_inst->addr_rnd_gen(); // Slave address
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
   wait(1000);
  // Just wait for few cycles
}
