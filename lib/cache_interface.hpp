#pragma once
#include <optional>
#include <tuple>
namespace handmade {
enum class result {
  miss,
  hit,
};

template <typename AddrType, typename StoreType> class cache_impl {
public:
  cache_impl() = default;
  [[nodiscard]] virtual bool isFull() const = 0;
  [[nodiscard]] virtual std::tuple<AddrType, StoreType>
  getTopElement() const = 0;
  [[nodiscard]] virtual std::tuple<AddrType, StoreType>
  getDownElement() const = 0;
  [[nodiscard]] virtual std::optional<StoreType>
  checkElement(const AddrType &addr) const = 0;
  virtual std::tuple<result, StoreType> getElement(const AddrType &addr) = 0;
  virtual void writeElement(const AddrType &addr, const StoreType &val) = 0;
  virtual void deleteElement(const AddrType &toDel) = 0;
  virtual void dump(std::ostream &out) const = 0;
  [[nodiscard]] virtual bool check() const = 0;
  virtual ~cache_impl() = default;
};
} // namespace handmade
