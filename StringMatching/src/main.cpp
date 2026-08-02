#include <iostream>
#include <fstream>
#include <string>
#include "StringMatching.h"

int main()
{
    std::string textFileName, patternFileName;

    // Get text file name from user
    std::cout << "Enter the text file name (e.g., text.txt): ";
    std::cin >> textFileName;

    // Get pattern file name from user
    std::cout << "Enter the pattern file name (e.g., pattern.txt): ";
    std::cin >> patternFileName;

    // Read text file
    std::ifstream textFile(textFileName);
    if (!textFile.is_open())
    {
        std::cerr << "Failed to open text file: " << textFileName << std::endl;
        return 1;
    }
    std::string text((std::istreambuf_iterator<char>(textFile)), std::istreambuf_iterator<char>());
    textFile.close();

    // Read pattern file
    std::ifstream patternFile(patternFileName);
    if (!patternFile.is_open())
    {
        std::cerr << "Failed to open pattern file: " << patternFileName << std::endl;
        return 1;
    }
    std::string pattern((std::istreambuf_iterator<char>(patternFile)), std::istreambuf_iterator<char>());
    patternFile.close();

    // Run the test
    StringMatching_test(pattern, text);

    return 0;
}