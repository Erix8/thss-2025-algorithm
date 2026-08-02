"""
Core module -- wraps the C++ pybind11 extension to provide
Pythonic APIs using numpy arrays and Pillow images.
"""

import time
from typing import List, Tuple

import numpy as np
from PIL import Image

# Import the compiled C++ extension
try:
    from . import _seamcarving_core as _cpp
except ImportError:
    _cpp = None  # gracefully degrade if extension not built yet


# Re-export low-level functions for advanced use
def _get_cpp():
    if _cpp is None:
        raise ImportError(
            "C++ extension _seamcarving_core not built. "
            "Run 'pip install -e .' to compile it."
        )
    return _cpp


def calculate_disruption(img: np.ndarray) -> np.ndarray:
    return _get_cpp().calculate_disruption(img)


def find_vertical_seam(img: np.ndarray) -> List[int]:
    return _get_cpp().find_vertical_seam(img)


def find_horizontal_seam(img: np.ndarray) -> List[int]:
    return _get_cpp().find_horizontal_seam(img)


def remove_vertical_seam(img: np.ndarray, seam: List[int]) -> np.ndarray:
    return _get_cpp().remove_vertical_seam(img, seam)


def remove_horizontal_seam(img: np.ndarray, seam: List[int]) -> np.ndarray:
    return _get_cpp().remove_horizontal_seam(img, seam)


def pil_to_numpy(img: Image.Image) -> np.ndarray:
    """Convert a PIL RGB image to a (H, W, 3) uint8 numpy array."""
    if img.mode != "RGB":
        img = img.convert("RGB")
    return np.asarray(img, dtype=np.uint8)


def numpy_to_pil(arr: np.ndarray) -> Image.Image:
    """Convert a (H, W, 3) uint8 numpy array to a PIL RGB image."""
    return Image.fromarray(arr, mode="RGB")


def compress(
    img: np.ndarray,
    desired_w: int,
    desired_h: int,
) -> Tuple[np.ndarray, List[List[int]], List[List[int]], float]:
    """
    Compress an image using seam carving.

    Args:
        img: (H, W, 3) uint8 numpy array.
        desired_w: Target width (must be <= img width).
        desired_h: Target height (must be <= img height).

    Returns:
        Tuple of:
        - result: Compressed image as (H', W', 3) uint8 numpy array.
        - vertical_seams: List of removed vertical seams (0-based col indices).
        - horizontal_seams: List of removed horizontal seams (0-based row indices).
        - elapsed_ms: Total computation time in milliseconds.
    """
    if desired_w > img.shape[1] or desired_h > img.shape[0]:
        raise ValueError(
            f"Desired size ({desired_w}x{desired_h}) "
            f"exceeds image size ({img.shape[1]}x{img.shape[0]})"
        )

    t0 = time.perf_counter()
    result, v_seams, h_seams = _get_cpp().compress_image(img, desired_w, desired_h)
    elapsed = (time.perf_counter() - t0) * 1000.0

    return result, v_seams, h_seams, elapsed


def compress_to_file(
    input_path: str,
    output_path: str,
    desired_w: int,
    desired_h: int,
) -> Tuple[List[List[int]], List[List[int]], float]:
    """
    Read an image file, compress it, and write the result.

    Returns:
        Tuple of (vertical_seams, horizontal_seams, elapsed_ms).
    """
    img = Image.open(input_path)
    arr = pil_to_numpy(img)
    result, v_seams, h_seams, elapsed = compress(arr, desired_w, desired_h)
    numpy_to_pil(result).save(output_path)
    return v_seams, h_seams, elapsed