#pragma once
#include <tuple>
#include <array>
#include "cache_interface.hpp"
#include "lru_cache.hpp"
#include <algorithm>

namespace handmade
{
template<typename AddrType, typename StoreType>
class snlru_cache : public cache_impl<AddrType, StoreType>
{
private:
	// 0 - hot
	// 1 - warm
	// 2 - cold
	std::array<lru_cache<AddrType, StoreType>, 3> _cache;
	void swapCacheElements(size_t to, size_t from, const AddrType& addr)
	{
		assert(_cache.at(to).isFull() && "error try to swap not full");
		assert(_cache.at(from).checkElement(addr).has_value() && "error try to swap nonexist");
		AddrType to_down_addr;
		StoreType to_down_val;
		std::tie(to_down_addr, to_down_val)	= _cache.at(to).getDownElement();
		_cache[to].writeElement(addr, _cache[from].checkElement(addr).value());
		_cache[from].deleteElement(addr);
		_cache[from].writeElement(to_down_addr, to_down_val);
	}
	void moveCacheElement(size_t to, size_t from, const AddrType& addr)
	{
		assert(!_cache.at(to).isFull() && "error try to move to full");
		assert(_cache.at(from).checkElement(addr).has_value() && "error try to move nonexist");
		_cache[to].writeElement(addr, _cache[from].checkElement(addr).value());
		_cache[from].deleteElement(addr);
	}
	void higherElement(size_t from, const AddrType& addr)
	{
		if (from == 0)
			return; // already top
		if (_cache.at(from - 1).isFull())
		// next is full
			swapCacheElements(from - 1, from, addr);
		else
			moveCacheElement(from - 1, from, addr);
		return;
	}

public:
	snlru_cache(size_t sz) :
		_cache{ sz, sz,  sz } {};
	snlru_cache(size_t hot_sz, size_t warm_sz, size_t cold_sz) :
		_cache{ hot_sz, warm_sz, cold_sz } {};
	bool isFull() const override final
	{
		// need ranges but do not have them...
		return std::all_of(_cache.begin(), _cache.end(), [](const lru_cache<AddrType, StoreType>& c)
		{
			return c.isFull();
		});
	}
	void dump(std::ostream& out = std::cout) const override final
	{
		out << "hot:\n";
		_cache[0].dump();
		out << "warm\n";
		_cache[1].dump();
		out << "cold\n";
		_cache[2].dump();
	}
	bool check() const override final
	{
		return true;
	}
	std::tuple<AddrType, StoreType> getTopElement() const override final
	{
		return _cache[0].getTopElement();
	}
	std::tuple<AddrType, StoreType> getDownElement() const override final
	{
		return _cache[2].getDownElement();
	}

	std::optional<StoreType> checkElement(const AddrType& addr) const override final
	{
		for (int i = 0; i < 3; ++i)
		{
			if (_cache[i].checkElement(addr).has_value())
				return _cache[i].checkElement(addr);
		}
		return {};
	}

	std::tuple<result, StoreType> getElement(const AddrType& addr) override final
	{
		if (_cache[0].checkElement(addr).has_value())
		{// it is in hot cache
			return _cache[0].getElement(addr);
		}
		for (int i = 1; i < 3; ++i)
		{
			if (_cache[i].checkElement(addr).has_value())
			{// it is in cache
				higherElement(i, addr);
				return _cache[--i].getElement(addr);
			}
		}
		// it is not in cache...
		return _cache[2].getElement(addr);
	}
	void writeElement(const AddrType& addr, const StoreType& val) override final
	{
		if (_cache[0].checkElement(addr).has_value())
		{// it is in hot cache
			return _cache[0].writeElement(addr, val);
		}
		for (int i = 1; i < 3; ++i)
		{
			if (_cache[i].checkElement(addr).has_value())
			{// it is in cache
				higherElement(i, addr);
				return _cache[--i].writeElement(addr, val);
			}
		}
		// it is not in cache...
		return _cache[2].writeElement(addr, val);
	}
	void deleteElement(const AddrType& addr) override final
	{
		for (int i = 0; i < 3; ++i)
		{
			if (_cache[i].checkElement(addr).has_value())
			{// it is in cache
				return _cache[i].deleteElement(addr);
			}
		}
		return;
	}
};

}

