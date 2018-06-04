#include <stdio.h>
#include <stdlib.h>
#include "vpi_user.h"

#include "sc_tb_ports.h"
#include "sc_tb_exports.h"

// CallBack Proto
int sc_tb_calltf(char *user_data);
int sc_tb_interface(p_cb_data cb_data);

int sc_tb_calltf(char *user_data) {
  s_vpi_value  value_s;
  s_vpi_time  time_s;
  s_cb_data  cb_data_s;
  vpiHandle wb_clk_i;

  time_s.type         = vpiSuppressTime;
  value_s.format      = vpiIntVal;

  // Set-Up Value Change callback option
  cb_data_s.user_data = NULL;
  cb_data_s.reason    = cbValueChange;
  cb_data_s.cb_rtn    = sc_tb_interface;
  cb_data_s.time      = &time_s;
  cb_data_s.value     = &value_s;

  //Callback signal
  wb_clk_i = vpi_handle_by_name("tb.wb_clk_i", NULL);
  cb_data_s.obj  = wb_clk_i;
  vpi_register_cb(&cb_data_s);

  init_sc();  // Initialize SystemC Model

  return(0);

}

// Value change simulation callback routine
int sc_tb_interface(p_cb_data cb_data)
{
  s_vpi_value  value_s;
  s_vpi_time time;

  // IO ports systemC testbench
  static INVECTOR   invector;
  static OUTVECTOR  outvector;

  // Handle VPI INPUTS
  // *****************
  vpiHandle wb_dat_o     = vpi_handle_by_name("tb.wb_dat_o",     NULL);

  // Handle VPI OUTPUTS
  // ******************
  vpiHandle wb_rst_i  = vpi_handle_by_name("tb.wb_rst_i",  NULL);
  vpiHandle arst_i    = vpi_handle_by_name("tb.arst_i",    NULL);
  vpiHandle wb_adr_i  = vpi_handle_by_name("tb.wb_adr_i",  NULL);
  vpiHandle wb_dat_i  = vpi_handle_by_name("tb.wb_dat_i",  NULL);
  vpiHandle scl_pad_i = vpi_handle_by_name("tb.scl_pad_i", NULL);
  vpiHandle sda_pad_i = vpi_handle_by_name("tb.sda_pad_i", NULL);

  // Read current value from Verilog
  value_s.format = vpiIntVal;

  // VPI INPUTS
  // **********
  vpi_get_value(wb_dat_o, &value_s);
  invector.wb_dat_o = value_s.value.integer;

  sc_time sc_time_tmp (1, SC_NS);
  exec_sc(&invector, &outvector, sc_time_tmp);

  // VPI OUTPUTS
  // ***********
  value_s.value.integer = outvector.wb_rst_i;
  vpi_put_value(wb_rst_i, &value_s, 0, vpiNoDelay);

  value_s.value.integer = outvector.arst_i;
  vpi_put_value(arst_i, &value_s, 0, vpiNoDelay);

  value_s.value.integer = outvector.wb_adr_i;
  vpi_put_value(wb_adr_i, &value_s, 0, vpiNoDelay);

  value_s.value.integer = outvector.wb_dat_i;
  vpi_put_value(wb_dat_i, &value_s, 0, vpiNoDelay);

  value_s.value.integer = outvector.scl_pad_i;
  vpi_put_value(scl_pad_i, &value_s, 0, vpiNoDelay);

  value_s.value.integer = outvector.sda_pad_i;
  vpi_put_value(sda_pad_i, &value_s, 0, vpiNoDelay);

  if (outvector.done) {

     exit_sc();

     vpi_control(vpiFinish);
  }
  return(0);
}

void sc_tb()
{
	s_vpi_systf_data tf;

	tf.type		=	vpiSysTask;
	tf.tfname	=	"$sc_tb";
	tf.calltf	=	sc_tb_calltf;
	tf.compiletf	=	NULL;
	tf.sizetf	=	0;
	tf.user_data	=	0;

	vpi_register_systf(&tf);
}

void (*vlog_startup_routines[])() = {
	sc_tb,
	0
};
