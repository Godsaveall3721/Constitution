#ifndef CONSTITUTION_CPP_CONSTITUTION_HPP
#define CONSTITUTION_CPP_CONSTITUTION_HPP

extern "C" {
#include "constitution.h"
}

namespace republic {

class Constitution {
public:
    Constitution();
    ~Constitution();

    constitution_t *native();
    const constitution_t *native() const;
    void printSummary() const;

private:
    constitution_t model_;
};

}

#endif
