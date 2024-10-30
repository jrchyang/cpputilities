#pragma once

#include "namespace.h"

namespace TOPNSPC {
  extern void __abort(const char *file, const int line, const char *func);
}

#define cpputilities_abort()	\
  TOPNSPC::__abort(__FILE__, __LINE__, __func__)
