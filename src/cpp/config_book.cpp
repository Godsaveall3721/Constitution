#include "config_book.hpp"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <filesystem>
#include <sstream>

extern "C" {
#include "config.h"
#include "executive.h"
}

namespace republic {
namespace cpp {

namespace fs = std::filesystem;

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

static bool loadElectoralDistrictCsv(ConfigBook &book, const std::string &path)
{
    std::ifstream districtsFile(path);
    if (!districtsFile.is_open()) {
        return false;
    }

    std::string line;
    while (std::getline(districtsFile, line)) {
        if (line.empty() || isIgnoredLine(line)) {
            continue;
        }

        const std::vector<std::string> fields = splitCsvLine(line);
        if (fields.size() < 6 || fields[0] == "name") {
            continue;
        }

        ElectoralDistrictRecord record;
        record.name = fields[0];
        record.kind = fields[1];
        record.label = fields[2];
        record.seatName = fields[3];
        record.population = static_cast<unsigned long long>(std::stoull(fields[4]));
        record.parent = fields[5];
        book.addElectoralDistrict(std::move(record));
    }

    return true;
}

} // namespace

bool ConfigBook::load(const std::string &directory)
{
    parties_.clear();
    electoralDistricts_.clear();

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

    const fs::path districtsDirectory = fs::path(directory) / "electoral_districts";
    if (fs::exists(districtsDirectory) && fs::is_directory(districtsDirectory)) {
        std::vector<fs::path> districtFiles;
        for (const auto &entry : fs::directory_iterator(districtsDirectory)) {
            if (entry.is_regular_file() && entry.path().extension() == ".csv") {
                districtFiles.push_back(entry.path());
            }
        }
        std::sort(districtFiles.begin(), districtFiles.end());
        for (const auto &filePath : districtFiles) {
            if (!loadElectoralDistrictCsv(*this, filePath.string())) {
                return false;
            }
        }
    } else {
        const std::string districtsPath = directory + "/electoral_districts.csv";
        std::ifstream districtsFile(districtsPath);
        if (districtsFile.is_open()) {
            while (std::getline(districtsFile, line)) {
                if (line.empty() || isIgnoredLine(line)) {
                    continue;
                }

                const std::vector<std::string> fields = splitCsvLine(line);
                if (fields.size() < 6 || fields[0] == "name") {
                    continue;
                }

                ElectoralDistrictRecord record;
                record.name = fields[0];
                record.kind = fields[1];
                record.label = fields[2];
                record.seatName = fields[3];
                record.population = static_cast<unsigned long long>(std::stoull(fields[4]));
                record.parent = fields[5];
                electoralDistricts_.push_back(std::move(record));
            }
        }
    }

    return true;
}

const std::vector<PartyRecord> &ConfigBook::parties() const
{
    return parties_;
}

const std::vector<ElectoralDistrictRecord> &ConfigBook::electoralDistricts() const
{
    return electoralDistricts_;
}

void ConfigBook::addElectoralDistrict(ElectoralDistrictRecord record)
{
    electoralDistricts_.push_back(std::move(record));
}

void ConfigBook::printPartyGallery(std::ostream &stream) const
{
    stream << "【政党展区】\n";
    for (const PartyRecord &party : parties_) {
        stream << "- " << party.name << "｜" << party.ideology << "｜票份额 "
               << party.voteShare << "%\n";
    }
}

void ConfigBook::printElectoralDistrictGallery(std::ostream &stream) const
{
    stream << "【选区展区】\n";
    for (const ElectoralDistrictRecord &district : electoralDistricts_) {
        stream << "- " << district.name << "｜" << district.kind;
        if (!district.label.empty() && district.label != "-") {
            stream << "｜" << district.label;
        }
        if (!district.seatName.empty() && district.seatName != "-") {
            stream << "｜驻地 " << district.seatName;
        }
        stream << "｜人口 " << district.population;
        if (!district.parent.empty() && district.parent != "-") {
            stream << "｜上级 " << district.parent;
        }
        stream << "\n";
    }
}

} // namespace cpp
} // namespace republic
