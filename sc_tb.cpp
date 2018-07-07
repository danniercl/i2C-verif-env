#include "sc_tb.h"

/* Addresses to configuration */
#define PRER_LO  0
#define PRER_HI  1
#define CTR      2
#define RXR      3
#define TXR      3
#define CR       4
#define SR       4

/* Write/Read flags */
#define RD  1
#define WR  0

/* Register bits */
/* ConTRol Register */
enum ctr {
  // 0:5 Reserved
  CTR_IEN = 6,
  CTR_EN = 7
};

/* Command Register */
enum cr {
  CR_IACK = 0,
  // 2:1 Reserved
  CR_ACK = 3,
  CR_WR = 4,
  CR_RD = 5,
  CR_STO = 6,
  CR_STA = 7
};

/* Status Register */
enum sr {
  SR_IF = 0,
  SR_TIP = 1,
  // 4:2 Reserved
  SR_AL = 5,
  SR_BUSY = 6,
  SR_RXACK = 7
};

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
  cout << "WRITE: dato: " << data << " addr: " << addr << endl;
  wait(2);
  intf_int->wb_adr_i = addr;
  intf_int->wb_dat_i = data;
  wait(2);
  intf_int->wb_cyc_i= true;
  intf_int->wb_stb_i= true;
  intf_int->wb_we_i = true;

  // negate wishbone signals
  wait(2);
  //scb_int->fifo.write(intf_int->wb_dat_i);
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
  // assert wishbone signal
  intf_int->wb_adr_i = addr;
  wait(4);
}

void monitor::mnt_out(){
  //while(true){
  wait(2);
  data_out_exp =  intf_int->wb_dat_o;
  //data_out_exp =  scb_int->fifo.read();;
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

// This is a Basic Write/Read test
void base_test::test() {
   // Generate address (ID) of the slave
   sc_uint<8> addr = env->drv->stim_gen_inst->addr_rnd_gen(); // Slave address
   env->drv->reset();
   sc_uint<8> data = env->drv->stim_gen_inst->data_rnd_gen(); // Generate random data to send
   cout << "SENT BYTE: " << data << endl;

   // R E S E T
   // *********
   intf_int->done = 0;
   wait(10);

   // S E T  T H E  C O R E
   // *********************
   env->drv->write(0x0A, PRER_LO); // load prescaler lo-byte
   env->drv->write(0x00, PRER_HI); // load prescaler hi-byte
   env->drv->write(0x80, CTR); // enable core

   // W R I T E
   // *********
   env->drv->write((addr << 1) | WR, TXR); // present slave address, set write-bit
   env->drv->write(0x90, CR);              // set command (start, write)

   env->drv->write(0x01, TXR); // present slave memory address
   env->drv->write(0x10, CR);  // set command (write)
   env->drv->write(data, TXR); // present the data
   env->drv->write(0x10, CR);  // set command (write)

   env->drv->write(0x40, CR); // Stop

   wait(1000);

   // R E A D
   // *******
   env->drv->write((addr << 1) | RD, TXR); // present slave address, set read-bit
   env->drv->write(0x90, CR);              // set command (start, write)
   env->drv->write(0x01, TXR);             // present slave memory address
   env->drv->write(0x10, CR);              // set command (write)
   env->drv->write(0x20, CR);              // set command (read)
   env->drv->read(RXR);                    // read the register
   cout << "RECEIVED BYTE: " << intf_int->wb_dat_o << endl;
   env->drv->write(0x40, CR); // Stop

   wait(10);

   env->mnt->mnt_out();
  wait(10);
  // Request for simulation termination
  cout << "=======================================" << endl;
  cout << " SIMULATION END" << endl;
  cout << "=======================================" << endl;
  wait(1000);
  intf_int->done = 1;
  // Just wait for few cycles
}
