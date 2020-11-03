#include <gtest/gtest.h>
#include <snlru_cache.hpp>

using namespace handmade;

TEST(snlru_cache, creation) {
  snlru_cache<int, double> c{0};
  EXPECT_EQ(c.check(), true);
  EXPECT_EQ(c.isFull(), true);
  snlru_cache<int, float> d{100};
  EXPECT_EQ(d.check(), true);
  EXPECT_EQ(d.isFull(), false);
}
TEST(snlru_cache, base) {
  size_t cache_size = 3;

  snlru_cache<int, double> c{3, 3, 3};
  EXPECT_EQ(c.check(), true);
  EXPECT_EQ(c.isFull(), false);
  for (int i = 0; i < 10; ++i)
    c.writeElement(i + 10, 100 / (1 + i));
  EXPECT_EQ(c.isFull(), false);
  for (int i = 0; i < 10; ++i) {
    c.writeElement(i + 1099, 100 / (1 + i));
    c.writeElement(i + 1099, 100 / (1 + i));
  }
  EXPECT_EQ(c.isFull(), false);
  for (int i = 0; i < 3; ++i) {
    c.writeElement(i + 10993, 100 / (1 + i));
    c.writeElement(i + 10993, 100 / (1 + i));
    c.writeElement(i + 10993, 100 / (1 + i));
  }
  for (int i = 0; i < 10; ++i) {
    c.writeElement(i + 1099, 100 / (1 + i));
    c.writeElement(i + 1099, 100 / (1 + i));
  }
  for (int i = 0; i < 10; ++i)
    c.writeElement(i + 10, 100 / (1 + i));
  EXPECT_EQ(c.isFull(), true);
}
