#pragma once

namespace StringUtils {
    /**
     * Converts a UTF-8 string to a UTF-16 wide string.
     * @param utf8Str The UTF-8 string.
     * @return The converted wide string.
     */
    std::wstring Utf8ToWide(const std::string& utf8Str);

    /**
     * Converts a UTF-16 wide string to a UTF-8 string.
     * @param wideStr The UTF-16 wide string.
     * @return The converted UTF-8 string.
     */
    std::string WideToUtf8(const std::wstring& wideStr);

    /**
     * Formats the current local time to an ISO 8601 string suitable for filenames.
     * Format: YYYYMMDD-HHMMSS
     * @return The formatted time string.
     */
    std::wstring FormatIso8601();
}
