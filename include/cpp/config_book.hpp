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

struct ElectoralDistrictRecord {
    std::string name;
    std::string kind;
    std::string label;
    std::string seatName;
    unsigned long long population = 0;
    std::string parent;
};

class ConfigBook {
public:
    ConfigBook() = default;

    bool load(const std::string &directory);

    const std::vector<PartyRecord> &parties() const;
    const std::vector<ElectoralDistrictRecord> &electoralDistricts() const;
    void printPartyGallery(std::ostream &stream) const;
    void printElectoralDistrictGallery(std::ostream &stream) const;
    void addElectoralDistrict(ElectoralDistrictRecord record);

private:
    std::vector<PartyRecord> parties_;
    std::vector<ElectoralDistrictRecord> electoralDistricts_;
};

} // namespace cpp
} // namespace republic

#endif
