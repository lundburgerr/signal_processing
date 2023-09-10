#ifndef SIGNAL_PROCESSING__BUFFER_H_
#define SIGNAL_PROCESSING__BUFFER_H_

#include "simple_math.h"

typedef struct DoubleBuffer {
  float* buffer;  // Size is 2*size.
  const int size;
  int oldest;  // Index to the oldest element.
} DoubleBuffer;

typedef struct DoubleBufferComplex {
  float* buffer_real;  // Size is 2*size.
  float* buffer_imag;  // Size is 2*size.
  const int size;
  int oldest;  // Index to the oldest element.
} DoubleBufferComplex;

// Double buffer with complex values where odd indices are real and even indices
// are imaginary.
typedef struct DoubleBufferComplexInterleaved {
  float* buffer_interleaved;  // Size is 4*size.
  const int size;
  int oldest;  // Index to the oldest element.
} DoubleBufferComplexInterleaved;

// Input buffer has length 2*size.
// Creates a double buffer and sets all buffer values to zero.
static inline DoubleBuffer double_buffer_init(float* buffer, int size) {
  for (int i = 0; i < 2 * size; ++i) {
    buffer[i] = 0;
  }  // Input buffer has length 2*size.
     // Creates a double buffer and sets all buffer values to zero.
  DoubleBuffer double_buffer = {buffer, size, /*oldest*/ 0};
  return double_buffer;
}

// Input buffers has length 2*size.
// Creates a double buffer and sets all buffer values to zero.
static inline DoubleBufferComplex double_buffer_complex_init(float* buffer_real,
                                                             float* buffer_imag,
                                                             int size) {
  for (int i = 0; i < 2 * size; ++i) {
    buffer_real[i] = 0;
    buffer_imag[i] = 0;
  }

  DoubleBufferComplex double_buffer = {buffer_real, buffer_imag, size,
                                       /*oldest*/ 0};
  return double_buffer;
}

// Input buffer has length 4*size.
// Creates a double buffer and sets all buffer values to zero.
static inline DoubleBufferComplexInterleaved
double_buffer_complex_interleaved_init(float* buffer_interleaved, int size) {
  for (int i = 0; i < 4 * size; ++i) {
    buffer_interleaved[i] = 0;
  }

  DoubleBufferComplexInterleaved double_buffer = {buffer_interleaved, size,
                                                  /*oldest*/ 0};
  return double_buffer;
}

// Update the buffer with the new input.
// The length in_size of the new input has to be less than or equal to the size
// of the buffer.
static inline void double_buffer_update(const float* x, int in_size,
                                        DoubleBuffer* double_buffer) {
  int buffer_size = double_buffer->size;
  int start = double_buffer->oldest;
  float* buffer = double_buffer->buffer;

  // Fill in the entire new buffer to the double buffer. We can fill it once
  // without worrying about wrapping since it is a double buffer.
  for (int i = 0; i < in_size; ++i) {
    buffer[start + i] = x[i];
  }

  // Fill in the extra duplicate elements in the double buffer, taking into
  // consideration the case if we are wrapping.
  int wrap_end = MIN(in_size, buffer_size - start);
  for (int i = 0; i < wrap_end; ++i) {
    buffer[buffer_size + start + i] = x[i];
  }
  for (int i = 0; i < in_size - wrap_end; ++i) {
    buffer[i] = x[wrap_end + i];
  }
  double_buffer->oldest = (start + in_size) % buffer_size;
}

static inline void double_buffer_complex_update(
    const float* x_real, const float* x_imag, int in_size,
    DoubleBufferComplex* double_buffer) {
  int buffer_size = double_buffer->size;
  int start = double_buffer->oldest;
  float* buffer_real = double_buffer->buffer_real;
  float* buffer_imag = double_buffer->buffer_imag;

  // Fill in the entire new buffer to the double buffer. We can fill it once
  // without worrying about wrapping since it is a double buffer.
  for (int i = 0; i < in_size; ++i) {
    buffer_real[start + i] = x_real[i];
    buffer_imag[start + i] = x_imag[i];
  }

  // Fill in the extra duplicate elements in the double buffer, taking into
  // consideration the case if we are wrapping.
  int wrap_end = MIN(in_size, buffer_size - start);
  for (int i = 0; i < wrap_end; ++i) {
    buffer_real[buffer_size + start + i] = x_real[i];
    buffer_imag[buffer_size + start + i] = x_imag[i];
  }
  for (int i = 0; i < in_size - wrap_end; ++i) {
    buffer_real[i] = x_real[wrap_end + i];
    buffer_imag[i] = x_imag[wrap_end + i];
  }
  double_buffer->oldest = (start + in_size) % buffer_size;
}

static inline void double_buffer_complex_interleaved_update(
    const float* x_real, const float* x_imag, int in_size,
    DoubleBufferComplexInterleaved* double_buffer) {
  int buffer_size = double_buffer->size;
  int start = double_buffer->oldest;
  float* buffer_interleaved = double_buffer->buffer_interleaved;

  // Fill in the entire new buffer to the double buffer. We can fill it once
  // without worrying about wrapping since it is a double buffer.
  for (int i = 0; i < in_size; ++i) {
    buffer_interleaved[2 * i + start] = x_real[i];
    buffer_interleaved[2 * i + start + 1] = x_imag[i];
  }

  // Fill in the extra duplicate elements in the double buffer, taking into
  // consideration the case if we are wrapping.
  int wrap_end = MIN(in_size, buffer_size - start);
  for (int i = 0; i < wrap_end; ++i) {
    buffer_interleaved[2 * buffer_size + 2 * i + start] = x_real[i];
    buffer_interleaved[2 * buffer_size + 2 * i + start + 1] = x_imag[i];
  }
  for (int i = 0; i < in_size - wrap_end; ++i) {
    buffer_interleaved[2 * i] = x_real[wrap_end + i];
    buffer_interleaved[2 * i + 1] = x_imag[wrap_end + i];
  }
  double_buffer->oldest = (start + 2 * in_size) % (2 * buffer_size);
}

#endif  // SIGNAL_PROCESSING__BUFFER_H_
