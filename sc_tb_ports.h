#ifndef SC_TB_PORTS_H
#define SC_TB_PORTS_H

// Define Complex Type of Input and Out for DUT
struct tagInput {
  // Wishbone Signals
  // ****************
  unsigned long wb_clk_i;     // master clock input
  unsigned long wb_dat_o;     // databus output
  unsigned long wb_ack_o;     // bus cycle acknowledge output
  unsigned long wb_inta_o;    // interrupt request signal output

  // i2c Clock Line
  // **************
  unsigned long scl_pad_o;    // SCL-line output (always 1'b0)
  unsigned long scl_padoen_o; // SCL-line output enable (active low)

  // i2c Data Line
  // *************
  unsigned long sda_pad_o;    // SDA-line output (always 1'b0)
  unsigned long sda_padoen_o; // SDA-line output enable (active low)
};

struct tagOutput {
  // Wishbone Signals
  // ****************
  unsigned long wb_rst_i;   // synchronous active high reset
  unsigned long arst_i;     // asynchronous reset
  unsigned long wb_adr_i;   // lower address bits
  unsigned long wb_dat_i;   // databus input
  unsigned long wb_we_i;    // write enable input
  unsigned long wb_stb_i;   // stobe/core select signal
  unsigned long wb_cyc_i;   // valid bus cycle input

  // i2c Clock Line
  // **************
  unsigned long scl_pad_i;  // SCL-line input

  // i2c Data Line
  // *************
  // unsigned long sda_pad_i;  // SDA-line input

  int done;
};

typedef struct tagInput    INVECTOR;
typedef struct tagOutput  OUTVECTOR;

#endif
