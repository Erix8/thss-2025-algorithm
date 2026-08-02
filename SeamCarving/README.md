# Seam Carving

A cross-platform C++ command-line tool for content-aware image resizing using the **seam carving** algorithm. Given an input image and desired dimensions, the tool removes the least visually disruptive seams (paths of pixels) until the target size is reached.

## Background

Seam carving is a content-aware image resizing technique originally described by Avidan and Shamir (2007), based on the classic CLRS problem of finding the minimum-disruption seam through an image:

> "Suppose you are given a color picture consisting of an m × n array of pixels. You want to compress this picture slightly, by removing one pixel from each of the m rows, so that the whole picture becomes one pixel narrower. To avoid incongruous visual effects, the pixels removed in two adjacent rows must lie in either the same column or adjacent columns. The pixels removed form a 'seam' from the top row to the bottom row."

The disruption measure of a pixel quantifies how different it is from its neighbors. The goal is to find a seam whose total disruption is minimized, so removing it causes the least visual distortion.

## Algorithm

### 1. Disruption Computation

For each pixel, compute a disruption score by summing the absolute differences in R, G, and B channels between that pixel and each of its up to 8 neighbors. A low disruption score means the pixel blends well with its surroundings and is a good candidate for removal.

### 2. Finding the Optimal Seam

Dynamic programming finds the seam with minimum total disruption.

For a vertical seam (one pixel per row, top-to-bottom):

Define `D[i][j]` = minimum total disruption of a seam ending at row `i`, column `j`.

**Initialization**: `D[1][j] = d[1][j]` for all columns `j`.

**Recurrence** (for `i > 1`):

```
D[i][j] = d[i][j] + min\{ D[i-1][j-1], D[i-1][j], D[i-1][j+1] \}
```

(with boundary checks on column indices). The overall minimum-disruption seam ends at `min_j D[m][j]` and is recovered by backtracing through a `prev[][]` array.

A horizontal seam is found analogously, transposing the row/column roles.

**Complexity**: Θ(mn) time, Θ(mn) space for both vertical and horizontal seams.

### 3. Image Compression

Iteratively remove seams until the image reaches the desired dimensions:

1. Compute the disruption map for the current image.
2. Find the optimal vertical/horizontal seam via DP.
3. Remove that seam, shrinking the image by 1 pixel in the corresponding direction.
4. Repeat until the target width/height is reached.

## Project Structure

```
SeamCarving/
├── README.md
├── CMakeLists.txt              # Cross-platform build
├── third_party/
│   └── stb/
│       ├── stb_image.h         # Header-only image loading
│       └── stb_image_write.h   # Header-only image writing
└── src/
    ├── util_image.h/.cpp       # Simple RGB image container + stb wrappers
    ├── SeamCarve.h/.cpp        # Disruption computation, seam finding, seam removal
    └── main.cpp                # CLI entry point
```

## Dependencies

Zero external dependencies beyond a C++11 compiler.

Image I/O is handled by [stb_image](https://github.com/nothings/stb) (public domain, single-header libraries), which are included in the project. Supported formats: PNG, JPEG, BMP, and more.

## Building

The project uses [CMake](https://cmake.org/) and works natively on Windows, macOS, and Linux:

```bash
cd SeamCarving
cmake -S . -B build
cmake --build build
```

The executable is then located at `build/seam_carving` (or `build\Release\seam_carving.exe` on Windows).

To clean up:

```bash
cmake -E remove_directory build
```

## Usage

```
./build/seam_carving <input_image> <output_image> <desired_width> <desired_height>
```

### Example

```bash
# Compress a 640x480 image down to 500x400
./build/seam_carving forest.png forest_compressed.png 500 400
```

Output:

```
Saved compressed image to forest_compressed.png (500x400)
```

### Error Handling

- If the desired dimensions are larger than the original, the tool exits with an error message.
- If the input file cannot be read (unsupported format, missing file, etc.), the tool reports the failure reason from stb_image and exits.
- If the output cannot be written, the tool reports the error.

## Notes

- The algorithm removes **all** vertical seams first, then all horizontal seams.
- For large images with many seams to remove, the process may take a while since the disruption map is recomputed after each pixel-wide seam removal. This is inherent to the seam carving approach.
- Seam carving was designed as a solution to CLRS Problem 14-8 ("Image compression by seam carving"), which was the original motivation for this project.