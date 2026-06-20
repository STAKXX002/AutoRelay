#include <algorithm>
#include <iostream>
#include <filesystem>
#include "FileWatcher.h"
#include "FileClassifier.h"
#include "FileMover.h"
#include "PathInput.h"
#include "ProgressBar.h"

int main(int argc, char* argv[]) {
    ProgressBar::enableAnsiSupport();

    std::filesystem::path sourceDir;
    std::filesystem::path targetRoot;

    if (argc == 3) {
        sourceDir = argv[1];
        targetRoot = argv[2];

         if (!std::filesystem::is_directory(sourceDir)) {
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

    // Don't move our own running executable if it happens to be sitting
    // inside the source directory.
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

    // Subdirectories aren't moved, so exclude them from the progress total.
    files.erase(
        std::remove_if(files.begin(), files.end(),
            [](const std::filesystem::path& f) {
                return std::filesystem::is_directory(f);
            }),
        files.end());

    if (files.empty()) {
        std::cout << "No files found to organize in the source directory.\n";
        return 0;
    }

    const std::size_t total = files.size();
    std::size_t processed = 0;
    std::size_t failures = 0;

    for (const auto& file : files) {
        ++processed;
        ProgressBar::update(processed, total, file.filename().string());

        std::string fileType = FileClassifier::getFileType(file);
        std::string datePath = FileClassifier::getDateSubfolder(file);

        std::filesystem::path targetDir =
            targetRoot / fileType / datePath;

        MoveResult result = FileMover::moveFile(file, targetDir);
        if (!result.success) {
            ++failures;
            ProgressBar::clear();
            std::cerr << "Error moving file " << file << ": " << result.errorMessage << "\n";
        }
    }

    ProgressBar::finish();

    std::cout << "AutoRelay finished moving files. ("
              << (total - failures) << "/" << total << " succeeded)\n";
    if (failures > 0) {
        std::cout << failures << " file(s) failed to move (see errors above).\n";
    }

    return 0;
}