"""Main application window for the Seam Carving GUI."""

import sys

from PIL import Image
from PySide6.QtCore import Qt
from PySide6.QtWidgets import (
    QApplication, QMainWindow, QWidget, QHBoxLayout,
    QSplitter, QStatusBar,
)

from seamcarving.gui.control_panel import ControlPanel
from seamcarving.gui.image_panel import ImagePanel
from seamcarving.gui.worker import CompressWorker


class MainWindow(QMainWindow):
    """Main window with control panel and image display."""

    def __init__(self):
        super().__init__()
        self.setWindowTitle("Seam Carving - Content-Aware Image Resizing")
        self.setMinimumSize(900, 500)
        self.resize(1200, 700)

        self._pil_image = None
        self._worker = None

        self._setup_ui()
        self._connect_signals()

    def _setup_ui(self):
        central = QWidget()
        self.setCentralWidget(central)
        layout = QHBoxLayout(central)
        layout.setContentsMargins(4, 4, 4, 4)

        splitter = QSplitter(Qt.Orientation.Horizontal)

        self._control = ControlPanel()
        self._control.setMinimumWidth(220)
        self._control.setMaximumWidth(350)
        splitter.addWidget(self._control)

        self._image_panel = ImagePanel()
        splitter.addWidget(self._image_panel)

        splitter.setStretchFactor(0, 0)
        splitter.setStretchFactor(1, 1)
        splitter.setSizes([280, 900])
        layout.addWidget(splitter)

        self._statusbar = QStatusBar()
        self.setStatusBar(self._statusbar)
        self._statusbar.showMessage("Ready. Open an image to begin.")

    def _connect_signals(self):
        self._control.open_requested.connect(self._on_open_image)
        self._control.compress_requested.connect(self._on_compress)
        self._control.show_seams_changed.connect(self._on_show_seams_changed)
        self._control.save_requested.connect(self._on_save)
        self._control.reset_requested.connect(self._on_reset)

    # ---- Slots ----

    def _on_open_image(self, path):
        try:
            img = Image.open(path)
            if img.mode != "RGB":
                img = img.convert("RGB")
            self._pil_image = img
            self._image_panel.load_original(img)
            self._control.set_image_loaded(img)
            self._statusbar.showMessage(
                f"Loaded: {path} ({img.width} x {img.height})"
            )
        except Exception as e:
            self._statusbar.showMessage(f"Error opening image: {e}")

    def _on_compress(self, desired_w, desired_h):
        if self._pil_image is None:
            return
        arr = self._image_panel.get_original_arr()
        if arr is None:
            return
        total_seams = (arr.shape[1] - desired_w) + (arr.shape[0] - desired_h)
        self._statusbar.showMessage(
            f"Processing: removing {total_seams} seams..."
        )
        self._control.set_processing(True)
        self._worker = CompressWorker(arr, desired_w, desired_h)
        self._worker.progress.connect(self._on_progress)
        self._worker.finished.connect(self._on_finished)
        self._worker.error.connect(self._on_error)
        self._worker.start()

    def _on_progress(self, current, total):
        self._control.set_progress(current, total)

    def _on_finished(self, result, v_seams, h_seams, elapsed):
        show_seams = self._control._seam_cb.isChecked()
        self._image_panel.show_result(result, v_seams, h_seams, show_seams)
        self._control.set_result_ready(len(v_seams), len(h_seams), elapsed)
        total = len(v_seams) + len(h_seams)
        h, w = result.shape[:2]
        self._statusbar.showMessage(
            f"Done: {w} x {h} | {total} seams removed in {elapsed:.0f} ms"
        )

    def _on_error(self, msg):
        self._control.set_error(msg)
        self._statusbar.showMessage(f"Error: {msg}")

    def _on_show_seams_changed(self, show):
        self._image_panel.set_show_seams(show)

    def _on_save(self, path):
        result = self._image_panel.get_result_arr()
        if result is None:
            return
        try:
            img = Image.fromarray(result, mode="RGB")
            img.save(path)
            self._statusbar.showMessage(f"Saved: {path}")
        except Exception as e:
            self._statusbar.showMessage(f"Error saving image: {e}")

    def _on_reset(self):
        self._control.reset()
        self._image_panel = ImagePanel()
        # Replace the image panel in the splitter
        splitter = self.centralWidget().layout().itemAt(0).widget()
        old_widget = splitter.widget(1)
        if old_widget:
            old_widget.deleteLater()
        splitter.insertWidget(1, self._image_panel)
        self._pil_image = None
        self._statusbar.showMessage("Reset. Open an image to begin.")


def run_app():
    """Launch the PySide6 GUI application."""
    import os
    # Suppress macOS Input Method Kit mach port warning on exit
    os.environ.setdefault("QT_LOGGING_RULES",
                          "qt.core.machport.debug=false;"
                          "qt.core.machport.warning=false")
    app = QApplication(sys.argv)
    app.setApplicationName("Seam Carving")
    app.setOrganizationName("SeamCarving")
    app.setStyle("Fusion")
    window = MainWindow()
    window.show()
    sys.exit(app.exec())


if __name__ == "__main__":
    run_app()