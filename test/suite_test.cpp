#include "../sc_tb.h"

// This is a Basic Write/Read test
void suite_test::test() {
  intf_int->done = 0;
  //suite_test::basic_test();
  //suite_test::random_addr();
  //suite_test::random_mem_addr();
  //suite_test::random_data();
  //suite_test::random_all();
  suite_test::reset_test();
  intf_int->done = 1;
}
