#include <iostream>
#include <lru_cache.hpp>
#include <snlru_cache.hpp>

int main()
{
	using namespace handmade;
    using namespace std;
    cout << "Hi there\n";
	//cache_impl<int, int> b;
	lru_cache<int, int> c(10);
	return 0;
}
