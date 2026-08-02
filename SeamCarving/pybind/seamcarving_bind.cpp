#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>
#include "SeamCarve.h"
#include "util_image.h"

#include <cstring>
#include <chrono>

namespace py = pybind11;

// ---- numpy (H, W, 3) uint8 <-> RGBImage ----

static RGBImage numpyToImage(py::array_t<uint8_t> arr)
{
    auto buf = arr.request();
    if (buf.ndim != 3)
    {
        throw std::runtime_error("Expected 3D array (H, W, 3)");
    }
    if (buf.shape[2] != 3)
    {
        throw std::runtime_error("Expected 3 channels (RGB)");
    }
    RGBImage img;
    img.height = static_cast<int>(buf.shape[0]);
    img.width = static_cast<int>(buf.shape[1]);
    img.data.assign(static_cast<uint8_t *>(buf.ptr),
                    static_cast<uint8_t *>(buf.ptr) + img.width * img.height * 3);
    return img;
}

static py::array_t<uint8_t> imageToNumpy(const RGBImage &img)
{
    auto result = py::array_t<uint8_t>({img.height, img.width, 3});
    auto buf = result.request();
    std::memcpy(buf.ptr, img.data.data(), img.data.size());
    return result;
}

// ---- Disruption as 2D numpy array ----

py::array_t<int> calculate_disruption(py::array_t<uint8_t> img_np)
{
    RGBImage img = numpyToImage(img_np);
    auto d = calculateDisruption(img);
    int h = img.height, w = img.width;
    auto result = py::array_t<int>({h, w});
    auto buf = result.request();
    int *ptr = static_cast<int *>(buf.ptr);
    for (int y = 0; y < h; ++y)
        for (int x = 0; x < w; ++x)
            ptr[y * w + x] = d[y][x];
    return result;
}

// ---- Seam finding (returns 0-indexed seam) ----

std::vector<int> find_vertical_seam(py::array_t<uint8_t> img_np)
{
    RGBImage img = numpyToImage(img_np);
    auto d = calculateDisruption(img);
    auto seam = findVerticalSeam(d, img.height, img.width);
    // Convert 1-based to 0-based
    for (auto &v : seam)
        v -= 1;
    return seam;
}

std::vector<int> find_horizontal_seam(py::array_t<uint8_t> img_np)
{
    RGBImage img = numpyToImage(img_np);
    auto d = calculateDisruption(img);
    auto seam = findHorizontalSeam(d, img.height, img.width);
    for (auto &v : seam)
        v -= 1;
    return seam;
}

// ---- Seam removal ----

py::array_t<uint8_t> remove_vertical_seam(py::array_t<uint8_t> img_np,
                                          std::vector<int> seam)
{
    RGBImage img = numpyToImage(img_np);
    // Convert 0-based to 1-based
    for (auto &v : seam)
        v += 1;
    RGBImage out = removeVerticalSeam(img, seam);
    return imageToNumpy(out);
}

py::array_t<uint8_t> remove_horizontal_seam(py::array_t<uint8_t> img_np,
                                            std::vector<int> seam)
{
    RGBImage img = numpyToImage(img_np);
    for (auto &v : seam)
        v += 1;
    RGBImage out = removeHorizontalSeam(img, seam);
    return imageToNumpy(out);
}

// ---- Full compression with seam collection ----

py::tuple compress_image(py::array_t<uint8_t> img_np,
                         int desired_w, int desired_h)
{
    RGBImage img = numpyToImage(img_np);

    if (desired_w > img.width || desired_h > img.height)
    {
        throw std::runtime_error(
            "Desired size is larger than current image size");
    }

    std::vector<std::vector<int>> vertical_seams;
    std::vector<std::vector<int>> horizontal_seams;

    // Remove vertical seams
    while (img.width > desired_w)
    {
        auto d = calculateDisruption(img);
        auto seam = findVerticalSeam(d, img.height, img.width);
        // Store 0-based seam
        std::vector<int> seam0(seam);
        for (auto &v : seam0)
            v -= 1;
        vertical_seams.push_back(seam0);
        img = removeVerticalSeam(img, seam);
    }

    // Remove horizontal seams
    while (img.height > desired_h)
    {
        auto d = calculateDisruption(img);
        auto seam = findHorizontalSeam(d, img.height, img.width);
        std::vector<int> seam0(seam);
        for (auto &v : seam0)
            v -= 1;
        horizontal_seams.push_back(seam0);
        img = removeHorizontalSeam(img, seam);
    }

    auto result = imageToNumpy(img);
    return py::make_tuple(result, vertical_seams, horizontal_seams);
}

// ---- Module definition ----

PYBIND11_MODULE(_seamcarving_core, m)
{
    m.doc() = "Seam carving C++ core (pybind11 binding)";

    m.def("calculate_disruption", &calculate_disruption,
          py::arg("img"),
          "Compute disruption map for an image (H, W, 3 uint8). "
          "Returns int32 array of shape (H, W).");

    m.def("find_vertical_seam", &find_vertical_seam,
          py::arg("img"),
          "Find one minimum-disruption vertical seam. "
          "Returns list of 0-based column indices (length = H).");

    m.def("find_horizontal_seam", &find_horizontal_seam,
          py::arg("img"),
          "Find one minimum-disruption horizontal seam. "
          "Returns list of 0-based row indices (length = W).");

    m.def("remove_vertical_seam", &remove_vertical_seam,
          py::arg("img"), py::arg("seam"),
          "Remove a vertical seam from the image. "
          "Returns new image with width-1.");

    m.def("remove_horizontal_seam", &remove_horizontal_seam,
          py::arg("img"), py::arg("seam"),
          "Remove a horizontal seam from the image. "
          "Returns new image with height-1.");

    m.def("compress_image", &compress_image,
          py::arg("img"), py::arg("desired_w"), py::arg("desired_h"),
          "Compress image to desired dimensions. "
          "Returns (result_img, vertical_seams, horizontal_seams) "
          "where seams are lists of 0-based index lists.");
}