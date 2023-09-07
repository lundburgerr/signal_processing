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

TEST(DoubleBufferComplexTest, Init) {
  int size = 5;
  std::vector<float> buffer_real(2 * size, 1.0f);
  std::vector<float> buffer_imag(2 * size, 1.0f);
  DoubleBufferComplex double_buffer =
      double_buffer_complex_init(buffer_real.data(), buffer_imag.data(), size);
  EXPECT_EQ(double_buffer.size, size);
  EXPECT_EQ(double_buffer.oldest, 0);

  std::vector<float> expected(10, 0.0f);
  EXPECT_THAT(expected, ElementsAreArray(double_buffer.buffer_real, 10));
  EXPECT_THAT(expected, ElementsAreArray(double_buffer.buffer_imag, 10));
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

TEST(DoubleBufferComplexTest, Update) {
  int size = 5;
  std::vector<float> buffer_real(2 * size, 1.0f);
  std::vector<float> buffer_imag(2 * size, 1.0f);
  DoubleBufferComplex double_buffer =
      double_buffer_complex_init(buffer_real.data(), buffer_imag.data(), size);

  std::vector<float> x_real = {1.0, 2.0, 3.0};
  std::vector<float> x_imag = {-1.0, -2.0, -3.0};
  double_buffer_complex_update(x_real.data(), x_imag.data(), /*in_size=*/3,
                               &double_buffer);
  EXPECT_EQ(double_buffer.size, size);
  EXPECT_EQ(double_buffer.oldest, 3);

  std::vector<float> expected_real = {1.0, 2.0, 3.0, 0.0, 0.0,
                                      1.0, 2.0, 3.0, 0.0, 0.0};
  std::vector<float> expected_imag = {-1.0, -2.0, -3.0, 0.0, 0.0,
                                      -1.0, -2.0, -3.0, 0.0, 0.0};
  EXPECT_THAT(expected_real, ElementsAreArray(double_buffer.buffer_real, 10));
  EXPECT_THAT(expected_imag, ElementsAreArray(double_buffer.buffer_imag, 10));
}

}  // namespace
