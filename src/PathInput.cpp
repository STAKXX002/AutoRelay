#include "PathInput.h"
#include <iostream>
#include <cstdlib>

namespace fs = std::filesystem;

std::string PathInput::sanitize(const std::string& raw) {
    static const char* whitespace = " \t\r\n";

    std::size_t start = raw.find_first_not_of(whitespace);
    if (start == std::string::npos) return "";
    std::size_t end = raw.find_last_not_of(whitespace);
    std::string trimmed = raw.substr(start, end - start + 1);

    if (trimmed.size() >= 2) {
        char first = trimmed.front();
        char last = trimmed.back();
        if ((first == '"' && last == '"') || (first == '\'' && last == '\'')) {
            trimmed = trimmed.substr(1, trimmed.size() - 2);
        }
    }

    return trimmed;
}

static fs::path getValidPath(const std::string& prompt, bool mustExist) {
    while (true) {
        std::cout << prompt;
        std::string input;

        if (!std::getline(std::cin, input)) {
            // EOF or stream failure (e.g. piped input ran out, Ctrl+D/Ctrl+Z).
            // Without this check, getline keeps returning instantly forever,
            // spinning the loop at full speed instead of exiting.
            std::cerr << "\nNo input received. Exiting.\n";
            std::exit(1);
        }

        input = PathInput::sanitize(input);

        if (input.empty()) {
            std::cout << " Path cannot be empty. Try again.\n";
            continue;
        }

        fs::path path(input);

        if (mustExist && !fs::is_directory(path)) {
            if (fs::exists(path)) {
                std::cout << " That path exists but is not a directory. Try again.\n";
            } else {
                std::cout << " Path does not exist. Try again.\n";
            }
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
