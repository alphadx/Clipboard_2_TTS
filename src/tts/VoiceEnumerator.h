#pragma once
#include <string>
#include <vector>
#include <windows.h>
#include <atlbase.h>
#include <sapi.h>

class CVoiceEnumerator {
public:
    static HRESULT EnumerateInstalledVoices(std::vector<std::pair<std::wstring, std::wstring>>& outVoices);
};
