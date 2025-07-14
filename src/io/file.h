#ifndef IO_FILE_H_
#define IO_FILE_H_

#include <string>
#include <vector>
#include <filesystem>

namespace io {

extern std::filesystem::path BasePath();
extern std::vector<char> ReadFile(const std::string& path);
extern bool PathExists(const std::string& path);

} // namespace io

#endif // IO_FILE_H_
