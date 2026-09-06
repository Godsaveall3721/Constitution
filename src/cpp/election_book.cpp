#include "election_book.hpp"

#include <algorithm>
#include <iomanip>

namespace republic {
namespace cpp {

ElectionBook::ElectionBook(std::size_t totalSeats)
    : totalSeats_(totalSeats)
{
}

void ElectionBook::loadFromPartyShares(const std::vector<std::pair<std::string, double>> &shares)
{
    allocations_.clear();
    allocations_.reserve(shares.size());
    for (const auto &share : shares) {
        SeatAllocation allocation;
        allocation.name = share.first;
        allocation.voteShare = share.second;
        allocations_.push_back(std::move(allocation));
    }
}

void ElectionBook::balanceSeats()
{
    std::size_t allocated = 0;
    for (auto &allocation : allocations_) {
        allocation.seats = static_cast<std::size_t>(allocation.voteShare * static_cast<double>(totalSeats_) + 0.5);
        allocated += allocation.seats;
    }

    while (allocated < totalSeats_ && !allocations_.empty()) {
        ++allocations_.front().seats;
        ++allocated;
    }

    while (allocated > totalSeats_) {
        for (auto it = allocations_.rbegin(); it != allocations_.rend() && allocated > totalSeats_; ++it) {
            if (it->seats > 0) {
                --it->seats;
                --allocated;
            }
        }
    }
}

void ElectionBook::printSeatTable(std::ostream &stream) const
{
    for (const auto &allocation : allocations_) {
        stream << allocation.name << "：" << allocation.seats << " 席（"
               << static_cast<int>(allocation.voteShare * 1000.0) / 10.0 << "%）\n";
    }
}

const SeatAllocation *ElectionBook::largestBloc() const
{
    if (allocations_.empty()) {
        return nullptr;
    }
    return &*std::max_element(allocations_.begin(), allocations_.end(), [](const SeatAllocation &left, const SeatAllocation &right) {
        return left.seats < right.seats;
    });
}

const std::vector<SeatAllocation> &ElectionBook::allocations() const
{
    return allocations_;
}

} // namespace cpp
} // namespace republic
