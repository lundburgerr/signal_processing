#ifndef SIGNAL_PROCESSING__BUFFER_H_
#define SIGNAL_PROCESSING__BUFFER_H_

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

// Creates a double buffer and sets all buffer values to zero.
inline DoubleBuffer double_buffer_init(float* buffer, int size) {
  for (int i = 0; i < 2 * size; ++i) {
    buffer[i] = 0;
  }
  return {buffer, size, /*oldest*/ 0};
}

inline DoubleBufferComplex double_buffer_complex_init(float* buffer_real,
                                                      float* buffer_imag,
                                                      int size) {
  for (int i = 0; i < 2 * size; ++i) {
    buffer_real[i] = 0;
    buffer_imag[i] = 0;
  }
  return {buffer_real, buffer_imag, size, /*oldest*/ 0};
}

// Update the buffer with the new input.
// The length in_size of the new input has to be less than or equal to the size
// of the buffer.
inline void double_buffer_update(const float* x, int in_size,
                                 DoubleBuffer* double_buffer) {
  int buffer_size = double_buffer->size;
  int start = double_buffer->oldest;
  float* buffer = double_buffer->buffer;
  for (int i = 0; i < in_size; ++i) {
    buffer[start + i] = x[i];
  }
  for (int i = 0; i < in_size; ++i) {
    buffer[buffer_size + start + i] = x[i];
  }
  double_buffer->oldest = (start + in_size) % buffer_size;
}

inline void double_buffer_complex_update(const float* x_real,
                                         const float* x_imag, int in_size,
                                         DoubleBufferComplex* double_buffer) {
  int buffer_size = double_buffer->size;
  int start = double_buffer->oldest;
  float* buffer_real = double_buffer->buffer_real;
  float* buffer_imag = double_buffer->buffer_imag;
  for (int i = 0; i < in_size; ++i) {
    buffer_real[start + i] = x_real[i];
    buffer_imag[start + i] = x_imag[i];
  }
  for (int i = 0; i < in_size; ++i) {
    buffer_real[buffer_size + start + i] = x_real[i];
    buffer_imag[buffer_size + start + i] = x_imag[i];
  }
  double_buffer->oldest = (start + in_size) % buffer_size;
}

#endif  // SIGNAL_PROCESSING__BUFFER_H_
