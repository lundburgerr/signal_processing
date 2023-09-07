extern "C" {
#include "filter.h"

#include "buffer.h"
}

#include <gtest/gtest-param-test.h>
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
  int filter_size;
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

INSTANTIATE_TEST_SUITE_P(
    FirFilterFilteringTests, FirFilterParameterized,
    Values(std::make_tuple(std::vector<float>{0.0, 0.0, 1.0},
                           std::vector<float>{1.0, 2.0, 3.0},
                           std::vector<float>{1.0, 2.0, 3.0}),
           std::make_tuple(std::vector<float>{0.0, 0.0, 1.0, 0.0},
                           std::vector<float>{1.0, 2.0, 3.0},
                           std::vector<float>{0.0, 1.0, 2.0})));

}  // namespace
