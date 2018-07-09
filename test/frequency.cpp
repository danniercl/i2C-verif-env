#include "../sc_tb.h"

#define PROCESS_CYCLES 28

// Normal Speed: 100kbps
#define NORMAL_SCALE 5000 // i2c_cycles / in_clock_cycles

// Fast Speed: 400kbps
#define FAST_SCALE 1250 // i2c_cycles / in_clock_cycles

// This test checks the DUT working with a Normal Speed:
void suite_test::normal_speed() {
  cout << "*******************" << endl;
  cout << " TEST: NORMAL SPEED" << endl;
  cout << "*******************" << endl;
  // Declate the clock counter
  int clock_counter = 0;
  int nominal_cycles = PROCESS_CYCLES * NORMAL_SCALE;

  // Generate address (ID) of the slave
  sc_uint<8> addr = 0x2;
  env->drv->reset();
  // Generate random data to send
  sc_uint<8> data = env->drv->stim_gen_inst->data_rnd_gen();
  sc_uint<8> received = 0x00;
  sc_uint<8> mem_addr = env->drv->stim_gen_inst->mem_addr_rnd_gen();

  // Set the prescale register to get a transmission of 100 kHz
  // Our clock has a frequency of 500 MHz

  // Prescale Register = [ 500M / (5 * 100k) ] - 1 = 999 = 0x03E7
  env->drv->write(0xE7, PRER_LO); // load prescaler lo-byte
  env->drv->write(0x03, PRER_HI); // load prescaler hi-byte
  env->drv->write(0x80, CTR);     // enable core

  // W R I T E
  // *********
  clock_counter = env->clock_counter;
  env->drv->write_data(addr, mem_addr, data);
  /* Store value in expected fifo */
  env->drv->scb_int->expected_fifo.nb_write(data);
  // Get the amount of cycles taken by the write
  clock_counter = env->clock_counter - clock_counter;

  // The value should be inside a +-10% of the nominal value
  if ((0.9 * nominal_cycles) < clock_counter &&
      clock_counter < (1.1 * nominal_cycles)){
    wait(100);
  }
  else {
    cout << "========================================" << endl;
    cout << " ERROR: Normal Frequency Failed in WRITE" << endl;
    cout << "========================================" << endl;
    return;
  }

  // R E A D
  // *******
  clock_counter = env->clock_counter;
  received = env->drv->read_data(addr, mem_addr);
  /* Store received value in received fifo*/
   env->drv->scb_int->received_fifo.nb_write(received);
  // Get the amount of cycles taken by the read
  clock_counter = env->clock_counter - clock_counter;

  // The value should be inside a +-10% of the nominal value
  if ((0.9 * nominal_cycles) < clock_counter &&
      clock_counter < (1.1 * nominal_cycles)){
    wait(10);
  }
  else {
    cout << "========================================" << endl;
    cout << " ERROR: Normal Frequency Failed in READ " << endl;
    cout << "========================================" << endl;
    intf_int->done = 1;
  }

  // M O N I T O R
  // *************
  env->mnt->mnt_out();
  wait(10);

  // TODO: SCOREBOARD - CHECKER
}

// This test checks the DUT working with a Fast Speed:
void suite_test::fast_speed() {
  cout << "*****************" << endl;
  cout << " TEST: FAST SPEED" << endl;
  cout << "*****************" << endl;
  // Declate the clock counter
  int clock_counter = 0;
  int nominal_cycles = PROCESS_CYCLES * FAST_SCALE;

  // Generate address (ID) of the slave
  sc_uint<8> addr = 0x2;
  env->drv->reset();
  // Generate random data to send
  sc_uint<8> data = env->drv->stim_gen_inst->data_rnd_gen();
  sc_uint<8> received = 0x00;
  sc_uint<8> mem_addr = env->drv->stim_gen_inst->mem_addr_rnd_gen();

  // Set the prescale register to get a transmission of 400 kHz
  // Our clock has a frequency of 500 MHz

  // Prescale Register = [ 500M / (5 * 400k) ] - 1 = 249 = 0x00F9
  env->drv->write(0xF9, PRER_LO); // load prescaler lo-byte
  env->drv->write(0x00, PRER_HI); // load prescaler hi-byte
  env->drv->write(0x80, CTR);     // enable core

  // W R I T E
  // *********
  clock_counter = env->clock_counter;
  /* Store value in expected fifo */
  env->drv->scb_int->expected_fifo.nb_write(data);
  env->drv->write_data(addr, mem_addr, data);
  // Get the amount of cycles taken by the write
  clock_counter = env->clock_counter - clock_counter;

  // The value should be inside a +-10% of the nominal value
  if ((0.9 * nominal_cycles) < clock_counter &&
      clock_counter < (1.1 * nominal_cycles)){
    wait(100);
  }
  else {
    cout << "========================================" << endl;
    cout << " ERROR: Fast Frequency Failed in WRITE" << endl;
    cout << "========================================" << endl;
    return;
  }

  // R E A D
  // *******
  clock_counter = env->clock_counter;
  received = env->drv->read_data(addr, mem_addr);
  /* Store received value in received fifo*/
  env->drv->scb_int->received_fifo.nb_write(received);
  // Get the amount of cycles taken by the read
  clock_counter = env->clock_counter - clock_counter;

  // The value should be inside a +-10% of the nominal value
  if ((0.9 * nominal_cycles) < clock_counter &&
      clock_counter < (1.1 * nominal_cycles)){
    wait(10);
  }
  else {
    cout << "========================================" << endl;
    cout << " ERROR: Fast Frequency Failed in READ " << endl;
    cout << "========================================" << endl;
    intf_int->done = 1;
  }

  // M O N I T O R
  // *************
  env->mnt->mnt_out();
  wait(10);

  // TODO: SCOREBOARD - CHECKER
}
