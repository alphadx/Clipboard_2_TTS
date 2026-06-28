#include "pch.h"
#include "ConfigManager.h"
#include <shlobj.h>

CConfigManager::CConfigManager() {
    m_configPath = GetConfigPath();
    
    // Default: Ctrl + 1
    m_skipHotkey = { MOD_CONTROL, '1', 0 };
    // Default: Alt + 1
    m_stopAllHotkey = { MOD_ALT, '1', 0 };
    // Default: Ctrl + 2
    m_pauseResumeHotkey = { MOD_CONTROL, '2', 0 }; // ALT+1
    
    m_ttsConfig = { L"", 10, 80 };
    
    m_audioConfig = { FALSE, 0, 128, GetDefaultRecordingsPath() };
}

std::wstring CConfigManager::GetConfigPath() const {
    PWSTR path = NULL;
    if (SUCCEEDED(SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, NULL, &path))) {
        std::wstring fullPath(path);
        CoTaskMemFree(path);
        fullPath += L"\\ClipboardTTSMonitor";
        EnsureDirectoryExists(fullPath);
        return fullPath + L"\\config.ini";
    }
    return L"config.ini";
}

std::wstring CConfigManager::GetDefaultRecordingsPath() const {
    PWSTR path = NULL;
    if (SUCCEEDED(SHGetKnownFolderPath(FOLDERID_Documents, 0, NULL, &path))) {
        std::wstring fullPath(path);
        CoTaskMemFree(path);
        return fullPath + L"\\TTSRecordings";
    }
    return L"";
}

void CConfigManager::EnsureDirectoryExists(const std::wstring& path) const {
    CreateDirectoryW(path.c_str(), NULL);
}

void CConfigManager::LoadConfig() {
    std::unique_lock<std::shared_mutex> lock(m_mutex);
    
    // Load Hotkeys
    m_skipHotkey.fsModifiers = GetPrivateProfileIntW(L"Hotkeys", L"SkipMod", MOD_CONTROL, m_configPath.c_str());
    m_skipHotkey.vkCode = GetPrivateProfileIntW(L"Hotkeys", L"SkipVK", '1', m_configPath.c_str());

    m_stopAllHotkey.fsModifiers = GetPrivateProfileIntW(L"Hotkeys", L"StopAllMod", MOD_ALT, m_configPath.c_str());
    m_stopAllHotkey.vkCode = GetPrivateProfileIntW(L"Hotkeys", L"StopAllVK", '1', m_configPath.c_str());

    m_pauseResumeHotkey.fsModifiers = GetPrivateProfileIntW(L"Hotkeys", L"PauseResumeMod", MOD_CONTROL, m_configPath.c_str());
    m_pauseResumeHotkey.vkCode = GetPrivateProfileIntW(L"Hotkeys", L"PauseResumeVK", '2', m_configPath.c_str());
    
    // Load TTS
    wchar_t buffer[512] = {0};
    GetPrivateProfileStringW(L"TTS", L"VoiceToken", L"", buffer, 512, m_configPath.c_str());
    m_ttsConfig.voiceTokenId = buffer;
    m_ttsConfig.speedLevel = GetPrivateProfileIntW(L"TTS", L"SpeedLevel", 10, m_configPath.c_str());
    m_ttsConfig.volume = static_cast<USHORT>(GetPrivateProfileIntW(L"TTS", L"Volume", 100, m_configPath.c_str()));
    m_ttsConfig.maxQueueSize = GetPrivateProfileIntW(L"TTS", L"MaxQueueSize", 1, m_configPath.c_str());
    if (m_ttsConfig.maxQueueSize < 1) m_ttsConfig.maxQueueSize = 1;
    if (m_ttsConfig.maxQueueSize > 10) m_ttsConfig.maxQueueSize = 10;
    
    // Load Audio
    m_audioConfig.recordingEnabled = GetPrivateProfileIntW(L"Audio", L"RecordingEnabled", 0, m_configPath.c_str()) != 0;
    m_audioConfig.outputFormat = GetPrivateProfileIntW(L"Audio", L"OutputFormat", 0, m_configPath.c_str());
    m_audioConfig.bitrateKbps = GetPrivateProfileIntW(L"Audio", L"BitrateKbps", 128, m_configPath.c_str());
    
    GetPrivateProfileStringW(L"Audio", L"OutputDirectory", m_audioConfig.outputDirectory.c_str(), buffer, 512, m_configPath.c_str());
    m_audioConfig.outputDirectory = buffer;
}

