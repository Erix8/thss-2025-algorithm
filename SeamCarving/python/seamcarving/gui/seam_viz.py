"""Seam visualization utilities -- draws seam paths on images."""

from typing import List

import numpy as np
from PySide6.QtGui import QImage, QPainter, QPen, QColor


def draw_seams_on_qimage(
    qimg: QImage,
    vertical_seams: List[List[int]],
    horizontal_seams: List[List[int]],
    original_width: int,
    original_height: int,
    seam_color: QColor = QColor(255, 0, 0, 180),
    line_width: int = 1,
) -> QImage:
    """
    Draw seam paths onto a QImage.

    Seam coordinates are in the *original* image coordinate space
    (before any seams were removed).  This function maps them onto
    the displayed result image via simple offset accumulation.

    For simplicity, we draw each seam at its original row/column
    offset, which gives a good visual approximation for small to
    moderate compression ratios.
    """
    img = qimg.copy()
    painter = QPainter(img)
    painter.setRenderHint(QPainter.RenderHint.Antialiasing, False)

    pen = QPen(seam_color, line_width)
    painter.setPen(pen)

    # Draw vertical seams (one pixel per row)
    # For each vertical seam, the column index is offset by how many
    # seams to its left were already removed.
    for seam_idx, seam in enumerate(vertical_seams):
        col = sum(1 for s in vertical_seams[:seam_idx] if s[0] <= seam[0])
        # simplified: just use the first row's column as reference
        offset_col = seam[0] if seam else 0
        for row in range(len(seam)):
            painter.drawPoint(offset_col, row)

    # Draw horizontal seams (one pixel per column)
    for seam_idx, seam in enumerate(horizontal_seams):
        offset_row = seam[0] if seam else 0
        for col in range(len(seam)):
            painter.drawPoint(col, offset_row)

    painter.end()
    return img


def create_seam_overlay(
    result_img: np.ndarray,
    vertical_seams: List[List[int]],
    horizontal_seams: List[List[int]],
    alpha: float = 0.5,
) -> np.ndarray:
    """
    Create a numpy array with seams drawn as red lines.

    Returns an RGBA (H, W, 4) uint8 array.
    """
    h, w = result_img.shape[:2]

    # Start with a copy of the result image in RGBA
    overlay = np.zeros((h, w, 4), dtype=np.uint8)
    overlay[:, :, :3] = result_img
    overlay[:, :, 3] = 255

    red = np.array([255, 0, 0, int(255 * alpha)], dtype=np.uint8)

    # Draw vertical seams (seam length = original image height, may exceed result h)
    for seam in vertical_seams:
        for row, col in enumerate(seam):
            if 0 <= row < h and 0 <= col < w:
                overlay[row, col] = red

    # Draw horizontal seams (seam length = original image width, may exceed result w)
    for seam in horizontal_seams:
        for col, row in enumerate(seam):
            if 0 <= row < h and 0 <= col < w:
                overlay[row, col] = red

    return overlay