#ifndef SC_TB_TEST_H
#define SC_TB_TEST_H

#include "systemc.h"

SC_MODULE (interface) {

  /*************
   Input Signals
   *************/

  // Wishbone Signals
  // ****************
  sc_inout<bool>        wb_clk_i;     // master clock input
  sc_inout<sc_uint<8> > wb_dat_o;     // databus output
  sc_inout<bool>        wb_ack_o;     // bus cycle acknowledge  output
  sc_inout<bool>        wb_inta_o;    // interrupt request signal output

  // i2c Clock Line
  // **************
  sc_inout<bool> scl_pad_o;    // SCL-line output (always 1'b0)
  sc_inout<bool> scl_padoen_o; // SCL-line output enable (active low)

  // i2c Data Line
  // *************
  sc_inout<bool> sda_pad_o;    // SDA-line output (always 1'b0)
  sc_inout<bool> sda_padoen_o; // SDA-line output enable (active low)

  /***************
   * Output Signals
   ****************/
  // Wishbone Signals
  // ****************
  sc_inout<bool>        wb_rst_i;   // synchronous active high reset
  sc_inout<bool>        arst_i;     // asynchronous reset
  sc_inout<sc_uint<8> > wb_adr_i;   // lower address bits
  sc_inout<sc_uint<8> > wb_dat_i;   // databus input
  sc_inout<bool>        wb_we_i;    // write enable input
  sc_inout<bool>        wb_stb_i;   // stobe/core select signal
  sc_inout<bool>        wb_cyc_i;   // valid bus cycle input

  // i2c Clock Line
  // **************Read
  sc_inout<bool> scl_pad_i;  // SCL-line input

  // i2c Data Line
  // *************
  sc_inout<bool> sda_pad_i;  // SDA-line input

  sc_out<bool>  done; //Terminate sim

  SC_CTOR(interface) {

  }

};

SC_MODULE (driver) {

  interface *intf_int;

  SC_HAS_PROCESS(driver);
  driver(sc_module_name driver, interface *intf_ext) {
    //Interface
    intf_int = intf_ext;
  }

  void reset();
  void write(sc_uint<8>,sc_uint<8>);
  void read(sc_uint<8>);

};

SC_MODULE (monitor) {

  interface *intf_int;

  sc_uint<8> data_out_exp;
  sc_uint<8> data_out_read;

  SC_HAS_PROCESS(monitor);
  monitor(sc_module_name monitor, interface *intf_ext) {
    //Interface
    intf_int=intf_ext;
    SC_THREAD(mnt_out);
      sensitive << intf_int->wb_we_i.pos();
  }

  void mnt_out();

};

SC_MODULE (environment) {

  driver *drv;
  monitor *mnt;

  SC_HAS_PROCESS(environment);
  environment(sc_module_name environment, interface *intf_ext) {

    //Driver
    drv = new driver("drv",intf_ext);
    //Monitor
    mnt = new monitor("mnt",intf_ext);

  }
};

SC_MODULE (base_test) {

  interface *intf_int;
  environment *env;

  void test ();

  SC_HAS_PROCESS(base_test);
  base_test(sc_module_name base_test, interface *intf_ext) {
    intf_int = intf_ext;
    //environment
    env = new environment("env",intf_ext);
    SC_CTHREAD(test,intf_ext->wb_clk_i.pos());

  }
};

SC_MODULE (sc_tb) {

  base_test *test1;
  interface *intf;

  SC_CTOR(sc_tb) {
    intf = new interface("intf");
    test1 = new base_test("test1",intf);

  }
};

#endif
