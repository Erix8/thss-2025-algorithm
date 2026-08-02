"""Cross-platform build script for SeamCarving.

Usage:
    python build.py          # full build (CLI + Python extension)
    python build.py cli      # CLI only (C++ executable, no Python deps)
"""

import os
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent


def run(cmd, **kwargs):
    print(f"[BUILD] {' '.join(cmd)}")
    subprocess.run(cmd, check=True, cwd=str(ROOT), **kwargs)


def find_pybind11_cmake_dir():
    """Return the pybind11 cmake directory from the pip-installed package."""
    import pybind11
    return pybind11.get_cmake_dir()


def build_cli_only():
    """Build only the seam_carving C++ executable (no Python deps needed)."""
    build_dir = ROOT / "build"
    run(["cmake", "-S", ".", "-B", str(build_dir)])
    run(["cmake", "--build", str(build_dir), "--target", "seam_carving",
         "--config", "Release"])


def build_full():
    """Build CLI + Python extension module (requires pybind11)."""
    pybind11_dir = find_pybind11_cmake_dir()
    build_dir = ROOT / "build"
    run(["cmake", "-S", ".", "-B", str(build_dir),
         f"-Dpybind11_DIR={pybind11_dir}"])
    run(["cmake", "--build", str(build_dir),
         "--config", "Release"])


def main():
    if len(sys.argv) > 1 and sys.argv[1] == "cli":
        print("Building CLI only (C++ executable)...")
        build_cli_only()
        exe_name = "seam_carving.exe" if sys.platform == "win32" else "seam_carving"
        exe_path = ROOT / "build" / exe_name
        alt_path = ROOT / "build" / "Release" / exe_name
        if alt_path.exists():
            exe_path = alt_path
        print(f"\nDone. Executable: {exe_path}")
        print(f"Usage: {exe_path} <input> <output> <width> <height>")
    else:
        print("Building full (CLI + Python extension)...")
        build_full()
        print("\nDone. Run 'pip install -e .' to install the Python package.")
        print("  CLI: seamcarving-cli <input> <output> <width> <height>")
        print("  GUI: seamcarving-gui")


if __name__ == "__main__":
    main()