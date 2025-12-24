#include "PathInput.h"
#include <iostream>

namespace fs = std::filesystem;

static fs::path getValidPath(const std::string& prompt, bool mustExist) {
    while (true) {
        std::cout << prompt;
        std::string input;
        std::getline(std::cin, input);

        fs::path path(input);

        if (mustExist && !fs::exists(path)) {
            std::cout << " Path does not exist. Try again.\n";
        } else {
            return path;
        }
    }
}

fs::path PathInput::getSourcePath() {
    return getValidPath("Enter SOURCE directory path: ", true);
}

fs::path PathInput::getTargetPath() {
    return getValidPath("Enter TARGET directory path: ", false);
}
