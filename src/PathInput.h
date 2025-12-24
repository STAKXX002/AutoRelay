#ifndef PATHINPUT_H
#define PATHINPUT_H

#include <filesystem>
#include <string>

class PathInput {
public:
    static std::filesystem::path getSourcePath();
    static std::filesystem::path getTargetPath();
};

#endif // PATHINPUT_H
