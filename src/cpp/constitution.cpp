#include "constitution.hpp"

#include <cstdio>

extern "C" {
#include "constitution.h"
}

namespace republic {

Constitution::Constitution()
{
    constitution_init(&model_);
}

Constitution::~Constitution()
{
    constitution_reset(&model_);
}

constitution_t *Constitution::native()
{
    return &model_;
}

const constitution_t *Constitution::native() const
{
    return &model_;
}

void Constitution::printSummary() const
{
    constitution_print_summary(const_cast<constitution_t *>(&model_));
}

}
