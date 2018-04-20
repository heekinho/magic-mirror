#ifndef IMAGE_OPERATIONS_H_
#define IMAGE_OPERATIONS_H_

#include "string.h"

//Helper class style is the best here?
class ImageOperations {
  public:

    /**
     * Flips the image vertically.
     * This is a somewhat naive implementation and can be tuned
     * This version do not change original image
     */
    static char* FlipVertical(const char* image, size_t cols, size_t rows);

    /**
     * Flips the image horizontally.
     * This is a somewhat naive implementation and can be tuned
     * This version do not change original image
     */
    static char* FlipHorizontal(const char *image, size_t cols, size_t rows);
};


#endif /* IMAGE_OPERATIONS_H_ */
