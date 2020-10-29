// Alternatively, can add libraries using linker options.
#ifdef _WIN32
#pragma comment ( lib, "Shlwapi.lib" )
#endif


#include <benchmark/benchmark.h>
#include <set>
#include <range_interface.hpp>
#include <range_mine.hpp>
#include <array>
#include <random>
#include <vector>
constexpr int size = 10000;
constexpr int min = 10;
constexpr int max = 90;
constexpr int times = 100;
constexpr int step_min = 2;
constexpr int step_max = 3;
using namespace handmade;
std::array<int, size> generate_same_data()
{
	// as default seed the same ...
	std::default_random_engine dre;
	std::uniform_int_distribution<> d{ 1200,1200};
	std::array<int, size> ret;
	for (int i = 0; i < size; ++i)
		ret[i] = d(dre);
	return ret;
}

static void set_insert(benchmark::State& state) {
	// Perform setup here
	interface_range<std::set<int>> V;
	std::array<int, size> vals = generate_same_data();
	for (auto _ : state) {
		// This code gets timed
		for (auto& v : vals)
		{
			V.insert(v);
		}
	}
	V.getDistance(10, 112);
}
static void handmade_insert(benchmark::State& state) {
	// Perform setup here
	interface_range<half_avl_tree<int>> V;
	std::array<int, size> vals = generate_same_data();
	for (auto _ : state) {
		// This code gets timed
		for (auto& v : vals)
		{
			V.insert(v);
		}
	}
	V.getDistance(10, 112);
}


static void set_speed(benchmark::State& state) {
	// Perform setup here
	interface_range<std::set<int>> V;
	std::array<int, size> vals = generate_same_data();
	for (auto _ : state) {
		// This code gets timed
		for (auto& v : vals)
		{
			V.insert(v);
		}
		volatile int res;
		for(int i = 0; i < times; ++i)
			benchmark::DoNotOptimize(res = V.getDistance(min + i * step_min, max + i * step_max ));
	}
}
static void handmade_speed(benchmark::State& state) {
	// Perform setup here
	interface_range<half_avl_tree<int>> V;
	std::array<int, size> vals = generate_same_data();
	for (auto _ : state) {
		// This code gets timed
		for (auto& v : vals)
		{
			V.insert(v);
		}
		volatile int res;
		for(int i = 0; i < times; ++i)
			benchmark::DoNotOptimize(res = V.getDistance(min + i * step_min, max + i * step_max ));
	}
}
// Register the function as a benchmark
BENCHMARK(set_insert);
BENCHMARK(handmade_insert);
BENCHMARK(set_speed);
BENCHMARK(handmade_speed);
// Run the benchmark
BENCHMARK_MAIN();