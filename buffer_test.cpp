extern "C" {
#include "buffer.h"
}

#include <gtest/gtest.h>

#include <vector>

#include "gmock/gmock.h"

namespace {

using ::testing::ElementsAreArray;

TEST(DoubleBufferTest, Init) {
  int size = 5;
  std::vector<float> buffer(2 * size, 1.0f);
  DoubleBuffer double_buffer = double_buffer_init(buffer.data(), size);
  EXPECT_EQ(double_buffer.size, size);
  EXPECT_EQ(double_buffer.oldest, 0);

  std::vector<float> expected(10, 0.0f);
  EXPECT_THAT(expected, ElementsAreArray(double_buffer.buffer, 10));
}

TEST(DoubleBufferTest, Update) {
  int size = 5;
  std::vector<float> buffer(2 * size, 1.0f);
  DoubleBuffer double_buffer = double_buffer_init(buffer.data(), size);

  std::vector<float> x = {1.0, 2.0, 3.0};
  double_buffer_update(x.data(), /*in_size=*/3, &double_buffer);
  EXPECT_EQ(double_buffer.size, size);
  EXPECT_EQ(double_buffer.oldest, 3);

  std::vector<float> expected = {1.0, 2.0, 3.0, 0.0, 0.0,
                                 1.0, 2.0, 3.0, 0.0, 0.0};
  EXPECT_THAT(expected, ElementsAreArray(double_buffer.buffer, 10));
}
}  // namespace
