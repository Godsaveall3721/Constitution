#include "config_book.hpp"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <sstream>

extern "C" {
#include "config.h"
#include "executive.h"
}

namespace republic {
namespace cpp {

namespace {

static std::string trim(std::string text)
{
    auto first = std::find_if_not(text.begin(), text.end(), [](unsigned char ch) {
        return std::isspace(ch) != 0;
    });
    auto last = std::find_if_not(text.rbegin(), text.rend(), [](unsigned char ch) {
        return std::isspace(ch) != 0;
    }).base();
    if (first >= last) {
        return {};
    }
    return std::string(first, last);
}

static std::vector<std::string> splitCsvLine(const std::string &line)
{
    std::vector<std::string> fields;
    std::stringstream stream(line);
    std::string field;
    while (std::getline(stream, field, ',')) {
        fields.push_back(trim(field));
    }
    return fields;
}

static bool isIgnoredLine(const std::string &line)
{
    for (char character : line) {
        if (!std::isspace(static_cast<unsigned char>(character))) {
            return character == '#';
        }
    }
    return true;
}

} // namespace

bool ConfigBook::load(const std::string &directory)
{
    parties_.clear();

    const std::string partiesPath = directory + "/parties.csv";
    std::ifstream partiesFile(partiesPath);
    if (!partiesFile.is_open()) {
        return false;
    }

    std::string line;
    while (std::getline(partiesFile, line)) {
        if (line.empty() || isIgnoredLine(line)) {
            continue;
        }

        const std::vector<std::string> fields = splitCsvLine(line);
        if (fields.size() < 3 || fields[0] == "name") {
            continue;
        }

        PartyRecord record;
        record.name = fields[0];
        record.ideology = fields[1];
        record.voteShare = std::stod(fields[2]);
        parties_.push_back(std::move(record));
    }

    return true;
}

const std::vector<PartyRecord> &ConfigBook::parties() const
{
    return parties_;
}

void ConfigBook::printPartyGallery(std::ostream &stream) const
{
    stream << "【政党展区】\n";
    for (const PartyRecord &party : parties_) {
        stream << "- " << party.name << "｜" << party.ideology << "｜票份额 "
               << party.voteShare << "%\n";
    }
}

} // namespace cpp
} // namespace republic