void CConfigManager::SaveConfig() {
    std::shared_lock<std::shared_mutex> lock(m_mutex);
    
    // Save Hotkeys
    WritePrivateProfileStringW(L"Hotkeys", L"SkipMod", std::to_wstring(m_skipHotkey.fsModifiers).c_str(), m_configPath.c_str());
    WritePrivateProfileStringW(L"Hotkeys", L"SkipVK", std::to_wstring(m_skipHotkey.vkCode).c_str(), m_configPath.c_str());

    WritePrivateProfileStringW(L"Hotkeys", L"StopAllMod", std::to_wstring(m_stopAllHotkey.fsModifiers).c_str(), m_configPath.c_str());
    WritePrivateProfileStringW(L"Hotkeys", L"StopAllVK", std::to_wstring(m_stopAllHotkey.vkCode).c_str(), m_configPath.c_str());

    WritePrivateProfileStringW(L"Hotkeys", L"PauseResumeMod", std::to_wstring(m_pauseResumeHotkey.fsModifiers).c_str(), m_configPath.c_str());
    WritePrivateProfileStringW(L"Hotkeys", L"PauseResumeVK", std::to_wstring(m_pauseResumeHotkey.vkCode).c_str(), m_configPath.c_str());
    
    // Save TTS
    WritePrivateProfileStringW(L"TTS", L"VoiceToken", m_ttsConfig.voiceTokenId.c_str(), m_configPath.c_str());
    WritePrivateProfileStringW(L"TTS", L"SpeedLevel", std::to_wstring(m_ttsConfig.speedLevel).c_str(), m_configPath.c_str());
    WritePrivateProfileStringW(L"TTS", L"Volume", std::to_wstring(m_ttsConfig.volume).c_str(), m_configPath.c_str());
    WritePrivateProfileStringW(L"TTS", L"MaxQueueSize", std::to_wstring(m_ttsConfig.maxQueueSize).c_str(), m_configPath.c_str());
    
    // Save Audio
    WritePrivateProfileStringW(L"Audio", L"RecordingEnabled", m_audioConfig.recordingEnabled ? L"1" : L"0", m_configPath.c_str());
    WritePrivateProfileStringW(L"Audio", L"OutputFormat", std::to_wstring(m_audioConfig.outputFormat).c_str(), m_configPath.c_str());
    WritePrivateProfileStringW(L"Audio", L"BitrateKbps", std::to_wstring(m_audioConfig.bitrateKbps).c_str(), m_configPath.c_str());
    WritePrivateProfileStringW(L"Audio", L"OutputDirectory", m_audioConfig.outputDirectory.c_str(), m_configPath.c_str());
}

SHotkeyConfig CConfigManager::GetSkipHotkey() const { std::shared_lock<std::shared_mutex> lock(m_mutex); return m_skipHotkey; }
void CConfigManager::SetSkipHotkey(UINT modifiers, UINT vkCode) { std::unique_lock<std::shared_mutex> lock(m_mutex); m_skipHotkey.fsModifiers = modifiers; m_skipHotkey.vkCode = vkCode; }

SHotkeyConfig CConfigManager::GetStopAllHotkey() const { std::shared_lock<std::shared_mutex> lock(m_mutex); return m_stopAllHotkey; }
void CConfigManager::SetStopAllHotkey(UINT modifiers, UINT vkCode) { std::unique_lock<std::shared_mutex> lock(m_mutex); m_stopAllHotkey.fsModifiers = modifiers; m_stopAllHotkey.vkCode = vkCode; }

SHotkeyConfig CConfigManager::GetPauseResumeHotkey() const { std::shared_lock<std::shared_mutex> lock(m_mutex); return m_pauseResumeHotkey; }
void CConfigManager::SetPauseResumeHotkey(UINT modifiers, UINT vkCode) { std::unique_lock<std::shared_mutex> lock(m_mutex); m_pauseResumeHotkey.fsModifiers = modifiers; m_pauseResumeHotkey.vkCode = vkCode; }

STTSConfig CConfigManager::GetTTSConfig() const { std::shared_lock<std::shared_mutex> lock(m_mutex); return m_ttsConfig; }
void CConfigManager::SetTTSConfig(const STTSConfig& config) { std::unique_lock<std::shared_mutex> lock(m_mutex); m_ttsConfig = config; }

SAudioConfig CConfigManager::GetAudioConfig() const { std::shared_lock<std::shared_mutex> lock(m_mutex); return m_audioConfig; }
void CConfigManager::SetAudioConfig(const SAudioConfig& config) { std::unique_lock<std::shared_mutex> lock(m_mutex); m_audioConfig = config; }
