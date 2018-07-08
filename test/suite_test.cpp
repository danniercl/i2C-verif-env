#include "../sc_tb.h"

// This is a Basic Write/Read test
void suite_test::test() {
  intf_int->done = 0;
  suite_test::basic_test();
  intf_int->done = 1;
}
