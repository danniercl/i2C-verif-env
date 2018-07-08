#include "sc_tb.h"

void driver::reset(){
  stim_gen_inst->init_seed_gen();
  intf_int->wb_rst_i = false;
  intf_int->arst_i = true;
  cout<<"@"<<sc_time_stamp()<<" Started Reset " << endl;
  wait(4);
  intf_int->arst_i = false;
  wait(4);
  intf_int->arst_i = true;
  cout<<"@"<<sc_time_stamp()<<" Finished Reset " << endl;
}

void driver::write(sc_uint<8> data, sc_uint<8> addr){
  wait(2);
  cout << "WRITE: data: " << data << " addr: " << addr << endl;
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
  intf_int->wb_adr_i  = addr;

	intf_int->wb_we_i  = false;
  cout<<"@"<<sc_time_stamp()<<" Writing " << endl;

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
  cout<<"@"<<sc_time_stamp()<<" Reading " << endl;
  intf_int->wb_adr_i = addr;
  wait(4);
}


void driver::write_data(sc_uint<8> addr, sc_uint<8> mem_addr, sc_uint<8> data){
   scb_int->fifo.write(data);
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
   cout << "RECEIVED BYTE: " << received << endl;
   write(0x40, CR); // Stop
   return received;
}

void driver::core_enable(){
   write(0x0A, PRER_LO); // load prescaler lo-byte
   write(0x00, PRER_HI); // load prescaler hi-byte
   write(0x80, CTR);     // enable core
}

void monitor::mnt_out(sc_uint<8> received){
  wait(2);
  cout<<"@"<<sc_time_stamp()<<" entered scoreboard!" << endl;
  num_available= scb_int->fifo.num_available();
  if (num_available > 0 ){
  data_out_read = received;
  data_out_exp =  scb_int->fifo.read();;
  cout<<"@"<<sc_time_stamp()<<" Data expected: " << data_out_exp << endl;
  cout<<"@"<<sc_time_stamp()<<" Data received: " << data_out_read << endl;
  // Checker
  //if (data_out_exp != data_out_read)
  //  cout<<"@"<<sc_time_stamp()<<" ERROR: data read and expected mismatch!" << endl;
  //else
  //  cout<<"@"<<sc_time_stamp()<<" SIRVE PAPIS; SIRVE!!!" << endl;
  //}
}
}
