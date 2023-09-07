
#include "least_mean_squares.h"

#include <gtest/gtest.h>

#include <vector>

#include "gmock/gmock.h"

namespace {

using ::testing::FloatNear;
using ::testing::Pointwise;

TEST(LeastMeanSquares, Step) {
  std::vector<float> h_real = {1.0, 1.0, 1.0};
  std::vector<float> h_imag = {1.0, 1.0, 1.0};
  FirFilterComplex filter_state =
      fir_complex_init(h_real.data(), h_imag.data(), 3);
  std::vector<float> x_real = {1.0, 1.0, 2.0};
  std::vector<float> x_imag = {-1.0, 1.0, 3.0};
  float error_real = 1.0;
  float error_imag = 1.0;
  float step_size = 0.1;
  lms(&filter_state, x_real.data(), x_imag.data(), error_real, error_imag,
      step_size);
  std::vector<float> expected_h_real = {1.2, 1.0, 0.9};
  std::vector<float> expected_h_imag = {1.0, 1.2, 1.5};
  EXPECT_THAT(expected_h_real, Pointwise(FloatNear(0.001), h_real));
  EXPECT_THAT(expected_h_imag, Pointwise(FloatNear(0.001), h_imag));
}
}  // namespace