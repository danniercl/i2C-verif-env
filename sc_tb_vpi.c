#include "sc_tb_ports.h"
#include "sc_tb_exports.h"
#include "strings.h"

// Instantiate Top-Level DUT
sc_tb    u_sc_tb("u_sc_tb");

  /***************
   * Input Signals
   ***************/

  // Wishbone Signals
  // ****************
  sc_signal<sc_uint<8> > wb_dat_o;     // databus output

  /***************
   * Output Signals
   ****************/
  // Wishbone Signals
  // ****************
  sc_signal<bool>        wb_rst_i;   // synchronous active high reset
  sc_signal<bool>        arst_i;     // asynchronous reset
  sc_signal<sc_uint<8> > wb_adr_i;   // lower address bits
  sc_signal<sc_uint<8> > wb_dat_i;   // databus input

  // i2c Clock Line
  // **************
  sc_signal<bool> scl_pad_i;  // SCL-line input
  // i2c Data Line
  // *************
  sc_signal<bool> sda_pad_i;  // SDA-line input

  sc_signal<bool>  done; //Terminate sim

// Top-Level testbench
void init_sc() {

  // Input signals
  // *************
  u_sc_tb.intf->wb_dat_o(wb_dat_o);

  // Output Signals
  // **************
  u_sc_tb.intf->wb_rst_i(wb_rst_i);
  u_sc_tb.intf->arst_i(arst_i);
  u_sc_tb.intf->wb_adr_i(wb_adr_i);
  u_sc_tb.intf->wb_dat_i(wb_dat_i);
  u_sc_tb.intf->scl_pad_i(scl_pad_i);
  u_sc_tb.intf->sda_pad_i(sda_pad_i);

  u_sc_tb.intf->done(done);

  // Initialize
  sc_start();
  cout<<"@"<<sc_time_stamp()<<" Started SystemC Scheduler"<<endl;
}

void sample_hdl(void *Invector) {
  INVECTOR *pInvector = (INVECTOR *)Invector;
  wb_dat_o.write(pInvector->wb_dat_o);
}

void drive_hdl(void *Outvector) {
  OUTVECTOR *pOutvector = (OUTVECTOR *)Outvector;
  pOutvector->wb_rst_i  = wb_rst_i.read();
  pOutvector->arst_i    = arst_i.read();
  pOutvector->wb_adr_i  = wb_adr_i.read();
  pOutvector->wb_dat_i  = wb_dat_i.read();
  pOutvector->scl_pad_i = scl_pad_i.read();
  pOutvector->sda_pad_i = sda_pad_i.read();

  pOutvector->done    = done.read();
}

void advance_sim(sc_time simtime) {
  sc_start(simtime);
}

//void exec_sc(void *invector, void *outvector, unsigned long simtime) {
void exec_sc(void *invector, void *outvector, sc_time simtime) {
  sample_hdl(invector);    // Input-Stimuli
  advance_sim(simtime);   // Advance Simulator
  drive_hdl(outvector);  // Output Vectors
}

void exit_sc() {
  cout<<"@"<<sc_time_stamp()<<" Stopping SystemC Scheduler"<<endl;
  sc_stop();
}
