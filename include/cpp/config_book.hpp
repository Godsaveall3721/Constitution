#ifndef CONSTITUTION_CPP_CONFIG_BOOK_HPP
#define CONSTITUTION_CPP_CONFIG_BOOK_HPP

#include <cstddef>
#include <iosfwd>
#include <string>
#include <vector>

extern "C" {
#include "constitution.h"
}

namespace republic {
namespace cpp {

struct PartyRecord {
    std::string name;
    std::string ideology;
    double voteShare = 0.0;
};

class ConfigBook {
public:
    ConfigBook() = default;

    bool load(const std::string &directory);

    const std::vector<PartyRecord> &parties() const;
    void printPartyGallery(std::ostream &stream) const;

private:
    std::vector<PartyRecord> parties_;
};

} // namespace cpp
} // namespace republic

#endif
