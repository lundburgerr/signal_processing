extern "C" {
#include "filter.h"

#include "buffer.h"
}

#include <gtest/gtest.h>

#include <vector>

#include "gmock/gmock.h"

namespace {
using ::testing::ElementsAreArray;
using ::testing::TestWithParam;
using ::testing::Values;

TEST(FirFilterTest, Init) {
  std::vector<float> filter = {1.0, 2.0, 3.0};
  FirFilter fir_filter = fir_init(filter.data(), /*filter_size=*/3);
  EXPECT_EQ(fir_filter.filter_size, 3);
  EXPECT_THAT(filter, ElementsAreArray(fir_filter.filter, 3));
}

class FirFilterParameterized
    : public TestWithParam<std::tuple<std::vector<float>, std::vector<float>,
                                      std::vector<float>>> {
 protected:
  std::vector<float> filter;
  std::vector<float> x;
  std::vector<float> expected;
};

TEST_P(FirFilterParameterized, Filter) {
  std::vector<float> filter = std::get<0>(GetParam());
  FirFilter fir_filter = fir_init(filter.data(), filter.size());

  std::vector<float> buffer(10, 0.0f);
  DoubleBuffer double_buffer = double_buffer_init(buffer.data(), /*size=*/5);

  std::vector<float> x = std::get<1>(GetParam());
  double_buffer_update(x.data(), /*in_size=*/3, &double_buffer);

  std::vector<float> y(x.size(), 0.0f);
  fir_filtering(&double_buffer, /*input_length=*/3, &fir_filter, y.data());

  std::vector<float> expected = std::get<2>(GetParam());
  EXPECT_EQ(expected, y);
}

class FirFilterComplexParameterized
    : public TestWithParam<std::tuple<std::vector<float>, std::vector<float>,
                                      std::vector<float>, std::vector<float>,
                                      std::vector<float>, std::vector<float>>> {
 protected:
  std::vector<float> filter_real;
  std::vector<float> filter_image;
  std::vector<float> x_real;
  std::vector<float> x_imag;
  std::vector<float> expected_real;
  std::vector<float> expected_imag;
};

TEST_P(FirFilterComplexParameterized, Filter) {
  std::vector<float> filter_real = std::get<0>(GetParam());
  std::vector<float> filter_imag = std::get<1>(GetParam());
  FirFilterComplex fir_filter =
      fir_complex_init(filter_real.data(), filter_imag.data(),
                       /*filter_size=*/filter_real.size());

  std::vector<float> buffer_real(10, 0.0f);
  std::vector<float> buffer_imag(10, 0.0f);
  DoubleBufferComplex double_buffer =
      double_buffer_complex_init(buffer_real.data(), buffer_imag.data(),
                                 /*size=*/5);

  std::vector<float> x_real = std::get<2>(GetParam());
  std::vector<float> x_imag = std::get<3>(GetParam());
  double_buffer_complex_update(x_real.data(), x_imag.data(), /*in_size=*/3,
                               &double_buffer);

  std::vector<float> y_real(x_real.size(), 0.0f);
  std::vector<float> y_imag(x_imag.size(), 0.0f);
  fir_filtering_complex(&double_buffer, /*input_length=*/3, &fir_filter,
                        y_real.data(), y_imag.data());

  std::vector<float> expected_real = std::get<4>(GetParam());
  std::vector<float> expected_imag = std::get<5>(GetParam());
  EXPECT_EQ(expected_real, y_real);
  EXPECT_EQ(expected_imag, y_imag);
}

INSTANTIATE_TEST_SUITE_P(
    FirFilterFilteringTests, FirFilterParameterized,
    Values(std::make_tuple(std::vector<float>{0.0, 0.0, 1.0},   // filter
                           std::vector<float>{1.0, 2.0, 3.0},   // x
                           std::vector<float>{1.0, 2.0, 3.0}),  // expected
           std::make_tuple(std::vector<float>{0.0, 0.0, 1.0, 0.0},
                           std::vector<float>{1.0, 2.0, 3.0},
                           std::vector<float>{0.0, 1.0, 2.0})));

INSTANTIATE_TEST_SUITE_P(
    FirFilterComplexParameterized, FirFilterComplexParameterized,
    Values(
        std::make_tuple(std::vector<float>{0.0, 0.0, 1.0},      // filter_real
                        std::vector<float>{0.0, 0.0, 0.0},      // filter_imag
                        std::vector<float>{1.0, 2.0, 3.0},      // x_real
                        std::vector<float>{-1.0, -2.0, -3.0},   // x_imag
                        std::vector<float>{1.0, 2.0, 3.0},      // expected_real
                        std::vector<float>{-1.0, -2.0, -3.0}),  // expected_imag
        std::make_tuple(std::vector<float>{0.0, 0.0, 1.0, 0.0},  // filter_real
                        std::vector<float>{0.0, 0.0, 0.0, 1.0},  // filter_imag
                        std::vector<float>{1.0, 2.0, 3.0},       // x_real
                        std::vector<float>{0.0, 0.0, 1.0},       // x_imag
                        std::vector<float>{0.0, 1.0, 1.0},     // expected_real
                        std::vector<float>{1.0, 2.0, 3.0})));  // expected_imag

}  // namespace
