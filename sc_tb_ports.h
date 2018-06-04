#ifndef SC_TB_PORTS_H
#define SC_TB_PORTS_H

// Define Complex Type of Input and Out for DUT
struct tagInput {
  // Wishbone Signals
  // ****************
  unsigned long wb_dat_o;     // databus output
};

struct tagOutput {
  // Wishbone Signals
  // ****************
  unsigned long wb_rst_i;   // synchronous active high reset
  unsigned long arst_i;     // asynchronous reset
  unsigned long wb_adr_i;   // lower address bits
  unsigned long wb_dat_i;   // databus input

  // i2c Clock Line
  // **************
  unsigned long scl_pad_i;  // SCL-line input

  // i2c Data Line
  // *************
  unsigned long sda_pad_i;  // SDA-line input

  int done;
};

typedef struct tagInput    INVECTOR;
typedef struct tagOutput  OUTVECTOR;

#endif
