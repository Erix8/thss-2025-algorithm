# Seam Carving

A cross-platform C++ tool + Python GUI for content-aware image resizing using the **seam carving** algorithm.

## Background

Seam carving (Avidan & Shamir, 2007) finds the minimum-disruption path of pixels (a "seam") through an image and removes it, shrinking the image by one pixel per seam. Built as a solution to CLRS Problem 14-8.

## Project Structure

```
SeamCarving/
├── README.md
├── build.py                     # Cross-platform build helper
├── CMakeLists.txt               # CMake build (CLI + Python module)
├── pyproject.toml               # Python package metadata
├── setup.py                     # setuptools packaging
├── data/                        # Sample images
│   └── image.jpeg               #    Test image (552×556)
│
├── src/                         # ⭐ ALGORITHM CORE ⭐
│   ├── SeamCarve.h              #    disruption, seam finding, seam removal
│   ├── SeamCarve.cpp            #    (all DP logic lives here)
│   ├── util_image.h             #    RGB image container + pixel helpers
│   ├── util_image.cpp           #    stb_image read/write wrappers
│   └── main.cpp                 #    C++ CLI entry point
│
├── pybind/                      # Python ↔ C++ bridge
│   └── seamcarving_bind.cpp     #    pybind11 bindings (numpy ↔ RGBImage)
│
├── python/seamcarving/          # Python package
│   ├── __init__.py
│   ├── core.py                  #    High-level APIs (numpy/Pillow)
│   ├── cli.py                   #    Python CLI (seamcarving-cli)
│   └── gui/                     #    PySide6 GUI
│       ├── __init__.py
│       ├── main_window.py       #       QMainWindow
│       ├── control_panel.py     #       Left-side controls
│       ├── image_panel.py       #       Side-by-side preview
│       ├── seam_viz.py          #       Seam overlay rendering
│       └── worker.py            #       QThread background worker
│
└── third_party/stb/             # stb_image (public domain)
    ├── stb_image.h
    └── stb_image_write.h
```

**Algorithm core**: `src/SeamCarve.h` / `src/SeamCarve.cpp` — disruption computation, DP seam finding, seam removal. The rest is I/O, bindings, and UI.

## Dependencies

### CLI only (C++ executable)
- C++11 compiler (GCC, Clang, or MSVC)
- CMake ≥ 3.18

### Python package (CLI + GUI)
- Python ≥ 3.8
- numpy, Pillow (`pip install seamcarving` brings these automatically)
- PySide6 6.5+ for GUI (`pip install seamcarving[gui]`)

## Building

### Quick: `build.py` (all platforms)

```bash
# Full build (CLI + Python module)
python build.py

# CLI only (C++ executable, no Python deps required)
python build.py cli
```

### Manual: CMake

**macOS / Linux**:
```bash
# Full build (requires pybind11 from pip)
PYBIND_DIR=$(python3 -c "import pybind11; print(pybind11.get_cmake_dir())")
cmake -S . -B build -Dpybind11_DIR="$PYBIND_DIR"
cmake --build build

# CLI only
cmake -S . -B build
cmake --build build --target seam_carving
```

**Windows (PowerShell)**:
```powershell
# Full build (requires pybind11 from pip)
$pybind11_DIR = python -c "import pybind11; print(pybind11.get_cmake_dir())"
cmake -S . -B build -Dpybind11_DIR="$pybind11_DIR"
cmake --build build --config Release

# CLI only
cmake -S . -B build
cmake --build build --target seam_carving --config Release
```

Executable locations:
- macOS/Linux: `build/seam_carving`
- Windows: `build\Release\seam_carving.exe`

## Python Installation

```bash
# CLI only (no GUI deps)
pip install -e .

# With GUI
pip install -e ".[gui]"
```

## Usage

### C++ CLI

```
./build/seam_carving <input> <output> <width> <height>
```

### Python CLI

```
seamcarving-cli <input> <output> <width> <height>
```

### Python GUI

```bash
seamcarving-gui
```

GUI features: open image → set target width/height → compress → side-by-side comparison → show removed seams on original → save result.

### Example

```bash
# Use the included sample: data/image.jpeg (552×556)
# 552×556 → 400×400, removes 308 seams
seamcarving-cli data/image.jpeg output.jpg 400 400
```

Output:
```
Saved compressed image to output.jpg (400x400)
  Removed 152 vertical + 156 horizontal = 308 seams in 6836.00 ms
```

## Algorithm

All algorithm logic is in **`src/SeamCarve.h`** and **`src/SeamCarve.cpp`**.

### 1. Disruption — `calculateDisruption()`
For each pixel, sum absolute R/G/B differences with up to 8 neighbors. Low disruption = blends well → good removal candidate.

### 2. Seam Finding — `findVerticalSeam()` / `findHorizontalSeam()`
Dynamic programming. Let `D[i][j]` = min disruption of a seam ending at row `i`, column `j`:

```
D[1][j] = d[1][j]
D[i][j] = d[i][j] + min{ D[i-1][j-1], D[i-1][j], D[i-1][j+1] }
```

Backtrace through a `prev[][]` array to recover the optimal seam. Complexity: Θ(mn) per seam.

### 3. Seam Removal — `removeVerticalSeam()` / `removeHorizontalSeam()`
Remove one pixel per row (or column) by shifting remaining pixels left/up.

### 4. Compression — `main.cpp` / `core.py`
Iterate: compute disruption → find seam → remove seam, until target dimensions reached. Vertical seams removed first, then horizontal.

## Error Handling

- Desired size > original → error + exit
- Missing/unreadable input file → error message + exit
- Unsupported image format → stb_image error forwarded

## Notes

- The disruption map is recomputed after every seam removal (inherent to seam carving, Θ(mn) per iteration)
- All Python code uses numpy arrays; C++ core operates on `RGBImage` structs converted via pybind11
- `stb_image.h` / `stb_image_write.h` are public domain single-header libraries included in `third_party/`