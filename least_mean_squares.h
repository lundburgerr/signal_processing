#ifndef SIGNAL_PROCESSING__LEAST_MEAN_SQUARES_H_
#define SIGNAL_PROCESSING__LEAST_MEAN_SQUARES_H_

#include "filter.h"

inline void lms(FirFilterComplex* filter_state, const float* x_real,
                const float* x_imag, float error_real, float error_imag,
                float step_size) {
  float* h_real = filter_state->filter_real;
  float* h_imag = filter_state->filter_imag;
  int filter_size = filter_state->filter_size;
  for (int i = 0; i < filter_size; ++i) {
    float complex_mult_real = error_real * x_real[i] - error_imag * x_imag[i];
    float complex_mult_imag = error_real * x_imag[i] + error_imag * x_real[i];
    h_real[i] += step_size * error_real * complex_mult_real;
    h_imag[i] += step_size * error_imag * complex_mult_imag;
  }
}

// TODO: Implement adaptive step size.
inline float adaptive_step_size(const float* x_real, const float* x_imag,
                                float x_power, float error_real,
                                float error_imag) {
  return 1.0;
}

#endif  // SIGNAL_PROCESSING__LEAST_MEAN_SQUARES_H_
