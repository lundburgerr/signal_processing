#ifndef SIGNAL_PROCESSING_BUFFER_H
#define SIGNAL_PROCESSING_BUFFER_H

typedef struct DoubleBuffer {
  float* buffer;  // Size is 2*size.
  const int size;
  int oldest;  // Index to the oldest element.
} DoubleBuffer;

// Creates a double buffer and sets all buffer values to zero.
inline DoubleBuffer double_buffer_init(float* buffer, int size) {
  for (int i = 0; i < 2 * size; ++i) {
    buffer[i] = 0;
  }
  return {buffer, size, /*oldest*/ 0};
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

#endif  // SIGNAL_PROCESSING_BUFFER_H
