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
        // Apply the same trimming/quote-stripping as interactive mode --
        // argv paths can arrive quoted or padded with whitespace when a
        // wrapper (script, GUI subprocess call) builds the command line.
        sourceDir = PathInput::sanitize(argv[1]);
        targetRoot = PathInput::sanitize(argv[2]);

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

    // Preflight: make sure the target root can actually be created/written
    // to before we start moving files. Without this, a bad target path only
    // surfaces as the same "failed to move" error repeated once per file.
    {
        std::error_code ec;
        std::filesystem::create_directories(targetRoot, ec);
        if (ec) {
            std::cerr << "Error: Cannot create or access target directory '"
                      << targetRoot.string() << "': " << ec.message() << "\n";
            return 1;
        }
    }

    // Warn (but don't block) if target is the same as, or nested inside,
    // source -- this still works since file listing is non-recursive and
    // captured up front, but produces folder structure inside the very
    // directory being organized, which is easy to mistake for a bug.
    {
        std::error_code ec;
        std::filesystem::path canonicalSource = std::filesystem::weakly_canonical(sourceDir, ec);
        std::filesystem::path canonicalTarget = ec ? targetRoot
            : std::filesystem::weakly_canonical(targetRoot, ec);

        if (!ec) {
            std::filesystem::path rel = canonicalTarget.lexically_relative(canonicalSource);
            bool sameOrNested = !rel.empty() && *rel.begin() != "..";
            if (sameOrNested) {
                std::cerr << "Warning: Target directory is the same as, or inside, "
                             "the source directory. Organized subfolders will be "
                             "created inside the folder being scanned.\n";
            }
        }
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