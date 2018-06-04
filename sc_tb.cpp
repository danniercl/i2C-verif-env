#include "sc_tb.h"
#define ADDR_SLAVE 16
  void driver::reset(){
    intf_int->wb_rst_i = true;
    cout<<"@"<<sc_time_stamp()<<" Started Reset " << endl;
    wait(10);
    intf_int->wb_rst_i = false;
    intf_int->wb_stb_i = true;
    cout<<"@"<<sc_time_stamp()<<" Finished Reset " << endl;
  }

  void driver::write(){
    cout<<"@"<<sc_time_stamp()<<" Writing " << endl;
    intf_int->wb_dat_i = (uint8_t)rand();
    intf_int->wb_adr_i = ADDR_SLAVE;
    wait(1);
    intf_int->wb_dat_i = 0;
    intf_int->wb_adr_i = 0;
    intf_int->wb_we_i = false;
  }

  void driver::read(){
    cout<<"@"<<sc_time_stamp()<<" Reading " << endl;
    intf_int->wb_adr_i = ADDR_SLAVE;
    wait(1);
    intf_int->wb_adr_i = 0;
  }

  void monitor::mnt_out(){
    while(true){
    wait(1);
    // No estoy seguro de esta siguiente línea
    data_out_exp = intf_int->wb_dat_o; // scb_int->fifo.read();
    data_out_read = intf_int->wb_dat_o;
    cout<<"@"<<sc_time_stamp()<<" Monitor data_out:" << data_out_exp << endl;
    cout<<"@"<<sc_time_stamp()<<" Scoreboard data_out:" << data_out_read << endl;
    //Checker, No es necesario aun
    //if (data_out_exp != data_out_read)
    //  //assert(data_out_exp == data_out_read);
    //  cout<<"@"<<sc_time_stamp()<<" ERROR: data read and expected mismatch!" << endl;
    }
  }


  //Test
  void base_test::test() {
    intf_int->done = 0;
    env->drv->reset();
    wait(10);
    env->drv->write();
    wait(10);
    env->drv->read();
    wait(10);
    // Request for simulation termination
    cout << "=======================================" << endl;
    cout << " SIMULATION END" << endl;
    cout << "=======================================" << endl;
    wait(10);
    intf_int->done = 1;
    // Just wait for few cycles
  }
