"""
Seam Carving -- Python package with C++ acceleration.

Provides:
- High-level ``compress()`` -> compressed numpy array + seam data
- ``compress_to_file()`` -> read/write images via Pillow
- CLI entry point
"""

from seamcarving.core import (
    compress,
    compress_to_file,
    calculate_disruption,
    find_vertical_seam,
    find_horizontal_seam,
    remove_vertical_seam,
    remove_horizontal_seam,
)

__version__ = "1.0.0"
__all__ = [
    "compress",
    "compress_to_file",
    "calculate_disruption",
    "find_vertical_seam",
    "find_horizontal_seam",
    "remove_vertical_seam",
    "remove_horizontal_seam",
]
