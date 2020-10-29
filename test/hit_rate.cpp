#include <random>
#include <iostream>
#include <lru_cache.hpp>
#include <snlru_cache.hpp>
#include <optional>


int main()
{
	using namespace handmade;
	std::random_device rd;
	std::mt19937 gen(rd());
	std::normal_distribution<> d{ 120,10};
	lru_cache<int, int> lru{ 12 };
	snlru_cache<int, int> snlru{ 4 , 4, 4 };
	int hit_snlru = 0;
	int hit_lru = 0;
	int times = 100000;
	for (int i = 0;i < times; ++i)
	{
		int addr = d(gen);
		if (std::get<0>(lru.getElement(addr)) == result::hit)
			++hit_lru;
		if (std::get<0>(snlru.getElement(addr)) == result::hit)
			++hit_snlru;
	}
	std::cout << static_cast<double>(hit_lru - hit_snlru) / times << "\n";
	std::cout <<"lru: "<< static_cast<double>(hit_lru) / times << "\n";
	std::cout <<"slru: "<<  static_cast<double>(hit_snlru) / times << "\n";

}
