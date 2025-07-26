#include "paths.h"
#include "fileDet.h"

std::filesystem::path FileDet::exeDir() {
#if defined(_WIN32)
    wchar_t buf[MAX_PATH];
    GetModuleFileNameW(nullptr, buf, MAX_PATH);
    return std::filesystem::path(buf).parent_path();
#else
    char buf[4096];
    auto len = readlink("/proc/self/exe", buf, sizeof(buf) - 1);
    if (len > 0) {
        buf[len] = '\0';
        return std::filesystem::path(buf).parent_path();
    }
    return std::filesystem::current_path();
#endif
}

std::filesystem::path FileDet::getRoot() {
    namespace fs = std::filesystem;

    if (fs::exists(BUILD_ASSET_DIR))
        return BUILD_ASSET_DIR;

    if (fs::exists(INSTALL_ASSET_DIR))
        return INSTALL_ASSET_DIR;

    auto exe = FileDet::exeDir();

    {
        auto candidate = fs::weakly_canonical(exe / ".." / "share" / APP_NAME);
        if (fs::exists(candidate))
            return candidate;
    }

    {
        auto candidate = exe / "assets";
        if (fs::exists(candidate))
            return candidate;
    }

    return exe;
}

std::string FileDet::getPath(FileDet::ASSET_TYPE type, std::string name) {
    auto root = getRoot();

    if (type == FileDet::SHADER) {
        root = root / "Shaders";
    }

    else if (type == FileDet::FONT) {
        root = root / "Fonts";
    }

    else if (type == FileDet::IMAGE) {
        root = root;
    }

    else {
        return "ERROR_TYPE";
    }

    std::string result = root.string();
    result += '/';
    result += name;

    return result;
}