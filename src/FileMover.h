#ifndef FILEMOVER_H
#define FILEMOVER_H

#include <filesystem>
#include <string>

struct MoveResult {
    bool success;
    std::string errorMessage; // empty when success is true
};

class FileMover {
public:
    static MoveResult moveFile(const std::filesystem::path& source, const std::filesystem::path& targetDirectory);
};

#endif // FILEMOVER_H