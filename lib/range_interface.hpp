#pragma once
// t - container of int , like std::set<int>
// try concepts... , no cpp20...

#include <algorithm>
#include <iterator>
namespace handmade {
template <typename T> class interface_range {
  T store;

public:
  void insert(int v) { store.insert(v); }
  [[nodiscard]] int getDistance(int lhs, int rhs) const {
    auto l = store.lower_bound(lhs);
    auto r = store.upper_bound(rhs);
    return std::distance(l, r);
  }
};
}; // namespace handmade
