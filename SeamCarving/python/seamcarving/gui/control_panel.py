"""Control panel widget with parameter inputs and action buttons."""

from PySide6.QtCore import Signal
from PySide6.QtWidgets import (
    QWidget, QVBoxLayout, QHBoxLayout, QLabel, QSpinBox,
    QPushButton, QCheckBox, QProgressBar, QGroupBox,
    QFileDialog, QMessageBox,
)

from PIL import Image


class ControlPanel(QWidget):
    """Left-side panel with settings, buttons, and progress."""

    open_requested = Signal(str)
    compress_requested = Signal(int, int)
    show_seams_changed = Signal(bool)
    save_requested = Signal(str)
    reset_requested = Signal()

    def __init__(self, parent=None):
        super().__init__(parent)
        self._original_w = 0
        self._original_h = 0
        self._has_image = False
        self._processing = False
        self._init_ui()

    def _init_ui(self):
        layout = QVBoxLayout(self)

        # --- Image info ---
        info_group = QGroupBox("Image Info")
        info_layout = QVBoxLayout(info_group)
        self._info_label = QLabel("No image loaded")
        self._info_label.setWordWrap(True)
        info_layout.addWidget(self._info_label)
        layout.addWidget(info_group)

        # --- Open button ---
        self._open_btn = QPushButton("Open Image...")
        self._open_btn.clicked.connect(self._on_open)
        layout.addWidget(self._open_btn)

        # --- Target dimensions ---
        dim_group = QGroupBox("Target Dimensions")
        dim_layout = QVBoxLayout(dim_group)

        w_layout = QHBoxLayout()
        w_layout.addWidget(QLabel("Width:"))
        self._w_spin = QSpinBox()
        self._w_spin.setRange(1, 99999)
        self._w_spin.setValue(1)
        self._w_spin.setEnabled(False)
        w_layout.addWidget(self._w_spin)
        dim_layout.addLayout(w_layout)

        h_layout = QHBoxLayout()
        h_layout.addWidget(QLabel("Height:"))
        self._h_spin = QSpinBox()
        self._h_spin.setRange(1, 99999)
        self._h_spin.setValue(1)
        self._h_spin.setEnabled(False)
        h_layout.addWidget(self._h_spin)
        dim_layout.addLayout(h_layout)

        layout.addWidget(dim_group)

        # --- Compress button ---
        self._compress_btn = QPushButton("Compress")
        self._compress_btn.setEnabled(False)
        self._compress_btn.clicked.connect(self._on_compress)
        self._compress_btn.setStyleSheet(
            "QPushButton { font-weight: bold; padding: 6px; }"
        )
        layout.addWidget(self._compress_btn)

        # --- Progress ---
        self._progress = QProgressBar()
        self._progress.setVisible(False)
        layout.addWidget(self._progress)

        # --- Options ---
        opt_group = QGroupBox("Display Options")
        opt_layout = QVBoxLayout(opt_group)
        self._seam_cb = QCheckBox("Show removed seams")
        self._seam_cb.setEnabled(False)
        self._seam_cb.toggled.connect(self.show_seams_changed.emit)
        opt_layout.addWidget(self._seam_cb)
        layout.addWidget(opt_group)

        # --- Save ---
        self._save_btn = QPushButton("Save Result...")
        self._save_btn.setEnabled(False)
        self._save_btn.clicked.connect(self._on_save)
        layout.addWidget(self._save_btn)

        # --- Reset ---
        self._reset_btn = QPushButton("Reset")
        self._reset_btn.setEnabled(False)
        self._reset_btn.clicked.connect(self._on_reset)
        layout.addWidget(self._reset_btn)

        # --- Status ---
        self._status_label = QLabel("")
        self._status_label.setWordWrap(True)
        self._status_label.setStyleSheet("color: gray; font-size: 11px;")
        layout.addWidget(self._status_label)

        layout.addStretch()

        # --- Algorithm info ---
        algo_group = QGroupBox("Algorithm Info")
        algo_layout = QVBoxLayout(algo_group)
        self._algo_label = QLabel(
            "Seam carving uses dynamic programming\n"
            "to find minimum-disruption seams.\n"
            "Complexity: Theta(mn) per seam."
        )
        self._algo_label.setWordWrap(True)
        self._algo_label.setStyleSheet("font-size: 10px; color: #666;")
        algo_layout.addWidget(self._algo_label)
        layout.addWidget(algo_group)

    def _on_open(self):
        path, _ = QFileDialog.getOpenFileName(
            self, "Open Image", "",
            "Images (*.png *.jpg *.jpeg *.bmp *.tiff *.webp);;All Files (*)",
        )
        if path:
            self.open_requested.emit(path)

    def _on_compress(self):
        w = self._w_spin.value()
        h = self._h_spin.value()
        self.compress_requested.emit(w, h)

    def _on_save(self):
        path, _ = QFileDialog.getSaveFileName(
            self, "Save Result", "",
            "PNG (*.png);;JPEG (*.jpg *.jpeg);;BMP (*.bmp);;All Files (*)",
        )
        if path:
            self.save_requested.emit(path)

    def _on_reset(self):
        self.reset_requested.emit()

    def set_image_loaded(self, pil_image):
        self._original_w = pil_image.width
        self._original_h = pil_image.height
        self._has_image = True
        self._info_label.setText(
            f"Loaded: {self._original_w} x {self._original_h} px"
        )
        self._w_spin.setEnabled(True)
        self._w_spin.setRange(1, self._original_w)
        self._w_spin.setValue(self._original_w)
        self._h_spin.setEnabled(True)
        self._h_spin.setRange(1, self._original_h)
        self._h_spin.setValue(self._original_h)
        self._compress_btn.setEnabled(True)
        self._reset_btn.setEnabled(True)

    def set_processing(self, processing):
        self._processing = processing
        self._open_btn.setEnabled(not processing)
        self._compress_btn.setEnabled(not processing)
        self._w_spin.setEnabled(not processing)
        self._h_spin.setEnabled(not processing)
        self._progress.setVisible(processing)
        if processing:
            self._progress.setValue(0)
            self._status_label.setText("Processing...")
            self._status_label.setStyleSheet("color: blue; font-size: 11px;")

    def set_progress(self, current, total):
        self._progress.setRange(0, total)
        self._progress.setValue(current)

    def set_result_ready(self, v_count, h_count, elapsed_ms):
        self._processing = False
        self._open_btn.setEnabled(True)
        self._compress_btn.setEnabled(True)
        self._w_spin.setEnabled(True)
        self._h_spin.setEnabled(True)
        self._progress.setVisible(False)
        self._seam_cb.setEnabled(True)
        self._save_btn.setEnabled(True)
        total = v_count + h_count
        self._status_label.setText(
            f"Done! {v_count} vertical + {h_count} horizontal "
            f"= {total} seams in {elapsed_ms:.0f} ms"
        )
        self._status_label.setStyleSheet("color: green; font-size: 11px;")

    def set_error(self, msg):
        self._processing = False
        self._open_btn.setEnabled(True)
        self._compress_btn.setEnabled(self._has_image)
        self._w_spin.setEnabled(self._has_image)
        self._h_spin.setEnabled(self._has_image)
        self._progress.setVisible(False)
        QMessageBox.critical(self, "Error", msg)

    def reset(self):
        self._has_image = False
        self._processing = False
        self._w_spin.setEnabled(False)
        self._h_spin.setEnabled(False)
        self._compress_btn.setEnabled(False)
        self._save_btn.setEnabled(False)
        self._seam_cb.setEnabled(False)
        self._seam_cb.setChecked(False)
        self._progress.setVisible(False)
        self._info_label.setText("No image loaded")
        self._status_label.setText("")