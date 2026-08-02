"""Background worker thread for seam carving computation."""

from typing import List

import numpy as np
from PySide6.QtCore import QThread, Signal

from seamcarving.core import compress


class CompressWorker(QThread):
    """
    Runs seam carving in a background thread so the UI stays responsive.

    Emits:
        progress(step, total)  -- after each seam removal
        finished(result, v_seams, h_seams, elapsed_ms)
        error(message)
    """

    progress = Signal(int, int)  # current_step, total_steps
    finished = Signal(np.ndarray, list, list, float)
    error = Signal(str)

    def __init__(self, img: np.ndarray, desired_w: int, desired_h: int, parent=None):
        super().__init__(parent)
        self._img = img.copy()
        self._desired_w = desired_w
        self._desired_h = desired_h

    def run(self):
        """Execute compression in the background."""
        try:
            h, w = self._img.shape[:2]
            total_steps = (w - self._desired_w) + (h - self._desired_h)

            # We call compress() and report progress here.
            # Since compress() is a single C++ call, we report
            # coarse progress before/after.
            self.progress.emit(0, total_steps)

            result, v_seams, h_seams, elapsed = compress(
                self._img, self._desired_w, self._desired_h
            )

            self.progress.emit(total_steps, total_steps)
            self.finished.emit(result, v_seams, h_seams, elapsed)

        except Exception as e:
            self.error.emit(str(e))