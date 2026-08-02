"""Image display panel with left/right comparison view."""

import numpy as np
from PySide6.QtCore import Qt
from PySide6.QtGui import QImage, QPixmap
from PySide6.QtWidgets import (
    QLabel, QVBoxLayout, QHBoxLayout, QWidget, QScrollArea, QSizePolicy,
)

from seamcarving.core import pil_to_numpy
from seamcarving.gui.seam_viz import create_seam_overlay


def _numpy_to_qpixmap(arr):
    """Convert a (H, W, 3) or (H, W, 4) uint8 array to QPixmap."""
    h, w, c = arr.shape
    if c == 3:
        fmt = QImage.Format.Format_RGB888
    elif c == 4:
        fmt = QImage.Format.Format_RGBA8888
    else:
        raise ValueError(f"Unsupported channels: {c}")
    qimg = QImage(arr.data.tobytes(), w, h, w * c, fmt)
    return QPixmap.fromImage(qimg.copy())


class ImagePanel(QWidget):
    """Side-by-side original and result image display."""

    def __init__(self, parent=None):
        super().__init__(parent)
        self._original_arr = None
        self._result_arr = None
        self._v_seams = []
        self._h_seams = []
        self._show_seams = False
        self._init_ui()

    def _init_ui(self):
        layout = QHBoxLayout(self)
        layout.setContentsMargins(0, 0, 0, 0)

        # --- Original image ---
        left = QVBoxLayout()
        left_header = QLabel("<b>Original</b>")
        left_header.setAlignment(Qt.AlignmentFlag.AlignCenter)
        left.addWidget(left_header)

        self._left_scroll = QScrollArea()
        self._left_scroll.setWidgetResizable(True)
        self._left_scroll.setAlignment(Qt.AlignmentFlag.AlignCenter)
        self._left_label = QLabel("No image loaded")
        self._left_label.setAlignment(Qt.AlignmentFlag.AlignCenter)
        self._left_label.setSizePolicy(
            QSizePolicy.Policy.Ignored, QSizePolicy.Policy.Ignored
        )
        self._left_scroll.setWidget(self._left_label)

        self._left_info = QLabel("")
        self._left_info.setAlignment(Qt.AlignmentFlag.AlignCenter)
        left.addWidget(self._left_scroll)
        left.addWidget(self._left_info)
        layout.addLayout(left)

        # --- Result image ---
        right = QVBoxLayout()
        right_header = QLabel("<b>Result</b>")
        right_header.setAlignment(Qt.AlignmentFlag.AlignCenter)
        right.addWidget(right_header)

        self._right_scroll = QScrollArea()
        self._right_scroll.setWidgetResizable(True)
        self._right_scroll.setAlignment(Qt.AlignmentFlag.AlignCenter)
        self._right_label = QLabel("Not processed yet")
        self._right_label.setAlignment(Qt.AlignmentFlag.AlignCenter)
        self._right_label.setSizePolicy(
            QSizePolicy.Policy.Ignored, QSizePolicy.Policy.Ignored
        )
        self._right_scroll.setWidget(self._right_label)

        self._right_info = QLabel("")
        self._right_info.setAlignment(Qt.AlignmentFlag.AlignCenter)
        right.addWidget(self._right_scroll)
        right.addWidget(self._right_info)
        layout.addLayout(right)

    def load_original(self, pil_image):
        """Display a PIL image in the left panel."""
        arr = pil_to_numpy(pil_image)
        self._original_arr = arr
        pixmap = _numpy_to_qpixmap(arr)
        self._left_label.setPixmap(pixmap)
        self._left_label.setMinimumSize(1, 1)
        h, w = arr.shape[:2]
        self._left_info.setText(f"{w} x {h} px")

    def show_result(self, result, v_seams, h_seams, show_seams=False):
        """Display the compressed result in the right panel."""
        self._result_arr = result
        self._v_seams = v_seams
        self._h_seams = h_seams
        self._show_seams = show_seams
        self._update_result_display()

    def set_show_seams(self, show):
        """Toggle seam visibility overlay on the ORIGINAL image."""
        self._show_seams = show
        if self._original_arr is not None:
            self._update_left_display()

    def _update_left_display(self):
        """Redraw original image, optionally with seam overlay."""
        if self._original_arr is None:
            return
        if self._show_seams and (self._v_seams or self._h_seams):
            overlay = create_seam_overlay(
                self._original_arr, self._v_seams, self._h_seams, alpha=0.6
            )
            pixmap = _numpy_to_qpixmap(overlay)
        else:
            pixmap = _numpy_to_qpixmap(self._original_arr)
        self._left_label.setPixmap(pixmap)
        self._left_label.setMinimumSize(1, 1)

    def _update_result_display(self):
        if self._result_arr is None:
            return
        pixmap = _numpy_to_qpixmap(self._result_arr)
        self._right_label.setPixmap(pixmap)
        self._right_label.setMinimumSize(1, 1)
        h, w = self._result_arr.shape[:2]
        self._right_info.setText(f"{w} x {h} px")

    def get_original_arr(self):
        return self._original_arr

    def get_result_arr(self):
        return self._result_arr