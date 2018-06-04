#include "sc_tb.h"

  void driver::reset(){
    intf_int->wb_rst_i = true;
    cout<<"@"<<sc_time_stamp()<<" Started Reset " << endl;
    wait(10);
    intf_int->wb_rst_i = false;
    cout<<"@"<<sc_time_stamp()<<" Finished Reset " << endl;
  }
  
  void driver::write(sc_uint<8> d_out, sc_uint<8> addr){
    cout<<"@"<<sc_time_stamp()<<" Writing " << endl;
    // assert wishbone signal
    wait(1);
    intf_int->wb_adr_i = addr;
    intf_int->wb_dat_o = d_out;
    intf_int->wb_cyc_i= true;
    intf_int->wb_stb_i= true;
    intf_int->wb_we_i = true;
 

   // wait for acknowledge from slave
    while(intf_int->wb_ack_o){
    // negate wishbone signals
        wait(1);
  
     	intf_int->wb_cyc_i= false;
        intf_int->wb_stb_i= false;
     	intf_int->wb_adr_i  = rand();
	intf_int->wb_dat_o = rand();
	intf_int->wb_we_i  = false;
		

     }
      
     
  }
  
  void driver::read(sc_uint<8> addr){
    cout<<"@"<<sc_time_stamp()<<" Reading " << endl;
    
    // assert wishbone signal
     wait(1);
    intf_int->wb_adr_i = addr;
    intf_int->wb_dat_o = rand();
    intf_int->wb_cyc_i= true;
    intf_int->wb_stb_i= true;
    intf_int->wb_we_i = false;
    
    
    


    // wait for acknowledge from slave
     while(intf_int->wb_ack_o){

    // negate wishbone signals
    
        wait(1);
        
     	intf_int->wb_cyc_i= false;
        intf_int->wb_stb_i= false;
     	intf_int->wb_adr_i= rand();
	intf_int->wb_dat_o= rand();
	intf_int->wb_we_i = true;
        
       
        }

	
       
}


  void monitor::mnt_out(){
    while(true){
    wait(1);
    data_out_exp =  intf_int->wb_dat_o;
    data_out_read = intf_int->wb_dat_i;
    cout<<"@"<<sc_time_stamp()<<" Monitor data_out:" << data_out_exp << endl;
    cout<<"@"<<sc_time_stamp()<<" Scoreboard data_out:" << data_out_read << endl;
    //Checker 
    if (data_out_exp != data_out_read)
      //assert(data_out_exp == data_out_read);
      cout<<"@"<<sc_time_stamp()<<" ERROR: data read and expected mismatch!" << endl;  
    }
  }


  //Test
  void base_test::test() {
    intf_int->done = 0;
    env->drv->reset();
    wait(10);
    
    for (int i=0; i<10; i++){
      sc_uint<8> random1 = rand();
      sc_uint<8> random2 = rand();
      env->drv->write(random1, random2);
      wait(1);
       env->drv->read(random2);
    }
    wait(10);
    // Request for simulation termination
    cout << "=======================================" << endl;
    cout << " SIMULATION END" << endl;
    cout << "=======================================" << endl;
    wait(10);
    intf_int->done = 1;
    // Just wait for few cycles
  }
