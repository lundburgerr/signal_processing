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

TEST(DoubleBufferComplexInterleavedTest, Init) {
  int size = 5;
  std::vector<float> buffer_interleaved(4 * size, 1.0f);
  DoubleBufferComplexInterleaved double_buffer =
      double_buffer_complex_interleaved_init(buffer_interleaved.data(), size);
  EXPECT_EQ(double_buffer.size, size);
  EXPECT_EQ(double_buffer.oldest, 0);

  std::vector<float> expected(20, 0.0f);
  EXPECT_THAT(expected, ElementsAreArray(double_buffer.buffer_interleaved, 20));
}

TEST(DoubleBufferTest, Update) {
  int size = 5;
  std::vector<float> buffer(2 * size, 1.0f);
  DoubleBuffer double_buffer = double_buffer_init(buffer.data(), size);
  EXPECT_EQ(double_buffer.size, size);

  // Fill in values without wrapping.
  std::vector<float> x = {1.0, 2.0, 3.0};
  double_buffer_update(x.data(), /*in_size=*/3, &double_buffer);
  EXPECT_EQ(double_buffer.oldest, 3);
  std::vector<float> expected = {1.0, 2.0, 3.0, 0.0, 0.0,
                                 1.0, 2.0, 3.0, 0.0, 0.0};
  EXPECT_THAT(expected, ElementsAreArray(double_buffer.buffer, 10));

  // Fill in values with wrapping.
  std::vector<float> x2 = {4.0, 5.0, 6.0, 7.0};
  double_buffer_update(x2.data(), /*in_size=*/4, &double_buffer);
  EXPECT_EQ(double_buffer.oldest, 2);
  std::vector<float> expected2 = {6.0, 7.0, 3.0, 4.0, 5.0,
                                  6.0, 7.0, 3.0, 4.0, 5.0};
  EXPECT_THAT(expected2, ElementsAreArray(double_buffer.buffer, 10));
}

TEST(DoubleBufferComplexTest, Update) {
  int size = 5;
  std::vector<float> buffer_real(2 * size, 1.0f);
  std::vector<float> buffer_imag(2 * size, 1.0f);
  DoubleBufferComplex double_buffer =
      double_buffer_complex_init(buffer_real.data(), buffer_imag.data(), size);
  EXPECT_EQ(double_buffer.size, size);

  // Fill in values without wrapping.
  std::vector<float> x_real = {1.0, 2.0, 3.0};
  std::vector<float> x_imag = {-1.0, -2.0, -3.0};
  double_buffer_complex_update(x_real.data(), x_imag.data(), /*in_size=*/3,
                               &double_buffer);
  std::vector<float> expected_real = {1.0, 2.0, 3.0, 0.0, 0.0,
                                      1.0, 2.0, 3.0, 0.0, 0.0};
  std::vector<float> expected_imag = {-1.0, -2.0, -3.0, 0.0, 0.0,
                                      -1.0, -2.0, -3.0, 0.0, 0.0};
  EXPECT_EQ(double_buffer.oldest, 3);
  EXPECT_THAT(expected_real, ElementsAreArray(double_buffer.buffer_real, 10));
  EXPECT_THAT(expected_imag, ElementsAreArray(double_buffer.buffer_imag, 10));

  // Fill in values with wrapping.
  std::vector<float> x_real2 = {4.0, 5.0, 6.0, 7.0};
  std::vector<float> x_imag2 = {-4.0, -5.0, -6.0, -7.0};
  double_buffer_complex_update(x_real2.data(), x_imag2.data(),
                               /*in_size=*/4, &double_buffer);
  std::vector<float> expected_real2 = {6.0, 7.0, 3.0, 4.0, 5.0,
                                       6.0, 7.0, 3.0, 4.0, 5.0};
  std::vector<float> expected_imag2 = {-6.0, -7.0, -3.0, -4.0, -5.0,
                                       -6.0, -7.0, -3.0, -4.0, -5.0};
  EXPECT_EQ(double_buffer.oldest, 2);
  EXPECT_THAT(expected_real2, ElementsAreArray(double_buffer.buffer_real, 10));
  EXPECT_THAT(expected_imag2, ElementsAreArray(double_buffer.buffer_imag, 10));
}

TEST(DoubleBufferComplexInterleavedTest, update) {
  int size = 5;
  std::vector<float> buffer_interleaved(4 * size, 1.0f);
  DoubleBufferComplexInterleaved double_buffer =
      double_buffer_complex_interleaved_init(buffer_interleaved.data(), size);
  EXPECT_EQ(double_buffer.size, size);

  // Fill in values without wrapping.
  std::vector<float> x_real = {1.0, 2.0, 3.0};
  std::vector<float> x_imag = {-1.0, -2.0, -3.0};
  double_buffer_complex_interleaved_update(x_real.data(), x_imag.data(),
                                           /*in_size=*/3, &double_buffer);
  std::vector<float> expected = {1.0, -1.0, 2.0, -2.0, 3.0,  -3.0, 0.0,
                                 0.0, 0.0,  0.0, 1.0,  -1.0, 2.0,  -2.0,
                                 3.0, -3.0, 0.0, 0.0,  0.0,  0.0};
  EXPECT_EQ(double_buffer.oldest, 6);
  EXPECT_THAT(expected, ElementsAreArray(double_buffer.buffer_interleaved, 20));

  // Fill in values with wrapping.
  std::vector<float> x_real2 = {4.0, 5.0, 6.0, 7.0};
  std::vector<float> x_imag2 = {-4.0, -5.0, -6.0, -7.0};
  double_buffer_complex_interleaved_update(x_real2.data(), x_imag2.data(),
                                           /*in_size=*/4, &double_buffer);
  std::vector<float> expected2 = {6.0,  -6.0, 7.0,  -7.0, 3.0,  -3.0, 4.0,
                                  -4.0, 5.0,  -5.0, 6.0,  -6.0, 7.0,  -7.0,
                                  3.0,  -3.0, 4.0,  -4.0, 5.0,  -5.0};
  EXPECT_EQ(double_buffer.oldest, 4);
  EXPECT_THAT(expected, ElementsAreArray(double_buffer.buffer_interleaved, 20));
}

}  // namespace
