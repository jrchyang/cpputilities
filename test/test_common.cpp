#include "gtest/gtest.h"
#include "macros.hpp"
#include "abort.hpp"

namespace TOPNSPC {

TEST(Macros, ContainerOf) {
  struct temp {
    int a;
    double b;

    temp(int a, double b) : a(a), b(b) {}
  };

  struct temp *t = new temp(3, 1.4);
  struct temp *x = container_of(&(t->b), struct temp, b);
}

TEST(Abort, Abort) {
  EXPECT_DEATH({
      cpputilities_abort();
    }, ".*");
}

}
