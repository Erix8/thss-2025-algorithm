#include "SeamCarve.h"
#include <iostream>
#include <string>

int main(int argc, char *argv[])
{
    if (argc != 5)
    {
        std::cerr << "Usage: " << argv[0]
                  << " <input_image> <output_image> <desired_width> <desired_height>"
                  << std::endl;
        return 1;
    }

    std::string inputFile = argv[1];
    std::string outputFile = argv[2];
    int desiredW = std::stoi(argv[3]);
    int desiredH = std::stoi(argv[4]);

    RGBImage img = readImage(inputFile);
    if (img.data.empty())
    {
        std::cerr << "Failed to load image." << std::endl;
        return 1;
    }

    if (desiredW > img.width || desiredH > img.height)
    {
        std::cerr << "Error: Desired size (" << desiredW << "x" << desiredH
                  << ") is larger than current image size ("
                  << img.width << "x" << img.height << ")." << std::endl;
        return 1;
    }

    // --- Remove vertical seams until width matches target ---
    while (img.width > desiredW)
    {
        auto d = calculateDisruption(img);
        auto seam = findVerticalSeam(d, img.height, img.width);
        img = removeVerticalSeam(img, seam);
    }

    // --- Remove horizontal seams until height matches target ---
    while (img.height > desiredH)
    {
        auto d = calculateDisruption(img);
        auto seam = findHorizontalSeam(d, img.height, img.width);
        img = removeHorizontalSeam(img, seam);
    }

    writeImage(img, outputFile);
    std::cout << "Saved compressed image to " << outputFile
              << " (" << img.width << "x" << img.height << ")" << std::endl;
    return 0;
}