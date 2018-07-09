#include "sc_tb.h"

void driver::reset(){
  stim_gen_inst->init_seed_gen();
  intf_int->wb_rst_i = false;
  intf_int->arst_i = true;
  cout<<"@"<<sc_time_stamp()<<" Starting Reset " << endl;
  wait(4);
  intf_int->arst_i = false;
  wait(4);
  intf_int->arst_i = true;
  cout<<"@"<<sc_time_stamp()<<" Finished Reset " << endl;
}

void driver::write(sc_uint<8> data, sc_uint<8> addr){
  wait(2);
  intf_int->wb_adr_i = addr;
  intf_int->wb_dat_i = data;
  wait(2);
  intf_int->wb_cyc_i= true;
  intf_int->wb_stb_i= true;
  intf_int->wb_we_i = true;

  // negate wishbone signals
  wait(2);
  intf_int->wb_cyc_i= false;
  intf_int->wb_stb_i= false;

	intf_int->wb_we_i  = false;

  sc_uint<8> need_wait_ack = data & ((1 << CR_WR) |
                                     (1 << CR_RD) |
                                     (1 << CR_STO) |
                                     (1 << CR_STA));

  if (CR == addr && need_wait_ack) {
    while(false == intf_int->wb_ack_o){
      wait(1);
    }
  }
}

void driver::read(sc_uint<8> addr){
  intf_int->wb_adr_i = addr;
  wait(4);
}


void driver::write_data(sc_uint<8> addr, sc_uint<8> mem_addr, sc_uint<8> data){
   write((addr << 1) | WR, TXR); // present slave address, set write-bit
   write(0x90, CR);              // set command (start, write)
   write(mem_addr, TXR);         // present slave memory address
   write(0x10, CR);              // set command (write)
   write(data, TXR);             // present the data
   write(0x10, CR);              // set command (write)
   write(0x40, CR);              // Stop
}

sc_uint<8> driver::read_data(sc_uint<8> addr, sc_uint<8> mem_addr){
   write((addr << 1) | RD, TXR); // present slave address, set read-bit
   write(0x90, CR);              // set command (start, write)
   write(mem_addr, TXR);         // present slave memory address
   write(0x10, CR);              // set command (write)
   write(0x20, CR);              // set command (read)
   read(RXR);                    // read the register
   sc_uint<8> received = intf_int->wb_dat_o;
   write(0x40, CR); // Stop
   return received;
}

void driver::core_enable(){
   write(0x0A, PRER_LO); // load prescaler lo-byte
   write(0x00, PRER_HI); // load prescaler hi-byte
   write(0x80, CTR);     // enable core
}

void monitor::mnt_out(){
  string red("\033[0;31m");
  string green("\033[1;32m");
  string reset("\033[0m");
  //while(true){
  wait(2);
  bool failed_test;
  // num_free = scb_int->expected_fifo.num_free();
  //  cout << "fifo size: " << num_free << endl;
  cout<<"@"<<sc_time_stamp()<<" entered scoreboard!" << endl;
  num_available= scb_int->expected_fifo.num_available();
  for (sc_uint<8> j = 0; j < num_available ; j ++){
  data_out_read = scb_int->received_fifo.read();;
  data_out_exp  =  scb_int->expected_fifo.read();
  //Checker
  if (data_out_exp != data_out_read){
   cout<<"@"<<sc_time_stamp() <<" ERROR:  read data and expected data mismatch!" << "at iteration # "
   << j+1 << endl;
   failed_test = true;
 }
  else
  cout<<"@" <<sc_time_stamp() <<  " Iteration # "<< j+1 <<" data_out_exp: " << data_out_exp
  << " data_out_read: "<< data_out_read << endl;
  }
  if (failed_test) {
  cout << red << "=============" << endl;
  cout <<        " TEST FAILED!" << endl;
  cout        << "=============" << reset << endl;
} else {
  cout << green << "=============" << endl;
  cout          << " TEST PASSED!" << endl;
  cout          << "=============" << reset << endl;
  }
}
