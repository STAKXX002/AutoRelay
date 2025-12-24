#include <iostream>
#include "FileWatcher.h"
#include "FileClassifier.h"
#include "FileMover.h"
#include "PathInput.h"


int main() {
    std::filesystem::path sourceDir = PathInput::getSourcePath();
    std::filesystem::path targetRoot = PathInput::getTargetPath();

    std::vector<std::filesystem::path> files =
        FileWatcher::getFiles(sourceDir.string());

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
