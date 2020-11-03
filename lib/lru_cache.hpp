#pragma once
#include "cache_interface.hpp"
#include <algorithm>
#include <cassert>
#include <iterator>
#include <list>
#include <unordered_map>
namespace handmade {
template <typename AddrType, typename StoreType>
class lru_cache : public cache_impl<AddrType, StoreType> {
public:
  using listIter = typename std::list<StoreType>::iterator;

private:
  size_t _sz;
  std::list<StoreType> _cache; // store raw "hottest" vals
  std::unordered_map<AddrType, listIter>
      _hash; // store address -> iterator to value

  void appendElement(const AddrType &addr, const StoreType &val) {
    assert(!isFull() && "try to add to full");
    assert((_hash.find(addr) == _hash.end()) && "element already in hash");
    _cache.push_front(val);
    _hash[addr] = _cache.begin();
  }
  void removeOldElement() {
    assert(isFull() && "try to remove not full");
    AddrType addr = std::get<0>(getDownElement());
    _hash.erase(addr);
    writeMem(addr, _cache.back()); // update
    _cache.pop_back();
  }

  static void writeMem(const AddrType &addr, const StoreType &val) {
    // for now, haha
  }
  void updatePosition(const listIter &findRes) {
    if (findRes != _cache.begin()) {
      _cache.splice(_cache.begin(), _cache, findRes);
    }
  }
  static StoreType getMemVal(const AddrType &addr) {
    constexpr int Magic = 10;
    // for now....
    return StoreType(static_cast<StoreType>(addr - Magic));
  }

public:
  std::tuple<AddrType, StoreType> getTopElement() const final {
    auto findResult =
        std::find_if(std::begin(_hash), std::end(_hash),
                     [&](const std::pair<AddrType, listIter> &pair) {
                       return pair.second == (_cache.begin());
                     });
    assert(findResult != std::end(_hash) && "error - in search top addr");
    return std::make_tuple((*findResult).first, (*(*findResult).second));
  }
  std::tuple<AddrType, StoreType> getDownElement() const final {
    auto findResult =
        std::find_if(std::begin(_hash), std::end(_hash),
                     [&](const std::pair<AddrType, listIter> &pair) {
                       return pair.second == (--_cache.end());
                     });
    assert(findResult != std::end(_hash) && "error - in search last addr");
    return std::make_tuple((*findResult).first, (*(*findResult).second));
  }
  [[nodiscard]] bool isFull() const final {
    assert(_cache.size() <= _sz && "Error, cache size too big");
    return (_cache.size() == _sz);
  }
  void dump(std::ostream &out) const final {
    out << "_cache:\n";
    std::for_each(std::begin(_cache), std::end(_cache),
                  [&out](const StoreType &val) { out << val << "\n"; });
    out << "_hash:\n";
    std::for_each(std::begin(_hash), std::end(_hash),
                  [&out](const std::pair<AddrType, listIter> &val) {
                    out << val.first << "|" << *(val.second) << "\n";
                  });
  }
  [[nodiscard]] bool check() const final { return true; }
  lru_cache(size_t sz) : _sz(sz){};
  std::tuple<result, StoreType> getElement(const AddrType &addr) final {
    auto searchRes = _hash.find(addr);
    if (searchRes == _hash.end()) { // no such address stored
      if (isFull()) {
        removeOldElement();
      }
      StoreType val = getMemVal(addr); // get value from next layer
      appendElement(addr, val);
      return std::make_tuple(result::miss, val);
    }
    // in cache, hohoho
    auto elmIt = searchRes->second;
    updatePosition(elmIt);
    return std::make_tuple(result::hit, *elmIt);
  }
  void writeElement(const AddrType &addr, const StoreType &val) final {
    auto searchRes = _hash.find(addr);
    if (searchRes == _hash.end()) { // no such address stored
      if (isFull()) {
        removeOldElement();
      }
      appendElement(addr, val);
      return;
    }
    // in cache, hohoho
    auto elmIt = searchRes->second;
    *elmIt = val;
    updatePosition(elmIt);
  }
  void deleteElement(const AddrType &toDel) final {
    auto pos = _hash.at(toDel);
    _hash.erase(toDel);
    writeMem(toDel, *pos); // update
    _cache.erase(pos);
  }
  std::optional<StoreType> checkElement(const AddrType &addr) const final {
    auto searchRes = _hash.find(addr);
    if (searchRes == _hash.end()) { // no such address stored
      return {};
    }
    // in cache, hohoho
    listIter elmIt = searchRes->second;
    return *elmIt;
  }
};
}; // namespace handmade
