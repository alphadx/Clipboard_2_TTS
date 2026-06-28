#pragma once
#include <string>
#include <shared_mutex>
#include <windows.h>

struct SHotkeyConfig {
    UINT fsModifiers;
    UINT vkCode;
    ATOM atomId;
};

struct STTSConfig {
    std::wstring voiceTokenId;
    INT speedLevel;
    USHORT volume;
    INT maxQueueSize;
};

struct SAudioConfig {
    BOOL recordingEnabled;
    UINT outputFormat;
    UINT bitrateKbps;
    std::wstring outputDirectory;
};

class CConfigManager {
public:
    CConfigManager();
    ~CConfigManager() = default;

    void LoadConfig();
    void SaveConfig();

    SHotkeyConfig GetSkipHotkey() const;
    void SetSkipHotkey(UINT modifiers, UINT vkCode);

    SHotkeyConfig GetStopAllHotkey() const;
    void SetStopAllHotkey(UINT modifiers, UINT vkCode);

    SHotkeyConfig GetPauseResumeHotkey() const;
    void SetPauseResumeHotkey(UINT modifiers, UINT vkCode);

    STTSConfig GetTTSConfig() const;
    void SetTTSConfig(const STTSConfig& config);

    SAudioConfig GetAudioConfig() const;
    void SetAudioConfig(const SAudioConfig& config);

private:
    std::wstring GetConfigPath() const;
    std::wstring GetDefaultRecordingsPath() const;
    void EnsureDirectoryExists(const std::wstring& path) const;
    
    std::wstring m_configPath;
    mutable std::shared_mutex m_mutex;

    SHotkeyConfig m_skipHotkey;
    SHotkeyConfig m_stopAllHotkey;
    SHotkeyConfig m_pauseResumeHotkey;
    
    STTSConfig m_ttsConfig;
    SAudioConfig m_audioConfig;
};
