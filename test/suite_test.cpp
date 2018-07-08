#include "../sc_tb.h"

// This is a Basic Write/Read test
void suite_test::test() {
  intf_int->done = 0;

  // B A S I C  T E S T
  // ------------------
  suite_test::basic_test();

  // C O R N E R  C A S E S
  // ----------------------
  suite_test::reset_test();

  // F R E Q U E N C Y
  // -----------------
  suite_test::normal_speed();
  suite_test::fast_speed();

  // R A N D O M  V A L U E S
  // ------------------------
 suite_test::random_addr();
 suite_test::random_mem_addr();
 suite_test::random_data();
 suite_test::random_all();

  intf_int->done = 1;
}

void suite_test::clock_counter(){
  while (true) {
    wait(1);
    env->clock_counter++;
  }
}
