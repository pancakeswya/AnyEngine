#ifndef FS_FILE_H_
#define FS_FILE_H_

#include <string>
#include <vector>

namespace fs {

extern std::vector<char> ReadFile(const std::string& path);

} // namespace fs

#endif // FS_FILE_H_
