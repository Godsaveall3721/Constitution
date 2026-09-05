#ifndef CONSTITUTION_CPP_RESULT_HPP
#define CONSTITUTION_CPP_RESULT_HPP

extern "C" {
#include "result.h"
}

namespace republic {
namespace cpp {
using Result = constitution_result_t;
inline const constitution_result_t *ok() { return constitution_ok(); }
inline const constitution_result_t *invalidArgument() { return constitution_invalid_argument(); }
}
}

#endif
