`timescale 1ns / 1ns
`include "./i2c/rtl/i2c_master_top.v"

// Testbench top level
module tb();

  /******************
	 * Inputs & Outputs
	 ******************/

	// Wishbone Signals
  // ****************
	reg        wb_clk_i;   // master clock input
	reg        wb_rst_i;   // synchronous active high reset
	reg        arst_i;     // asynchronous reset
	reg  [2:0] wb_adr_i;   // lower address bits
	reg  [7:0] wb_dat_i;   // databus input
	wire [7:0] wb_dat_o;   // databus output
	reg        wb_we_i;    // write enable input
	reg        wb_stb_i;   // stobe/core select signal
	reg        wb_cyc_i;   // valid bus cycle input
	wire       wb_ack_o;   // bus cycle acknowledge output
	wire       wb_inta_o;  // interrupt request signal output

	/*************
   * I2C signals
   *************/

	// i2c Clock Line
  // **************
	reg  scl_pad_i;       // SCL-line input
	wire scl_pad_o;       // SCL-line output (always 1'b0)
	wire scl_padoen_o;    // SCL-line output enable (active low)

	// i2c Data Line
  // *************
	reg  sda_pad_i;       // SDA-line input
	wire sda_pad_o;       // SDA-line output (always 1'b0)
	wire sda_padoen_o;    // SDA-line output enable (active low)

  // Clock Generator
  // ***************
  always #1 wb_clk_i = ~wb_clk_i;

  /**************
   * DUT Instance
   **************/

  i2c_master_top i2c_master(
  // Wishbone
    .wb_clk_i(wb_clk_i),
    .wb_rst_i(wb_rst_i),
    .arst_i(arst_i),
    .wb_adr_i(wb_adr_i),
    .wb_dat_i(wb_dat_i),
    .wb_dat_o(wb_dat_o),
    .wb_we_i(wb_we_i),
    .wb_stb_i(wb_stb_i),
    .wb_cyc_i(wb_cyc_i),
    .wb_ack_o(wb_ack_o),
    .wb_inta_o(wb_inta_o),

    // I2C
   	.scl_pad_i(scl_pad_i),
    .scl_pad_o(scl_pad_o),
    .scl_padoen_o(scl_padoen_o),
    .sda_pad_i(sda_pad_i),
    .sda_pad_o(sda_pad_o),
    .sda_padoen_o(sda_padoen_o)
  );

  initial begin
    $dumpfile("i2c.vcd");
    $dumpvars(0);
    $display("[DUT]: Starting I2C Master in SystemC...");
    $sc_tb; // Testbench Connection
    wb_clk_i = 1'b0;
  end

endmodule
