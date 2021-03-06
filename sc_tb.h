#ifndef SC_TB_TEST_H
#define SC_TB_TEST_H

#include "systemc.h"
#include "scv.h"

#define MAX_VALID_DATA 255
#define MIN_VALID_DATA 0
#define MAX_VALID_ADDR 3
#define MIN_VALID_ADDR 2
#define MAX_VALID_MEM_ADDR 255
#define MIN_VALID_MEM_ADDR 0

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
  // sc_inout<bool> sda_pad_i;  // SDA-line input
  sc_out<bool>  done; //Terminate sim

  SC_CTOR(interface) {

  }

};

//Scoreboard
SC_MODULE (scoreboard) {

  sc_fifo<sc_uint<8> > received_fifo ;
    sc_fifo<sc_uint<8> > expected_fifo;

  SC_CTOR(scoreboard) {
    sc_fifo<sc_uint<8> > received_fifo (100); //FIXME this should be dynamic allocation.
    sc_fifo<sc_uint<8> > expected_fifo (100);
  }
};

// Random data using SCV
class data_rnd_constraint : public scv_constraint_base {
public:
  scv_smart_ptr< sc_uint<8> > data;
  SCV_CONSTRAINT_CTOR(data_rnd_constraint) {
    // Hard Constraint
    SCV_CONSTRAINT ( data() <= MAX_VALID_DATA ); // Max
    SCV_CONSTRAINT ( data() >= MIN_VALID_DATA ); // Min
  }
};

// Random addr using SCV
class addr_rnd_constraint : public scv_constraint_base {
public:
  scv_smart_ptr< sc_uint<8> > addr;
  SCV_CONSTRAINT_CTOR(addr_rnd_constraint) {
    // Hard Constraint
    SCV_CONSTRAINT ( addr() <= MAX_VALID_ADDR ); // Max
    SCV_CONSTRAINT ( addr() >= MIN_VALID_ADDR ); // Min
  }
};

// Random mem_addr using SCV
class mem_addr_rnd_constraint : public scv_constraint_base {
public:
  scv_smart_ptr< sc_uint<8> > mem_addr;
  SCV_CONSTRAINT_CTOR(mem_addr_rnd_constraint) {
    // Hard Constraint
    SCV_CONSTRAINT ( mem_addr() <= MAX_VALID_MEM_ADDR ); // Max
    SCV_CONSTRAINT ( mem_addr() >= MIN_VALID_MEM_ADDR ); // Min
  }
};

SC_MODULE(stim_gen) {
  SC_HAS_PROCESS(stim_gen);
  stim_gen(sc_module_name stim_gen) {
  }

  void init_seed_gen(unsigned long semilla){
    scv_random::set_global_seed(semilla); //FIXME: needs to come from test seed;
    cout << "TEST_SEED: " << semilla << endl;
  }
  sc_uint<8 > data_rnd_gen(){
    data_rnd_constraint data_rnd ("data_rnd_constraint");
    data_rnd.next();
    return data_rnd.data.read();
  }
  sc_uint<8 > addr_rnd_gen(){
    addr_rnd_constraint addr_rnd ("addr_rnd_constraint");
    addr_rnd.next();
    return addr_rnd.addr.read();
  }
  sc_uint<8 > mem_addr_rnd_gen(){
    mem_addr_rnd_constraint mem_addr_rnd ("mem_addr_rnd_constraint");
    mem_addr_rnd.next();
    return mem_addr_rnd.mem_addr.read();
  }
};

SC_MODULE (driver) {
  interface *intf_int;
  stim_gen *stim_gen_inst;
  scoreboard *scb_int;

  SC_HAS_PROCESS(driver);
  driver(sc_module_name driver,scoreboard *scb_ext, interface *intf_ext) {
    //Interface
    intf_int = intf_ext;
    //Scoreboard
   scb_int = scb_ext;
  }

  void reset();
  void write(sc_uint<8>, sc_uint<8>);
  void read(sc_uint<8>);
  void write_data(sc_uint<8>, sc_uint<8>, sc_uint<8>);
  sc_uint<8> read_data(sc_uint<8>, sc_uint<8>);
  void core_enable();
};

SC_MODULE (monitor) {
  interface *intf_int;
  scoreboard *scb_int;

  sc_uint<8> data_out_exp;
  sc_uint<8> data_out_read;
  sc_uint<8> num_available; // Number of data values in sc_fifo
  sc_uint<8> num_free;      // Number of free slots in sc_fifo

  SC_HAS_PROCESS(monitor);
  monitor(sc_module_name monitor,scoreboard *scb_ext, interface *intf_ext) {
    //Interface
    intf_int=intf_ext;
    //Scoreboard
   scb_int = scb_ext;
    // SC_THREAD(mnt_out);
    //   sensitive << intf_int->wb_we_i.pos();
  }

  void mnt_out();
};

SC_MODULE (environment) {
  driver *drv;
  monitor *mnt;
  scoreboard *scb;
  int clock_counter;

  SC_HAS_PROCESS(environment);
  environment(sc_module_name environment, interface *intf_ext) {
    //Scoreboard
    scb = new scoreboard("scb");
    //Driver
    drv = new driver("drv",scb,intf_ext);
    //Monitor
    mnt = new monitor("mnt",scb,intf_ext);
    //Clock Counter
    clock_counter = 0;
  }
};

SC_MODULE (suite_test) {
  interface *intf_int;
  environment *env;

  // B A S I C  T E S T
  // ------------------
  void basic_test();

  // C O R N E R  C A S E S
  // ----------------------
  void reset_test();

  // F R E Q U E N C Y
  // -----------------
  void normal_speed ();
  void fast_speed ();

  // M U L T I S L A V E
  // -------------------
  void two_slave ();

  // R A N D O M  V A L U E S
  // ------------------------
  void random_addr();
  void random_mem_addr();
  void random_data();
  void random_all();
  void burst();

  // SUITE INIT
  // **********
  void test ();
  void clock_counter ();

  SC_HAS_PROCESS(suite_test);
  suite_test(sc_module_name suite_test, interface *intf_ext) {
    intf_int = intf_ext;
    //environment
    env = new environment("env",intf_ext);
    SC_CTHREAD(clock_counter, intf_ext->wb_clk_i.pos());
    SC_CTHREAD(test, intf_ext->wb_clk_i.pos());
  }
};

SC_MODULE (sc_tb) {
  suite_test *test1;
  interface *intf;

  SC_CTOR(sc_tb) {
    intf = new interface("intf");
    test1 = new suite_test("test1",intf);
  }
};

#endif
