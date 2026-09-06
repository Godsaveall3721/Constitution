#ifndef CONSTITUTION_CPP_ELECTION_BOOK_HPP
#define CONSTITUTION_CPP_ELECTION_BOOK_HPP

#include <cstddef>
#include <ostream>
#include <string>
#include <vector>

namespace republic {
namespace cpp {

struct SeatAllocation {
    std::string name;
    double voteShare = 0.0;
    std::size_t seats = 0;
};

class ElectionBook {
public:
    explicit ElectionBook(std::size_t totalSeats);

    void loadFromPartyShares(const std::vector<std::pair<std::string, double>> &shares);
    void balanceSeats();
    void printSeatTable(std::ostream &stream) const;
    const SeatAllocation *largestBloc() const;

private:
    std::size_t totalSeats_;
    std::vector<SeatAllocation> allocations_;
};

} // namespace cpp
} // namespace republic

#endif
