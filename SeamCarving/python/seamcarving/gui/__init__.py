"""Seam Carving GUI application."""

import sys


def main():
    """Launch the PySide6 GUI."""
    from seamcarving.gui.main_window import run_app

    run_app()
    sys.exit(0)


if __name__ == "__main__":
    main()
