#include "pch.h"
#include "StringUtils.h"

namespace StringUtils {

std::wstring Utf8ToWide(const std::string& utf8Str) {
    if (utf8Str.empty()) return std::wstring();
    
    int sizeNeeded = MultiByteToWideChar(CP_UTF8, 0, &utf8Str[0], static_cast<int>(utf8Str.size()), NULL, 0);
    std::wstring wideStr(sizeNeeded, 0);
    MultiByteToWideChar(CP_UTF8, 0, &utf8Str[0], static_cast<int>(utf8Str.size()), &wideStr[0], sizeNeeded);
    
    return wideStr;
}

std::string WideToUtf8(const std::wstring& wideStr) {
    if (wideStr.empty()) return std::string();
    
    int sizeNeeded = WideCharToMultiByte(CP_UTF8, 0, &wideStr[0], static_cast<int>(wideStr.size()), NULL, 0, NULL, NULL);
    std::string utf8Str(sizeNeeded, 0);
    WideCharToMultiByte(CP_UTF8, 0, &wideStr[0], static_cast<int>(wideStr.size()), &utf8Str[0], sizeNeeded, NULL, NULL);
    
    return utf8Str;
}

std::wstring FormatIso8601() {
    SYSTEMTIME st;
    GetLocalTime(&st);
    
    wchar_t buffer[64];
    swprintf_s(buffer, L"%04d%02d%02d-%02d%02d%02d",
               st.wYear, st.wMonth, st.wDay,
               st.wHour, st.wMinute, st.wSecond);
               
    return std::wstring(buffer);
}

}
