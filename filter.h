#ifndef SIGNAL_PROCESSING_FILTER_H
#define SIGNAL_PROCESSING_FILTER_H

#include "buffer.h"

// Filter should be in reverse order.
// Meaning the zero delay coefficient is the last element.
// For example the impulse response is [0, 0, ..., 1].
typedef struct FirFilter {
  const float* filter;
  const int filter_size;
} FirFilter;

inline FirFilter fir_init(const float* filter, int filter_size) {
  return {filter, filter_size};
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

#endif  // SIGNAL_PROCESSING_FILTER_H