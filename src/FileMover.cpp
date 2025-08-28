#include "FileMover.h"
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

void FileMover::moveFile(const fs::path& source, const fs::path& targetDirectory) {
    try {
        if (!fs::exists(targetDirectory)) {
            fs::create_directories(targetDirectory);
        }

        fs::path targetPath = targetDirectory / source.filename();

        // Avoid overwriting: rename if target exists
        if (fs::exists(targetPath)) {
            std::string stem = source.stem().string();
            std::string ext = source.extension().string();
            int counter = 1;

            do {
                targetPath = targetDirectory / fs::path(stem + "_" + std::to_string(counter) + ext);
                counter++;
            } while (fs::exists(targetPath));
        }

        fs::rename(source, targetPath);

        std::cout << "Moved: " << source << " → " << targetPath << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error moving file " << source << ": " << e.what() << std::endl;
    }
}
