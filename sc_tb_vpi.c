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
  sc_signal<bool>        wb_clk_i;     // master clock input
  sc_signal<sc_uint<8> > wb_dat_o;     // databus output
  sc_signal<bool>        wb_ack_o;     // bus cycle acknowledge output
  sc_signal<bool>        wb_inta_o;    // interrupt request signal output

  // i2c Clock Line
  // **************
  sc_signal<bool> scl_pad_o;    // SCL-line output (always 1'b0)
  sc_signal<bool> scl_padoen_o; // SCL-line output enable (active low)

  // i2c Data Line
  // *************
  sc_signal<bool> sda_pad_o;    // SDA-line output (always 1'b0)
  sc_signal<bool> sda_padoen_o; // SDA-line output enable (active low)

  /***************
   * Output Signals
   ****************/
  // Wishbone Signals
  // ****************
  sc_signal<bool>        wb_rst_i;   // synchronous active high reset
  sc_signal<bool>        arst_i;     // asynchronous reset
  sc_signal<sc_uint<8> > wb_adr_i;   // lower address bits
  sc_signal<sc_uint<8> > wb_dat_i;   // databus input
  sc_signal<bool>        wb_we_i;    // write enable input
  sc_signal<bool>        wb_stb_i;   // stobe/core select signal
  sc_signal<bool>        wb_cyc_i;   // valid bus cycle input

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
  u_sc_tb.intf->wb_clk_i(wb_clk_i);
  u_sc_tb.intf->wb_dat_o(wb_dat_o);
  u_sc_tb.intf->wb_ack_o(wb_ack_o);
  u_sc_tb.intf->wb_inta_o(wb_inta_o);
  u_sc_tb.intf->scl_pad_o(scl_pad_o);
  u_sc_tb.intf->scl_padoen_o(scl_padoen_o);
  u_sc_tb.intf->sda_pad_o(sda_pad_o);
  u_sc_tb.intf->sda_padoen_o(sda_padoen_o);

  // Output Signals
  // **************
  u_sc_tb.intf->wb_rst_i(wb_rst_i);
  u_sc_tb.intf->arst_i(arst_i);
  u_sc_tb.intf->wb_adr_i(wb_adr_i);
  u_sc_tb.intf->wb_dat_i(wb_dat_i);
  u_sc_tb.intf->wb_we_i(wb_we_i);
  u_sc_tb.intf->wb_stb_i(wb_stb_i);
  u_sc_tb.intf->wb_cyc_i(wb_cyc_i);
  u_sc_tb.intf->scl_pad_i(scl_pad_i);
  u_sc_tb.intf->sda_pad_i(sda_pad_i);

  u_sc_tb.intf->done(done);

  // Initialize
  sc_start();
  cout<<"@"<<sc_time_stamp()<<" Started SystemC Scheduler"<<endl;
}

void sample_hdl(void *Invector) {
  INVECTOR *pInvector = (INVECTOR *)Invector;
  wb_clk_i.write(pInvector->wb_clk_i);
  wb_dat_o.write(pInvector->wb_dat_o);
  wb_ack_o.write(pInvector->wb_ack_o);
  wb_inta_o.write(pInvector->wb_inta_o);
  scl_pad_o.write(pInvector->scl_pad_o);
  scl_padoen_o.write(pInvector->scl_padoen_o);
  sda_pad_o.write(pInvector->sda_pad_o);
  sda_padoen_o.write(pInvector->sda_padoen_o);
}

void drive_hdl(void *Outvector) {
  OUTVECTOR *pOutvector = (OUTVECTOR *)Outvector;
  pOutvector->wb_rst_i  = wb_rst_i.read();
  pOutvector->arst_i    = arst_i.read();
  pOutvector->wb_adr_i  = wb_adr_i.read();
  pOutvector->wb_dat_i  = wb_dat_i.read();
  pOutvector->wb_we_i   = wb_we_i.read();
  pOutvector->wb_stb_i  = wb_stb_i.read();
  pOutvector->wb_cyc_i  = wb_cyc_i.read();
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
