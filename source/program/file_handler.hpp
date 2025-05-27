#pragma once

#include "lib.hpp"
#include "fs.hpp"
#include <string>

namespace FileHandler {
    // only call once !
    Result MountSD() {
        return nn::fs::MountSdCardForDebug("sd");
    }
    bool FileExists(const char *path) {
        nn::fs::DirectoryEntryType type;
        if (R_FAILED(nn::fs::GetEntryType(&type, path))) {
            return false;
        }
        return type == nn::fs::DirectoryEntryType_File;
    }
    Result ReadFile(const char *path, std::string &out) {
        nn::fs::FileHandle handle{};
        R_TRY(nn::fs::OpenFile(&handle, path, nn::fs::OpenMode_Read));
        s64 size;
        R_TRY(nn::fs::GetFileSize(&size, handle));
        char* buffer = (char*)malloc(size);
        Result res = nn::fs::ReadFile(handle, 0, buffer, size);
        if (R_FAILED(res)) {
            free(buffer);
            return res;
        }
        nn::fs::CloseFile(handle);
        out = std::string(buffer, size);
        free(buffer);
        return 0;
    }
    Result WriteFile(const char *path, std::string text) {
        nn::fs::FileHandle handle{};
        if (FileExists(path)) {
            nn::fs::DeleteFile(path);
        }
        R_TRY(nn::fs::CreateFile(path, text.length()));
        R_TRY(nn::fs::OpenFile(&handle, path, nn::fs::OpenMode_Write));
        R_TRY(nn::fs::WriteFile(handle, 0, text.c_str(), text.length(), nn::fs::WriteOption::CreateOption(nn::fs::WriteOptionFlag_Flush)));
        nn::fs::CloseFile(handle);
        return 0;
    }
}