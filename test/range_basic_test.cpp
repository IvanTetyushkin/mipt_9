#include <gtest/gtest.h>
#include <range_interface.hpp>
#include <range_mine.hpp>
#include <random>
#include<set>
using namespace handmade;
TEST(stdset, basic)
{
	interface_range<std::set<int>> V;
	int min = 10;
	int count = 100;
	for (int i = 0; i < count; ++i)
	{
		V.insert(i);
	}
	ASSERT_EQ(V.getDistance(1, 8), 8);
	ASSERT_EQ(V.getDistance(-1, 8), 9);
	ASSERT_EQ(V.getDistance(-1, 9), 10);
	ASSERT_EQ(V.getDistance(-1, 99), 100);
	for (int i = 0; i < count; ++i)
	{
		V.insert(i);
	}
	// it is set!!!
	ASSERT_EQ(V.getDistance(1, 8), 8);
	ASSERT_EQ(V.getDistance(-1, 8), 9);
	ASSERT_EQ(V.getDistance(-1, 9), 10);
	ASSERT_EQ(V.getDistance(-1, 99), 100);
	interface_range<std::set<int>> V2;
	for (int i = 0; i < count; ++i)
	{
		V2.insert(i * 10);
	}
	ASSERT_EQ(V2.getDistance(1, 8), 0);
	ASSERT_EQ(V2.getDistance(-1, 8), 1);
	ASSERT_EQ(V2.getDistance(-1, 9), 1);
	ASSERT_EQ(V2.getDistance(-1, 99), 10);
}
TEST(my_range, basic)
{
	interface_range<half_avl_tree<int>> V;
	int min = 10;
	int count = 100;
	for (int i = 0; i < count; ++i)
	{
		V.insert(i);
	}
	ASSERT_EQ(V.getDistance(1, 8), 8);
	ASSERT_EQ(V.getDistance(-1, 8), 9);
	ASSERT_EQ(V.getDistance(-1, 9), 10);
	ASSERT_EQ(V.getDistance(-1, 99), 100);
	for (int i = 0; i < count; ++i)
	{
		V.insert(i);
	}
	// it is set!!!
	ASSERT_EQ(V.getDistance(1, 8), 8);
	ASSERT_EQ(V.getDistance(-1, 8), 9);
	ASSERT_EQ(V.getDistance(-1, 9), 10);
	ASSERT_EQ(V.getDistance(-1, 99), 100);
	interface_range<half_avl_tree<float>> V2;
	for (int i = 0; i < count; ++i)
	{
		V2.insert(i * 10);
	}
	ASSERT_EQ(V2.getDistance(1, 8), 0);
	ASSERT_EQ(V2.getDistance(-1, 8), 1);
	ASSERT_EQ(V2.getDistance(-1, 9), 1);
	ASSERT_EQ(V2.getDistance(-1, 99), 10);
	interface_range<half_avl_tree<int>> V3;
	for (int i = count - 1; i > -1; --i)
	{
		V3.insert(i);
	}
	ASSERT_EQ(V3.getDistance(1, 8), 8);
	ASSERT_EQ(V3.getDistance(-1, 8), 9);
	ASSERT_EQ(V3.getDistance(-1, 9), 10);
	ASSERT_EQ(V3.getDistance(-1, 99), 100);
	interface_range<half_avl_tree<int>> V4;
	std::default_random_engine dre;
	std::normal_distribution<> d{ 120.,10.};
	for (int i = 0; i < 100; ++i)
	{
		int tmp = d(dre);
		V4.insert(tmp);
	}
	V4.getDistance(1, 16);
}