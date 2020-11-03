#include <gtest/gtest.h>
#include <lru_cache.hpp>
using namespace handmade;

TEST(lru_cache, creation) {
  lru_cache<int, double> c{0};
  EXPECT_EQ(c.check(), true);
  EXPECT_EQ(c.isFull(), true);
  lru_cache<int, float> d{100};
  EXPECT_EQ(d.check(), true);
  EXPECT_EQ(d.isFull(), false);
}
TEST(lru_cache, base) {
  size_t cache_size = 3;

  lru_cache<int, double> c{cache_size};
  EXPECT_EQ(c.check(), true);
  EXPECT_EQ(c.isFull(), false);
  c.writeElement(40, 0.1);
  EXPECT_EQ(c.isFull(), false);
  EXPECT_EQ(std::get<1>(c.getElement(40)), 0.1);
  EXPECT_EQ(std::get<0>(c.getElement(40)), result::hit);
  EXPECT_EQ(std::get<0>(c.getElement(42)), result::miss);
  // in cache 2 elements
  EXPECT_EQ(c.isFull(), false);
  EXPECT_EQ(std::get<0>(c.getElement(42)), result::hit);
  c.writeElement(40, 0.3);
  EXPECT_EQ(c.isFull(), false);
  EXPECT_EQ(std::get<1>(c.getElement(40)), 0.3);
  EXPECT_EQ(std::get<1>(c.getElement(42)), 32); // magic
  c.getElement(22);
  c.getElement(11);
  // c.dump();
  EXPECT_EQ(std::get<0>(c.getElement(40)), result::miss);
  // c.dump();
  c.deleteElement(11);
  // c.dump();
  EXPECT_EQ(std::get<0>(c.getElement(11)), result::miss);

  // c.dump();
  EXPECT_EQ(c.checkElement(11).has_value(), true);
  EXPECT_EQ(c.checkElement(1).has_value(), false);
  // c.dump();
  EXPECT_EQ(c.getTopElement(), std::make_tuple(11, 1.0));
  EXPECT_EQ(c.getDownElement(), std::make_tuple(22, 12.0));
}
int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}