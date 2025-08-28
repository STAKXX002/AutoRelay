#ifndef FILECLASSIFIER_H
#define FILECLASSIFIER_H

#include <string>
#include <filesystem>

class FileClassifier {
public:
    static std::string getFileType(const std::filesystem::path& filePath);
    static std::string getDateSubfolder(const std::filesystem::path& filePath);
};

#endif // FILECLASSIFIER_H