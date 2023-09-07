#ifndef SIGNAL_PROCESSING__FILTER_H_
#define SIGNAL_PROCESSING__FILTER_H_

#include "buffer.h"

// Filter should be in reverse order.
// Meaning the zero delay coefficient is the last element.
// For example the impulse response is [0, 0, ..., 1].
typedef struct FirFilter {
  float* filter;
  const int filter_size;
} FirFilter;

typedef struct FirFilterComplex {
  float* filter_real;
  float* filter_imag;
  const int filter_size;
} FirFilterComplex;

inline FirFilter fir_init(float* filter, int filter_size) {
  return {filter, filter_size};
}

inline FirFilterComplex fir_complex_init(float* filter_real,
                                         float* filter_imag,
                                         int filter_size) {
  return {filter_real, filter_imag, filter_size};
}

// FIR filter using a double buffer. The double buffer is assumed to be of at
// least length input_length + filter_size - 1.
inline void fir_filtering(const DoubleBuffer* double_buffer, int input_length,
                          const FirFilter* fir_filter, float* y) {
  const float* buffer = double_buffer->buffer;
  const int buffer_size = double_buffer->size;
  const float* h = fir_filter->filter;
  const int filter_size = fir_filter->filter_size;
  const int start =
      double_buffer->oldest + buffer_size - input_length - filter_size + 1;

  // For loops using delay buffer.
  for (int i = 0; i < input_length; ++i) {
    y[i] = 0.0;
    for (int j = 0; j < filter_size; ++j) {
      y[i] += h[j] * buffer[start + i + j];
    }
  }
}

#endif  // SIGNAL_PROCESSING__FILTER_H_