extern "C" {
#include "filter.h"

#include "buffer.h"
}

#include <gtest/gtest.h>

#include <vector>

#include "gmock/gmock.h"

namespace {
using ::testing::ElementsAreArray;

TEST(FirFilterTest, Init) {
  std::vector<float> filter = {1.0, 2.0, 3.0};
  FirFilter fir_filter = fir_init(filter.data(), /*filter_size=*/3);
  EXPECT_EQ(fir_filter.filter_size, 3);
  EXPECT_THAT(filter, ElementsAreArray(fir_filter.filter, 3));
}
}  // namespace
