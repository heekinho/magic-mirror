#include "image_operations.h"

/**
 * Flips the image vertically.
 * This is a somewhat naive implementation and can be tuned
 * This version do not change original image
 */
char* ImageOperations::FlipVertical(const char* image, size_t cols,
    size_t rows) {
  size_t num_bytes = rows * cols;
  char* result = new char[num_bytes];

  for (int row = 0; row < rows; row++) {
    // Here we can copy entire rows
    memcpy(result + row * cols, image + (rows - 1 - row) * cols,
        sizeof(char) * cols);
  }

  return result;
}

/**
 * Flips the image horizontally.
 * This is a somewhat naive implementation and can be tuned
 * This version do not change original image
 */
char* ImageOperations::FlipHorizontal(const char *image, size_t cols,
    size_t rows) {
  size_t num_bytes = rows * cols;
  char* result = new char[num_bytes];

  for (int row = 0; row < rows; row++) {
    for (int col = 0; col < cols; col++) {
      result[row * cols + col] = image[row * cols + (cols - col - 1)];
    }
  }

  return result;
}
