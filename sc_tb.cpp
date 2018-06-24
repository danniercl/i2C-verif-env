#include "sc_tb.h"
#define SLAVE_ADDR 2

/* Addresses to configuration */
#define PRER_LO  0
#define PRER_HI  1
#define CTR      2
#define RXR      3
#define TXR      4
#define CR       5
#define SR       6

/* Write/Read flags */
#define RD  1
#define WR  0

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
  // assert wishbone signal
  wait(2);
  //cout << "WRITE: dato: " << dato << " addr: " << addr << endl;
  wait(2);
  intf_int->wb_adr_i = addr;
  intf_int->wb_dat_i = data;
  wait(2);
  intf_int->wb_cyc_i= true;
  intf_int->wb_stb_i= true;
  intf_int->wb_we_i = true;

  //while(intf_int->wb_clk_i){}
  cout << "Waiting ack: " << endl;
  // wait for acknowledge from slave
  while(intf_int->wb_ack_o){}
  // negate wishbone signals
  wait(2);
  intf_int->wb_cyc_i= false;
  intf_int->wb_stb_i= false;
  intf_int->wb_adr_i  = addr;
	//intf_int->wb_dat_i = rand();
	intf_int->wb_we_i  = false;
  cout<<"@"<<sc_time_stamp()<<" Writing " << endl;
}

void driver::read(sc_uint<8> addr){
  cout<<"@"<<sc_time_stamp()<<" Reading " << endl;

  // assert wishbone signal
  wait(2);
  intf_int->wb_adr_i = addr;
  //intf_int->wb_dat_o = rand();
  intf_int->wb_cyc_i= true;
  intf_int->wb_stb_i= true;
  intf_int->wb_we_i = false;

  //while(intf_int->wb_clk_i){}

  // wait for acknowledge from slave
  while(intf_int->wb_ack_o){}
  // negate wishbone signals
  wait(2);
 	intf_int->wb_cyc_i= false;
  intf_int->wb_stb_i= false;
 	//intf_int->wb_adr_i= rand();
	//intf_int->wb_dat_o = rand();
	intf_int->wb_we_i = true;
}

void monitor::mnt_out(){
  //while(true){
  wait(2);
  data_out_exp =  intf_int->wb_dat_o;
  data_out_read = intf_int->wb_dat_i;
  // cout<<"@"<<sc_time_stamp()<<" data_out_exp:" << data_out_exp << endl;
  // cout<<"@"<<sc_time_stamp()<<" data_out_read:" << data_out_read << endl;
  //Checker
  //if (data_out_exp != data_out_read)
  //  cout<<"@"<<sc_time_stamp()<<" ERROR: data read and expected mismatch!" << endl;
  //else
  //  cout<<"@"<<sc_time_stamp()<<" SIRVE PAPIS; SIRVE!!!" << endl;
  //}
}

//Test
void base_test::test() {
   intf_int->done = 0;
   env->drv->reset();
   wait(10);

   // Program Internal Registers
   env->drv->write(0xc8, PRER_LO); // load prescaler lo-byte
   env->drv->write(0x00, PRER_HI); // load prescaler hi-byte
   env->drv->write(0x80, CTR); // enable core

   // Drive data write
   env->drv->write((SLAVE_ADDR << 1) | WR, TXR); // present slave address, set write-bit
   env->drv->write(0x90, CR); // set command (start, write)

   // Send Memory address
   env->drv->write(0x01, TXR); // present slave's memory address
   env->drv->write(0x90, CR); // set command (write)

   // Send Data
   env->drv->write(0xa5, TXR); // present slave's memory address
   env->drv->write(0x10, CR); // set command (write)

   intf_int->wb_dat_i = env->drv->stim_gen_inst->data_rnd_gen();
   wait(1);

   sc_uint<8> addr = SLAVE_ADDR;
   cout<<" TEST: Dato: " << intf_int->wb_dat_i << " Dirección: " << addr << endl;
   env->drv->write(intf_int->wb_dat_i, addr);
   wait(2);
   //env->drv->read(addr);
   wait(2);
   env->mnt->mnt_out();
  wait(10);
  // Request for simulation termination
  cout << "=======================================" << endl;
  cout << " SIMULATION END" << endl;
  cout << "=======================================" << endl;
  wait(10);
  intf_int->done = 1;
  // Just wait for few cycles
}
