#ifndef PATHINPUT_H
#define PATHINPUT_H

#include <filesystem>
#include <string>

class PathInput {
public:
    static std::filesystem::path getSourcePath();
    static std::filesystem::path getTargetPath();

    // Trims surrounding whitespace and, if present, one matching pair of
    // quotes (common when pasting a path copied via "Copy as path" on
    // Windows, or when a path arrives quoted from argv/a GUI). Exposed so
    // callers other than the interactive prompts (e.g. argv parsing) can
    // apply the same cleanup.
    static std::string sanitize(const std::string& raw);
};

#endif // PATHINPUT_H