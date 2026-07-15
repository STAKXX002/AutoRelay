#include "FileMover.h"
#include <filesystem>

namespace fs = std::filesystem;

MoveResult FileMover::moveFile(const fs::path& source, const fs::path& targetDirectory) {
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

        std::error_code ec;
        fs::rename(source, targetPath, ec);

        if (ec) {
            // rename() fails with "cross-device link" when source and target
            // are on different filesystems/drives (e.g. Downloads on C:,
            // target on D:, or across mounts on Linux) -- it can only move
            // within a single filesystem. Fall back to copy+delete in that
            // case; any other error is a genuine failure.
            if (ec == std::errc::cross_device_link) {
                std::error_code copyEc;
                fs::copy_file(source, targetPath, fs::copy_options::none, copyEc);
                if (copyEc) {
                    return {false, copyEc.message()};
                }

                std::error_code removeEc;
                fs::remove(source, removeEc);
                if (removeEc) {
                    // The copy succeeded but we couldn't clean up the
                    // original -- surface this rather than silently leaving
                    // a duplicate behind at the source.
                    return {false, "Copied to target but failed to remove original: " + removeEc.message()};
                }
            } else {
                return {false, ec.message()};
            }
        }

        return {true, ""};

    } catch (const std::exception& e) {
        return {false, e.what()};
    }
}