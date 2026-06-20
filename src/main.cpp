#include <algorithm>
#include <iostream>
#include <filesystem>
#include "FileWatcher.h"
#include "FileClassifier.h"
#include "FileMover.h"
#include "PathInput.h"

int main(int argc, char* argv[]) {
    std::filesystem::path sourceDir;
    std::filesystem::path targetRoot;

    if (argc == 3) {
        sourceDir = argv[1];
        targetRoot = argv[2];

        if (!std::filesystem::exists(sourceDir)) {
            std::cerr << "Error: Source directory does not exist.\n";
            return 1;
        }

    } else if (argc == 1) {
        sourceDir = PathInput::getSourcePath();
        targetRoot = PathInput::getTargetPath();

    } else {
        std::cerr << "Usage:\n"
                  << "  AutoRelay <source_path> <target_path>\n"
                  << "  AutoRelay                (interactive mode)\n";
        return 1;
    }

    std::vector<std::filesystem::path> files =
        FileWatcher::getFiles(sourceDir.string());

    try {
        std::filesystem::path selfPath = std::filesystem::canonical(argv[0]);
        files.erase(
            std::remove_if(files.begin(), files.end(),
                [&](const std::filesystem::path& f) {
                    return std::filesystem::canonical(f) == selfPath;
                }),
            files.end());
    } catch (const std::exception&) {
        // If argv[0] can't be resolved (e.g. unusual invocation), skip the check.
    }

    for (const auto& file : files) {
        if (std::filesystem::is_directory(file)) continue;

        std::string fileType = FileClassifier::getFileType(file);
        std::string datePath = FileClassifier::getDateSubfolder(file);

        std::filesystem::path targetDir =
            targetRoot / fileType / datePath;

        FileMover::moveFile(file, targetDir);
    }

    std::cout << "AutoRelay finished moving files.\n";
    return 0;
}
