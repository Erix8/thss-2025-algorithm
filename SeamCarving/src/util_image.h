#ifndef UTIL_IMAGE_H
#define UTIL_IMAGE_H

#include <cstdint>
#include <vector>
#include <string>

/// Simple RGB image container.
struct RGBImage
{
    int width = 0;
    int height = 0;
    /// Interleaved RGB pixels, row-major.
    /// data[3 * (y * width + x) + 0] = R
    /// data[3 * (y * width + x) + 1] = G
    /// data[3 * (y * width + x) + 2] = B
    std::vector<uint8_t> data;
};

/// Read an image file (PNG, JPEG, BMP, etc.) into an RGB buffer.
RGBImage readImage(const std::string &filename);

/// Write an RGB buffer to an image file (PNG, JPEG, BMP, etc.).
/// Format is determined from the file extension.
void writeImage(const RGBImage &img, const std::string &filename);

/// Access a pixel by coordinates (0-indexed).
inline void getPixel(const RGBImage &img, int x, int y,
                     uint8_t &r, uint8_t &g, uint8_t &b)
{
    int idx = 3 * (y * img.width + x);
    r = img.data[idx];
    g = img.data[idx + 1];
    b = img.data[idx + 2];
}

/// Set a pixel at the given coordinates.
inline void setPixel(RGBImage &img, int x, int y,
                     uint8_t r, uint8_t g, uint8_t b)
{
    int idx = 3 * (y * img.width + x);
    img.data[idx] = r;
    img.data[idx + 1] = g;
    img.data[idx + 2] = b;
}

#endif // UTIL_IMAGE_H