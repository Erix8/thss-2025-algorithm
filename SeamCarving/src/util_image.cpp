#include "util_image.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../third_party/stb/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../third_party/stb/stb_image_write.h"

#include <iostream>

RGBImage readImage(const std::string &filename)
{
    RGBImage img;
    int w, h, channels;
    unsigned char *data = stbi_load(filename.c_str(), &w, &h, &channels, 3);
    if (data == nullptr)
    {
        std::cerr << "Error: cannot read image " << filename
                  << " (" << stbi_failure_reason() << ")" << std::endl;
        return img;
    }
    img.width = w;
    img.height = h;
    img.data.assign(data, data + w * h * 3);
    stbi_image_free(data);
    return img;
}

void writeImage(const RGBImage &img, const std::string &filename)
{
    if (img.data.empty())
    {
        std::cerr << "Error: empty image, nothing to write." << std::endl;
        return;
    }
    if (stbi_write_png(filename.c_str(), img.width, img.height, 3,
                       img.data.data(), img.width * 3) == 0)
    {
        std::cerr << "Error: cannot write image " << filename << std::endl;
    }
}