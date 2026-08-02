#ifndef SEAMCARVE_H
#define SEAMCARVE_H

#include "util_image.h"
#include <vector>

/// Compute the disruption measure for every pixel of an RGB image.
/// Disruption at pixel (x,y) = sum of absolute differences in R,G,B
/// between that pixel and each of its (up to 8) neighbors.
std::vector<std::vector<int>> calculateDisruption(const RGBImage &img);

/// Find a vertical seam (one pixel per row, top-to-bottom) with minimum
/// total disruption. Returns a vector of column indices (1-based).
std::vector<int> findVerticalSeam(const std::vector<std::vector<int>> &d,
                                  int rows, int cols);

/// Find a horizontal seam (one pixel per column, left-to-right) with minimum
/// total disruption. Returns a vector of row indices (1-based).
std::vector<int> findHorizontalSeam(const std::vector<std::vector<int>> &d,
                                    int rows, int cols);

/// Remove one vertical seam from the image, shrinking width by 1.
RGBImage removeVerticalSeam(const RGBImage &img,
                            const std::vector<int> &seam);

/// Remove one horizontal seam from the image, shrinking height by 1.
RGBImage removeHorizontalSeam(const RGBImage &img,
                              const std::vector<int> &seam);

#endif // SEAMCARVE_H