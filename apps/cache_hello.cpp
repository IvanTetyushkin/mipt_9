#include <iostream>
#include <lru_cache.hpp>
#include <snlru_cache.hpp>

int main()
{
	using namespace handmade;
    using namespace std;
	int size;
	cin >> size;
	int num;
	// num
	cin >> num;
	snlru_cache<int, int> snlru(size, size, size);
	lru_cache<int, int> lru(size);
	int hitcount_snlru = 0;
	int hitcount_lru = 0;
	int current_addr;
	for (int i = 0; i < num; ++i)
	{
		cin >> current_addr;
		if(std::get<0>(snlru.getElement(current_addr)) == handmade::result::hit)
			++hitcount_snlru;
		if(std::get<0>(lru.getElement(current_addr)) == handmade::result::hit)
			++hitcount_lru;
	}
	cout << hitcount_snlru << "\n";
	cout << hitcount_lru << "\n";

	return 0;
}
