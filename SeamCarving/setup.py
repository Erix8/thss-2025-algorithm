"""Minimal setup for the seamcarving Python package.

C++ extension (_seamcarving_core) must be built separately with cmake:
    cmake -S . -B build && cmake --build build
"""

from setuptools import setup, find_packages

setup(
    packages=find_packages(where="python"),
    package_dir={"": "python"},
    package_data={
        "seamcarving": ["_seamcarving_core*.so", "_seamcarving_core*.pyd"],
    },
)