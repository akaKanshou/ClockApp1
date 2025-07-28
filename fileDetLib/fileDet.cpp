#include "fileDet.h"

std::filesystem::path getCur() {
    #if defined(_WIN32)
    wchar_t buf[MAX_PATH];
    GetModuleFileNameW(nullptr, buf, MAX_PATH);
    return std::filesystem::path(buf).parent_path();
    #else
        char buf[4096];
        auto len = readlink("/proc/self/exe", buf, sizeof(buf)-1);
        if (len > 0) {
            buf[len] = '\0';
            return std::filesystem::path(buf).parent_path();
        }
        return std::filesystem::current_path();
    #endif
}

std::string getAsset(std::string assetname) {
    auto path = getCur() / ".." / "share" / "ClockApp1" / assetname;
    return std::filesystem::weakly_canonical(path).string();
}

// #include <iostream>

// int main() {
//     std::cout << getAsset("Fonts/aria.tff");
    
// }